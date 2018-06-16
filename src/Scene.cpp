#include "Scene.h"
#include "Image.h"
#include "Globals.h"
#include "Shader.h"
#include "Box.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <atomic>
#include <thread>

using namespace Objects;
Scene::Scene() {
	width = height = 0;
	thread = 1;
	s = 1;
	ss = fresnel = beers = sds = gi = progress = false;
}

Scene::Scene(const Scene &other) {

	camera = new Camera(*(other.camera));
	for (unsigned int i = 0; i < other.lights.size(); i++) {
		lights.push_back(new Light(*(other.lights[i])));
	}
	for (unsigned int i = 0; i < other.objects.size(); i++) {
		if (other.objects[i]->type().compare("Box") == 0) {
			objects.push_back(new Box(*(dynamic_cast<Box*>(other.objects[i]))));
		}
		else if (other.objects[i]->type().compare("Plane") == 0) {
			objects.push_back(new Plane(*(dynamic_cast<Plane*>(other.objects[i]))));
		}
		else if (other.objects[i]->type().compare("Sphere") == 0) {
			objects.push_back(new Sphere(*(dynamic_cast<Sphere*>(other.objects[i]))));
		}
		else if (other.objects[i]->type().compare("Triangle") == 0) {
			objects.push_back(new Triangle(*(dynamic_cast<Triangle*>(other.objects[i]))));
		}
		else {
			std::cerr << "Unknown Object type - " << other.objects[i]->type() << std::endl;
		}
	}

	width = other.width;
	height = other.height;
	s = other.s;
	ss = other.ss;
	fresnel = other.fresnel;
	beers = other.beers;
	sds = other.sds;
	gi = other.gi;
	progress = other.progress;
}

Scene::~Scene() {
	delete(camera);

	for (unsigned int i = 0; i < lights.size(); i++) {
		delete(lights[i]);
	}

	for (unsigned int i = 0; i < objects.size(); i++) {
		delete(objects[i]);
	}
}

void Scene::renderScene() {
	renderScene(Globals::outfilename);
}

void Scene::renderScene(std::string filename) {
	Image* image = new Image(width, height);
	glm::vec3 color;

	if (sds) {
		initSDS();
	}

	for (int i = 0; i < width; i++) {
		if (progress) {std::cout << "x: " << i << std::endl;}
		for (int j = 0; j < height; j++) {
			color = glm::vec3(0.f);
			for (int q = 0; q < s; q++) {
				for (int r = 0; r < s; r++) {
					color += Shader::getColor((Scene*)this, i, j, q, r, s);
				}
			}

			color /= powf(s, 2);

			color.r = std::min(std::max(color.r, 0.f), 1.0f);
			color.g = std::min(std::max(color.g, 0.f), 1.0f);
			color.b = std::min(std::max(color.b, 0.f), 1.0f);

			unsigned int red = (unsigned int)std::round(color.r * 255.f);
			unsigned int green = (unsigned int)std::round(color.g * 255.f);
			unsigned int blue = (unsigned int)std::round(color.b * 255.f);
 
			image->setPixel(i, j, red, green, blue);
		}
	}

	image->writeToFile(filename);
	delete(image);
}

void Scene::renderSceneThreaded(const int numThreads) {
	Image* image = new Image(width, height);
	glm::vec3 color;

	if (sds) {
		initSDS();
	}

	int const pixelCount = width * height;

	std::atomic<int> doneCount;
	std::atomic<int> currentPixel;

	doneCount = 0;
	currentPixel = 0;

	auto RenderKernel = [&](int const threadIndex) {
		while(true) {
			int pixel = currentPixel ++;

			if (pixel >= pixelCount) {
				break;
			}

			int const x = pixel / height;
			int const y = pixel % height;

			if (progress && y == 0) {
				std::cout << "x: " << x << std::endl;
			}

			glm::vec3 color;
			for (int q = 0; q < s; q++) {
				for (int r = 0; r < s; r++) {
					color += Shader::getColor((Scene*)this, x, y, q, r, s);
				}
			}

			color /= powf(s, 2);

			color.r = std::min(std::max(color.r, 0.f), 1.0f);
			color.g = std::min(std::max(color.g, 0.f), 1.0f);
			color.b = std::min(std::max(color.b, 0.f), 1.0f);

			unsigned int red = (unsigned int)std::round(color.r * 255.f);
			unsigned int green = (unsigned int)std::round(color.g * 255.f);
			unsigned int blue = (unsigned int)std::round(color.b * 255.f);

			image->setPixel(x, y, red, green, blue);
		}

		doneCount++;

		if (threadIndex == 0) {
			while (doneCount < numThreads) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	};

	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; ++i) {
		threads.push_back(std::thread(RenderKernel, i));
	}
	for (int i = 0; i < numThreads; ++i) {
		threads[i].join();
	}

	image->writeToFile(Globals::outfilename);
	delete(image);
}

void Scene::renderScene1(const int numThreads) {
	std::atomic<int> doneCount;
	std::atomic<int> currentFrame;

	doneCount = 0;
	currentFrame = 0;

	auto RenderKernel = [&](int const threadIndex) {
		while(true) {
			int frame = currentFrame ++;
			Scene* copy = new Scene(*(this));

			if (frame >= Globals::SCENE_1_FRAMES) {
				break;
			}

			renderScene1Frame(copy, frame);

			if (progress) {
				std::cout << "frame: " << frame << std::endl;
			}

			delete(copy);
		}

		doneCount++;

		if (threadIndex == 0) {
			while (doneCount < numThreads) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	};

	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(RenderKernel, i));
	}
	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}
}

void Scene::renderScene1Frame(Scene* copy, const int frame) {
	float angle = glm::radians(frame * (360.f / Globals::SCENE_1_FRAMES));

	float xOffset = 4 * cos(angle);
	float zOffset = 4 * sin(angle);

	copy->lights[0]->location = glm::vec3(lights[0]->location.x + xOffset,
		lights[0]->location.y,
	 	lights[0]->location.z + zOffset);

	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i]->isLight) {
			glm::vec3 oldPos = objects[i]->getPosition();
			copy->objects[i]->setPosition(glm::vec3(oldPos.x + xOffset, oldPos.y, oldPos.z + zOffset));
		}
	}

	std::string filename = "scene_1_frame_" + std::to_string(frame) + ".png";

	copy->renderScene(filename);
}

void Scene::renderScene2(const int numThreads) {
	std::atomic<int> doneCount;
	std::atomic<int> currentFrame;

	doneCount = 0;
	currentFrame = 0;

	auto RenderKernel = [&](int const threadIndex) {
		while(true) {
			int frame = currentFrame ++;
			Scene* copy = new Scene(*(this));

			if (frame >= Globals::SCENE_2_FRAMES) {
				break;
			}

			renderScene2Frame(copy, frame);

			if (progress) {
				std::cout << "frame: " << frame << std::endl;
			}

			delete(copy);
		}

		doneCount++;

		if (threadIndex == 0) {
			while (doneCount < numThreads) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	};

	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(RenderKernel, i));
	}
	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}
}

void Scene::renderScene2Frame(Scene* copy, const int frame) {
	// First figure out velocity based on frame
	glm::vec2 velocity = glm::vec2(-1.3f, 0.f); // Moving 1 unit per second to the left
	glm::vec2 position = glm::vec2(objects[0]->getPosition().x, objects[0]->getPosition().y);

	for (int i = 0; i <= frame; i++) {
		velocity.y -= 0.2f;

		position += velocity * (1.f / Globals::FPS);

		// Don't let the bottom of the sphere be below the floor
		// Floor y-value is -4, and Sphere radius is 2, so center must be above -2
		if (position.y < -2) {
			velocity.y *= -0.9; // Set velocity to be opposite, and bounce a little less high
			position.y = -2;
		}
	}

	// Now position should be accurate for this frame
	copy->objects[0]->setPosition(glm::vec3(position, objects[0]->getPosition().z)); // Set position in the copy scene

	std::string filename = "scene_2_frame_" + std::to_string(frame) + ".png";

	copy->renderScene(filename);
}

void Scene::renderScene3(const int numThreads) {
	std::atomic<int> doneCount;
	std::atomic<int> currentFrame;

	doneCount = 0;
	currentFrame = 99;

	auto RenderKernel = [&](int const threadIndex) {
		while(true) {
			int frame = currentFrame ++;
			Scene* copy = new Scene(*(this));

			if (frame >= Globals::SCENE_3_FRAMES) {
				break;
			}

			renderScene3Frame(copy, frame);

			if (progress) {
				std::cout << "frame: " << frame << std::endl;
			}

			delete(copy);
		}

		doneCount++;

		if (threadIndex == 0) {
			while (doneCount < numThreads) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	};

	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(RenderKernel, i));
	}
	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}
}

void Scene::renderScene3Frame(Scene* copy, const int frame) {
	// Camera moves 1 unit per frame, starting at z=0, moving in negative z direction
	copy->camera->setPosition(glm::vec3(0.f, 0.f, -frame));

	for (int i = 0; i < Globals::SCENE_3_LIGHTS; i++) {
		// Add blue light
		Light* l = new Light(*(lights[1]));
		l->setPosition(glm::vec3(0, 25, i * -200));
		copy->lights.push_back(l);

		// Add blue sphere
		Sphere* s = new Sphere(*(dynamic_cast<Sphere*>(objects[1])));
		s->setPosition(glm::vec3(0, 25, i * -200));
		copy->objects.push_back(s);
	}

	for (int i = 0; i < Globals::SCENE_3_LIGHTS; i++) {
		// Add red light
		Light* l = new Light(*(lights[0]));
		l->setPosition(glm::vec3(0, 25, -100 + (i * -200)));
		copy->lights.push_back(l);

		// Add red sphere
		Sphere* s = new Sphere(*(dynamic_cast<Sphere*>(objects[0])));
		s->setPosition(glm::vec3(0, 25, -100 + (i * -200)));
		copy->objects.push_back(s);
	}

	std::string filename = "scene_3_frame_" + std::to_string(frame) + ".png";

	copy->renderScene(filename);
}

Intersection Scene::getFirstIntersection(Ray &ray) {
	if (sds) {
		Intersection sds = getFirstIntersectionSDS(root, ray);

		Intersection intersection;
		Object* nearest = NULL;
		float t = 0, nearest_t = -1;

		for (unsigned int i = 0; i < planes.size(); i++) {
			Object* o = planes[i];

			Ray object_ray = Ray();

			glm::vec4 temp = o->InverseMatrix * glm::vec4(ray.d, 0.f);
			object_ray.d = glm::vec3(temp);

			temp = o->InverseMatrix * glm::vec4(ray.origin, 1.f);
			object_ray.origin = glm::vec3(temp);

			t = o->getFirstCollision(object_ray);

			if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
				nearest_t = t;
				nearest = o;
			}
		}

		if (nearest) {
			// There was an intersection!
			intersection = Intersection(ray, nearest_t, nearest);
		}
		else {
			return sds;
		}

		if (!sds.object) {
			return intersection;
		}

		if (sds.t < intersection.t) {
			return sds;
		}
		else {
			return intersection;
		}
	}

	Intersection intersection;
	Object* nearest = NULL;
	float t = 0, nearest_t = -1;

	for (unsigned int i = 0; i < objects.size(); i++) {
		Object* o = objects[i];

		Ray object_ray = Ray();

		glm::vec4 temp = o->InverseMatrix * glm::vec4(ray.d, 0.f);
		object_ray.d = glm::vec3(temp);

		temp = o->InverseMatrix * glm::vec4(ray.origin, 1.f);
		object_ray.origin = glm::vec3(temp);

		t = o->getFirstCollision(object_ray);

		if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
			if (!(ray.isShadowRay && o->isLight)) {
				nearest_t = t;
				nearest = o;
			}
		}
	}

	if (nearest) {
		// There was an intersection!
		intersection = Intersection(ray, nearest_t, nearest);
	}

	return intersection;
}

Intersection Scene::getFirstIntersectionSDS(BVH_Node* node, Ray &ray) {
	Intersection intersection;

	if (!node->box.intersects(ray)) {
		// Ray doesn't intersect this bounding box
		return intersection;
	}

	// Ray does hit bounding box!

	if (node->object) {
		// Node has an object - which means it has no left / right
		Ray object_ray = Ray();

		glm::vec4 temp = node->object->InverseMatrix * glm::vec4(ray.d, 0.f);
		object_ray.d = glm::vec3(temp);

		temp = node->object->InverseMatrix * glm::vec4(ray.origin, 1.f);
		object_ray.origin = glm::vec3(temp);

		float t = node->object->getFirstCollision(object_ray);

		if (t == -1) {
			// Object Ray doesn't hit object :/
			return intersection;
		}
		// We hit an object!
		return Intersection(ray, t, node->object);
	}

	// No object in node - compare left and right
	Intersection left;
	Intersection right;

	if (node->left) {
		// left node exists
		left = getFirstIntersectionSDS(node->left, ray);
	}
	if (node->right) {
		// right node exists
		right = getFirstIntersectionSDS(node->right, ray);
	}

	if (left.object) {
		// Left node hits an object!
		if (right.object) {
			// Right node also hits an object! Let's JUDGE THEM
			if (left.t < right.t) {
				return left;
			}
			else {
				return right;
			}
		}
		else {
			// Only left node hit!
			return left;
		}
	}
	if (right.object) {
		// Only right node hit!
		return right;
	}
	// Nothing hit!
	return intersection;
}

bool Scene::isInShadow(Intersection &intersection, Light* light) {
	Ray shadow_ray = Ray(intersection.position, light->location);
	shadow_ray.origin = intersection.position + (glm::normalize(light->location - intersection.position)) * Globals::EPSILON;

	shadow_ray.isShadowRay = true;

	Intersection shadowIntersection = getFirstIntersection(shadow_ray);	

	if (!shadowIntersection.object) {
		// No intersection or intersection after light
		return false;
	}

	if (shadowIntersection.t > glm::distance(shadow_ray.origin, light->location)) {
		return false;
	}

	return true;
}

void Scene::initSDS() {
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i]->type().compare("Plane") == 0) {
			// is a plane - don't put it in sds
			planes.push_back(objects[i]);
		}
		else {
			objects_without_planes.push_back(objects[i]);
		}
	}

	for (unsigned int i = 0; i < objects_without_planes.size(); i++) {
		objects_without_planes[i]->calculateBoundingBox();
	}

	root = new BVH_Node();
	root->buildTree(objects_without_planes, 0);
}