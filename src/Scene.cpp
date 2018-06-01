#include "Scene.h"
#include "Image.h"
#include "Globals.h"
#include "Shader.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Scene::Scene() {
	width = height = 0;
	s = 1;
	ss = fresnel = beers = sds = false;
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

void Scene::printSceneInfo() {
	// Setup cout

	std::cout << std::setiosflags(std::ios::fixed);
	std::cout << std::setprecision(4);

	// Print Camera

	camera->print();

	// Print Light Sources

	std::cout << "\n---\n\n" << lights.size() << " light(s)" << std::endl;

	for (unsigned int i = 0; i < lights.size(); i++) {
		Light* l = lights[i];
		std::cout << "\nLight[" << i << "]:" << std::endl;

		l->print();
	}

	// Print Objects

	std::cout << "\n---\n\n" << objects.size() << " object(s)" << std::endl;

	for (unsigned int i = 0; i < objects.size(); i++) {
		Object* o = objects[i];

		std::cout << "\nObject[" << i << "]:" << std::endl;

		o->print();
	}
}

void Scene::renderScene() {
	Image* image = new Image(width, height);
	glm::vec3 color;

	if (sds) {
		initSDS();
	}

	for (int i = 0; i < width; i++) {
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

	image->writeToFile(Globals::outfilename);
	delete(image);
}

void Scene::printPixelColor(int x, int y) {
	glm::vec3 color = glm::vec3(0.f);
	for (int q = 0; q < s; q++) {
		for (int r = 0; r < s; r++) {
			color += Shader::getColor((Scene*)this, x, y, q, r, s);
		}
	}
	color /= powf(s, 2);

	unsigned int red = (unsigned int)std::round(color.r * 255.f);
	unsigned int green = (unsigned int)std::round(color.g * 255.f);
	unsigned int blue = (unsigned int)std::round(color.b * 255.f);

	std::cout << "Pixel: [" << x << ", " << y << "] ";
	std::cout << "Color: (" << red << ", " << green << ", " << blue << ")" << std::endl;

	color = Shader::getColor((Scene*)this, x, y, 0, 0, 0);
}

Intersection* Scene::getFirstIntersection(Ray* ray) {
	if (sds) {
		Intersection* sds = getFirstIntersectionSDS(root, ray);

		Intersection* intersection = NULL;
		Object* nearest = NULL;
		float t = 0, nearest_t = -1;

		for (unsigned int i = 0; i < planes.size(); i++) {
			Object* o = planes[i];

			Ray* object_ray = new Ray();

			glm::vec4 temp = o->InverseMatrix * glm::vec4(ray->d, 0.f);
			object_ray->d = glm::vec3(temp);

			temp = o->InverseMatrix * glm::vec4(ray->origin, 1.f);
			object_ray->origin = glm::vec3(temp);

			t = o->getFirstCollision(object_ray);

			if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
				nearest_t = t;
				nearest = o;
			}

			delete(object_ray);
		}

		if (nearest) {
			// There was an intersection!
			intersection = new Intersection(ray, nearest_t, nearest);
		}
		else {
			return sds;
		}

		if (!sds) {
			return intersection;
		}

		if (sds->t < intersection->t) {
			delete(intersection);
			return sds;
		}
		else {
			delete(sds);
			return intersection;
		}
	}

	Intersection* intersection = NULL;
	Object* nearest = NULL;
	float t = 0, nearest_t = -1;

	for (unsigned int i = 0; i < objects.size(); i++) {
		Object* o = objects[i];

		Ray* object_ray = new Ray();

		glm::vec4 temp = o->InverseMatrix * glm::vec4(ray->d, 0.f);
		object_ray->d = glm::vec3(temp);

		temp = o->InverseMatrix * glm::vec4(ray->origin, 1.f);
		object_ray->origin = glm::vec3(temp);

		t = o->getFirstCollision(object_ray);

		if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
			nearest_t = t;
			nearest = o;
		}

		delete(object_ray);
	}

	if (nearest) {
		// There was an intersection!
		intersection = new Intersection(ray, nearest_t, nearest);
	}

	return intersection;
}

Intersection* Scene::getFirstIntersectionSDS(BVH_Node* node, Ray* ray) {
	Intersection* intersection = NULL;

	if (!node->box->intersects(ray)) {
		// Ray doesn't intersect this bounding box
		return intersection;
	}

	// Ray does hit bounding box!

	if (node->object) {
		// Node has an object - which means it has no left / right
		Ray* object_ray = new Ray();

		glm::vec4 temp = node->object->InverseMatrix * glm::vec4(ray->d, 0.f);
		object_ray->d = glm::vec3(temp);

		temp = node->object->InverseMatrix * glm::vec4(ray->origin, 1.f);
		object_ray->origin = glm::vec3(temp);

		float t = node->object->getFirstCollision(object_ray);

		delete(object_ray);

		if (t == -1) {
			// Object Ray doesn't hit object :/
			return intersection;
		}
		// We hit an object!
		return new Intersection(ray, t, node->object);
	}

	// No object in node - compare left and right
	Intersection* left = NULL;
	Intersection* right = NULL;

	if (node->left) {
		// left node exists
		left = getFirstIntersectionSDS(node->left, ray);
	}
	if (node->right) {
		// right node exists
		right = getFirstIntersectionSDS(node->right, ray);
	}

	if (left) {
		// Left node hits an object!
		if (right) {
			// Right node also hits an object! Let's JUDGE THEM
			if (left->t < right->t) {
				delete(right);
				return left;
			}
			else {
				delete(left);
				return right;
			}
		}
		else {
			// Only left node hit!
			return left;
		}
	}
	if (right) {
		// Only right node hit!
		return right;
	}
	// Nothing hit!
	return NULL;
}

bool Scene::isInShadow(Intersection* intersection, Light* light) {
	Ray* shadow_ray = new Ray(intersection->position, light->location);
	shadow_ray->origin = intersection->position + (glm::normalize(light->location - intersection->position)) * Globals::EPSILON;

	Intersection* shadowIntersection = getFirstIntersection(shadow_ray);	

	if (!shadowIntersection) {
		// No intersection or intersection after light
		delete(shadow_ray);
		return false;
	}

	if (shadowIntersection->t > glm::distance(shadow_ray->origin, light->location)) {
		delete(shadow_ray);
		delete(shadowIntersection);
		return false;
	}

	delete(shadow_ray);
	delete(shadowIntersection);

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