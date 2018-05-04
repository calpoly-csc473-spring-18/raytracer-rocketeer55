#include "Scene.h"
#include "Image.h"
#include "Globals.h"
#include "Shader.h"
#include <iostream>
#include <iomanip>

Scene::Scene() {
	camera = new Camera();

	width = height = 0;
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

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			color = Shader::getColor((Scene*)this, i, j, false);

			unsigned int red = (unsigned int)std::round(color.r * 255.f);
			unsigned int green = (unsigned int)std::round(color.g * 255.f);
			unsigned int blue = (unsigned int)std::round(color.b * 255.f);

			image->setPixel(i, j, red, green, blue);
		}
	}

	image->writeToFile(Globals::outfilename);
}

void Scene::printPixelColor(int x, int y) {
	glm::vec3 color = Shader::getColor((Scene*)this, x, y, true);

	unsigned int red = (unsigned int)std::round(color.r * 255.f);
	unsigned int green = (unsigned int)std::round(color.g * 255.f);
	unsigned int blue = (unsigned int)std::round(color.b * 255.f);

	std::cout << "BRDF: Blinn-Phong" << std::endl;
	std::cout << "Color: (" << red << ", " << green << ", " << blue << ")" << std::endl;
}

Intersection* Scene::getFirstIntersection(Ray* ray) {
	Intersection* intersection = NULL;
	Object* nearest = NULL;
	float t = 0, nearest_t = -1;

	for (unsigned int i = 0; i < objects.size(); i++) {
		Object* o = objects[i];
		t = o->getFirstCollision(ray);

		if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
			nearest_t = t;
			nearest = o;
		}
	}

	if (nearest) {
		// There was an intersection!
		intersection = new Intersection(ray, nearest_t, nearest);
	}
	return intersection;
}

bool Scene::isInShadow(Intersection* intersection, Light* light) {
	Ray* shadow_ray = new Ray(intersection->position, light->location);
	shadow_ray->origin = intersection->position + (glm::normalize(light->location - intersection->position)) * Globals::EPSILON;

	Intersection* shadowIntersection = getFirstIntersection(shadow_ray);	

	if (!shadowIntersection || shadowIntersection->t > glm::distance(shadow_ray->origin, light->location)) {
		// No intersection or intersection after light
		delete(shadow_ray);
		return false;
	}

	delete(shadow_ray);
	delete(shadowIntersection);

	return true;
}