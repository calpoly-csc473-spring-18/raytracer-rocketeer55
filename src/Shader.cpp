#include <iostream>
#include <algorithm>
#include "Shader.h"
#include "Ray.h"

glm::vec3 Shader::getColor(Scene* scene, int i, int j, bool print) {
	glm::vec3 color = glm::vec3(0.f);
	Ray* ray = new Ray(i, j, scene->width, scene->height, scene->camera);

	if (print) {
		ray->print();
	}

	// Get first collision
	Intersection* intersection = scene->getFirstIntersection(ray);

	if (intersection) {
		// Hits an object = calculate color!

		if (print) {
			intersection->print();
		}

		// Add ambient
		color = Shader::getAmbient(intersection->object);

		for (unsigned int i = 0; i < scene->lights.size(); i++) {
			Light* light = scene->lights[i];

			// Check if it's not in a shadow
			if (!scene->isInShadow(intersection, light)) {
				// Add diffuse
				color += Shader::getDiffuse(intersection, light);
				// Add specular
				color += Shader::getSpecular(intersection, light);
			}
		}
	}
	
	delete(ray);

	color.r = std::min(std::max(color.r, 0.f), 1.0f);
	color.g = std::min(std::max(color.g, 0.f), 1.0f);
	color.b = std::min(std::max(color.b, 0.f), 1.0f);

	return color;
}

glm::vec3 Shader::getAmbient(Object* object) {
	glm::vec3 color = glm::vec3(0.f);

	color.r = object->finish.ambient * object->pigment.r;
	color.g = object->finish.ambient * object->pigment.g;
	color.b = object->finish.ambient * object->pigment.b;

	return color;
}

glm::vec3 Shader::getDiffuse(Intersection* intersection, Light* light) {
	glm::vec3 color = glm::vec3(0.f);

	glm::vec3 N = intersection->object->getNormal(intersection->position);
	glm::vec3 L = glm::normalize(light->location - intersection->position);

	float NdotL = std::max(glm::dot(N, L), 0.f);

	glm::vec3 Kd = glm::vec3(intersection->object->pigment.r, 
		intersection->object->pigment.g, 
		intersection->object->pigment.b) * 
		intersection->object->finish.diffuse;

	color.r = Kd.r * NdotL * light->pigment.r;
	color.g = Kd.g * NdotL * light->pigment.g;
	color.b = Kd.b * NdotL * light->pigment.b;

	return color;
}

glm::vec3 Shader::getSpecular(Intersection* intersection, Light* light) {
	glm::vec3 color = glm::vec3(0.f);

	glm::vec3 N = intersection->object->getNormal(intersection->position);

	glm::vec3 L = glm::normalize(light->location - intersection->position);
	glm::vec3 V = -intersection->ray->d;
	glm::vec3 H = glm::normalize(V + L);

	float HdotN = std::max(glm::dot(H, N), 0.f);

	float alpha = 2.f / powf(intersection->object->finish.roughness, 2) - 2;

	glm::vec3 Ks = glm::vec3(intersection->object->pigment.r,
		intersection->object->pigment.g,
		intersection->object->pigment.b) *
		intersection->object->finish.specular;

	color.r = Ks.r * powf(HdotN, alpha) * light->pigment.r;
	color.g = Ks.g * powf(HdotN, alpha) * light->pigment.g;
	color.b = Ks.b * powf(HdotN, alpha) * light->pigment.b;

	return color;
}