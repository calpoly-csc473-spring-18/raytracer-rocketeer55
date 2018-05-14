#include <iostream>
#include <algorithm>
#include "Shader.h"
#include "Ray.h"
#include "Globals.h"

glm::vec3 Shader::getColor(Scene* scene, int i, int j, bool print) {
	Ray* ray = new Ray(i, j, scene->width, scene->height, scene->camera);

	int currBounce = 0;
	int maxBounce = Globals::MAX_BOUNCE;

	return Shader::getColor(scene, ray, print, currBounce, maxBounce, "Primary");
}

glm::vec3 Shader::getColor(Scene* scene, Ray* ray, bool print, int currBounce, int maxBounce, std::string iterationType) {
	glm::vec3 final_color = glm::vec3(0.f);
	glm::vec3 local_color = glm::vec3(0.f);

	glm::vec3 ambient_color = glm::vec3(0.f);
	glm::vec3 diffuse_color = glm::vec3(0.f);
	glm::vec3 specular_color = glm::vec3(0.f);

	glm::vec3 reflection_color = glm::vec3(0.f);
	glm::vec3 refraction_color = glm::vec3(0.f);

	Ray* reflection_ray = new Ray();
	Ray* refraction_ray = new Ray();

	float reflection = 0, refraction = 0;

	if (print) {
		std::cout << "----" << std::endl;
		std::cout << "Iteration type: " << iterationType << std::endl;
		ray->print();
	}

	// Get first collision
	Intersection* intersection = scene->getFirstIntersection(ray);

	if (intersection) {
		// Hits an object = calculate color!

		if (print) {
			intersection->print();
		}

		reflection = intersection->object->finish.reflection;
		refraction = intersection->object->finish.refraction;

		// Add ambient
		ambient_color = Shader::getAmbient(intersection->object) * (1 - reflection - refraction);

		for (unsigned int i = 0; i < scene->lights.size(); i++) {
			Light* light = scene->lights[i];

			// Check if it's not in a shadow
			if (!scene->isInShadow(intersection, light)) {
				// Add diffuse
				diffuse_color = Shader::getDiffuse(intersection, light) * (1 - reflection - refraction);
				// Add specular
				specular_color = Shader::getSpecular(intersection, light) * (1 - reflection - refraction);
			}
		}

		if (reflection > 0 && currBounce < maxBounce) {
			// Add reflection!

			// Calculate reflection ray

			glm::vec3 normal = intersection->object->getNormal(intersection->position);

			reflection_ray->d = ray->d - 2 * glm::dot(ray->d, normal) * normal;
			reflection_ray->origin = intersection->position + reflection_ray->d * Globals::EPSILON;

			reflection_color = Shader::getColor(scene, reflection_ray, false, currBounce + 1, maxBounce, "Reflection") * reflection;
			reflection_color.x *= intersection->object->pigment.r;
			reflection_color.y *= intersection->object->pigment.g;
			reflection_color.z *= intersection->object->pigment.b;
		}

	}

	local_color = ambient_color + diffuse_color + specular_color;

	final_color = local_color + reflection_color;

	if (print) {
		std::cout << "Final Color: {" << final_color.x << " " << final_color.y << " " << final_color.z << "}" << std::endl;
		std::cout << "Ambient: {" << ambient_color.x << " " << ambient_color.y << " " << ambient_color.z << "}" << std::endl;
		std::cout << "Diffuse: {" << diffuse_color.x << " " << diffuse_color.y << " " << diffuse_color.z << "}" << std::endl;
		std::cout << "Specular: {" << specular_color.x << " " << specular_color.y << " " << specular_color.z << "}" << std::endl;
		std::cout << "Reflection: {" << reflection_color.x << " " << reflection_color.y << " " << reflection_color.z << "}" << std::endl;
		std::cout << "Refraction: {" << refraction_color.x << " " << refraction_color.y << " " << refraction_color.z << "}" << std::endl;
		std::cout << "Contributions: " << (1 - reflection - refraction) << " Local, " << reflection << " Reflection, " << refraction << " Transmission" << std::endl;

		if (reflection > 0 && currBounce < maxBounce) { reflection_color = Shader::getColor(scene, reflection_ray, true, currBounce + 1, maxBounce, "Reflection"); }
	}
	
	//delete(ray);
	//delete(intersection);

	final_color.r = std::min(std::max(final_color.r, 0.f), 1.0f);
	final_color.g = std::min(std::max(final_color.g, 0.f), 1.0f);
	final_color.b = std::min(std::max(final_color.b, 0.f), 1.0f);

	return final_color;
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