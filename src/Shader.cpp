#include <iostream>
#include <algorithm>
#include <math.h>
#include <cstdlib>
#include <tgmath.h>
#include "Shader.h"
#include "Ray.h"
#include "Globals.h"
#include "glm/gtx/rotate_vector.hpp"

glm::vec3 Shader::getColor(Scene* scene, int i, int j, int q, int r, int s) {
	Ray ray = Ray(i, j, scene->width, scene->height, q, r, s, scene->camera);

	int currBounce = 0;
	int maxBounce = Globals::MAX_BOUNCE;

	return Shader::getColor(scene, ray, currBounce, maxBounce);
}

glm::vec3 Shader::getColor(Scene* scene, Ray &ray, int currBounce, int maxBounce) {
	return Shader::getColor(scene, ray, currBounce, maxBounce, 0);
}

glm::vec3 Shader::getColor(Scene* scene, Ray &ray, int currBounce, int maxBounce, int GIBounce) {
	glm::vec3 final_color = glm::vec3(0.f);

	glm::vec3 local_color, reflection_color, refraction_color;

	// Get first collision
	Intersection intersection = scene->getFirstIntersection(ray);

	if (!intersection.object) {
		// Doesn't hit an object - return 0 color
		return final_color;
	}

	local_color = getLocal(intersection, scene, GIBounce);

	float reflection = intersection.object->finish.reflection;
	float filter = intersection.object->pigment.f;

	if ((reflection > 0.f || (filter > 0.f && scene->fresnel)) && currBounce < maxBounce) {
		// Add reflection!

		reflection_color = Shader::getReflection(intersection, scene, currBounce, maxBounce);
	}

	if (filter > 0.f && currBounce < maxBounce) {
		// Add refraction!

		refraction_color = Shader::getRefraction(intersection, scene, currBounce, maxBounce);
	}

	float fresnel = Shader::getFresnel(intersection, scene);

	float local_contribution = (1 - reflection) * (1 - filter);
	float reflection_contribution = (1 - filter) * reflection + filter * fresnel;
	float refraction_contribution = filter * (1 - fresnel);

	final_color += local_color * local_contribution;
	final_color += reflection_color * reflection_contribution;
	final_color += refraction_color * refraction_contribution;

	return final_color;
}

glm::vec3 Shader::getAmbient(Scene* scene, Intersection &intersection, int GIBounce) {
	glm::vec3 color = glm::vec3(0.f);

	if (!scene->gi) {
		color.r = intersection.object->finish.ambient * intersection.object->pigment.r;
		color.g = intersection.object->finish.ambient * intersection.object->pigment.g;
		color.b = intersection.object->finish.ambient * intersection.object->pigment.b;

		return color;
	}

	int bounces;
	if (GIBounce == 0) {
		// First GI samples
		bounces = Globals::FIRST_BOUNCE_GI;
	}
	else if (GIBounce == 1) {
		bounces = Globals::SECOND_BOUNCE_GI;
	}
	else {
		color.r = intersection.object->finish.ambient * intersection.object->pigment.r;
		color.g = intersection.object->finish.ambient * intersection.object->pigment.g;
		color.b = intersection.object->finish.ambient * intersection.object->pigment.b;

		return color;
	}

	int stratified = (int)sqrtf(bounces);

	float u, v;

	Ray GI_ray;
	for (int x = 0; x < stratified; x++) {
		for (int y = 0; y < stratified; y++) {
			u = ((rand() / ((float)RAND_MAX)) / (float)stratified) + ((float)x / (float)stratified);
			v = ((rand() / ((float)RAND_MAX)) / (float)stratified) + ((float)y / (float)stratified);

			GI_ray = Shader::generateHemisphereSampleRay(intersection, u, v);
			color += Shader::getColor(scene, GI_ray, 0, Globals::MAX_BOUNCE, GIBounce + 1);
		}
	}

	color *= (1.f / bounces);

	return color;
}

glm::vec3 Shader::getDiffuse(Intersection &intersection, Light* light) {
	glm::vec3 color = glm::vec3(0.f);

	glm::vec3 N = intersection.getNormal();
	glm::vec3 L = glm::normalize(light->location - intersection.position);

	float NdotL = std::max(glm::dot(N, L), 0.f);

	glm::vec3 Kd = glm::vec3(intersection.object->pigment.r, 
		intersection.object->pigment.g, 
		intersection.object->pigment.b) * 
		intersection.object->finish.diffuse;

	color.r = Kd.r * NdotL * light->pigment.r;
	color.g = Kd.g * NdotL * light->pigment.g;
	color.b = Kd.b * NdotL * light->pigment.b;

	return color;
}

glm::vec3 Shader::getSpecular(Intersection &intersection, Light* light) {
	glm::vec3 color = glm::vec3(0.f);

	glm::vec3 N = intersection.getNormal();

	glm::vec3 L = glm::normalize(light->location - intersection.position);
	glm::vec3 V = -intersection.ray.d;
	glm::vec3 H = glm::normalize(V + L);

	float HdotN = std::max(glm::dot(H, N), 0.f);

	float alpha = 2.f / powf(intersection.object->finish.roughness, 2) - 2;

	glm::vec3 Ks = glm::vec3(intersection.object->pigment.r,
		intersection.object->pigment.g,
		intersection.object->pigment.b) *
		intersection.object->finish.specular;

	color.r = Ks.r * powf(HdotN, alpha) * light->pigment.r;
	color.g = Ks.g * powf(HdotN, alpha) * light->pigment.g;
	color.b = Ks.b * powf(HdotN, alpha) * light->pigment.b;

	return color;
}

glm::vec3 Shader::getLocal(Intersection &intersection, Scene* scene, int GIBounce) {
	glm::vec3 local_color = glm::vec3(0.f);

	local_color += Shader::getAmbient(scene, intersection, GIBounce);

	for (unsigned int i = 0; i < scene->lights.size(); i++) {
		if (!scene->isInShadow(intersection, scene->lights[i])) {

			local_color += Shader::getDiffuse(intersection, scene->lights[i]);
			local_color += Shader::getSpecular(intersection, scene->lights[i]);
		}
	}

	return local_color;
}

glm::vec3 Shader::getReflection(Intersection &intersection, Scene* scene, int currBounce, int maxBounce) {
	glm::vec3 reflection_color = glm::vec3(0.f);

	Ray reflection_ray = Ray();
	// Calculate reflection ray

	glm::vec3 N = intersection.getNormal();

	reflection_ray.d = intersection.ray.d - 2 * glm::dot(intersection.ray.d, N) * N;
	reflection_ray.origin = intersection.position + reflection_ray.d * Globals::EPSILON;

	// Get reclection color recursively
	reflection_color = Shader::getColor(scene, reflection_ray, currBounce + 1, maxBounce);

	// Multiply reflection color by object pigment color
	reflection_color.x *= intersection.object->pigment.r;
	reflection_color.y *= intersection.object->pigment.g;
	reflection_color.z *= intersection.object->pigment.b;

	return reflection_color;
}

glm::vec3 Shader::getRefraction(Intersection &intersection, Scene* scene, int currBounce, int maxBounce) {
	glm::vec3 refraction_color = glm::vec3(0.f);

	Ray refraction_ray = Ray();
	// Calculate refraction ray

	glm::vec3 N = intersection.getNormal();
	float n1, n2;

	float d_dot_n = glm::dot(intersection.ray.d, N);
	if (d_dot_n < 0) {
		// Going into object
		n1 = 1.f;
		n2 = intersection.object->finish.ior;

		refraction_ray.d = glm::normalize((n1/n2) * (intersection.ray.d - d_dot_n * N) - N * sqrtf(1 - powf(n1/n2, 2) * (1 - powf(d_dot_n, 2))));
		refraction_ray.origin = intersection.position + refraction_ray.d * Globals::EPSILON;

		// Get refraction color recursively
		glm::vec3 beer = Shader::getBeer(intersection, scene, refraction_ray);

		refraction_color = Shader::getColor(scene, refraction_ray, currBounce + 1, maxBounce);

		if (!scene->beers) {
			// Multiply refraction color by object pigment color
			refraction_color.x *= intersection.object->pigment.r;
			refraction_color.y *= intersection.object->pigment.g;
			refraction_color.z *= intersection.object->pigment.b;
		}

		else {
			// Multiply refraction color by Beer's law value
			refraction_color.x *= beer.x;
			refraction_color.y *= beer.y;
			refraction_color.z *= beer.z;
		}
	}

	else {
		// Leaving object
		N = -N;
		n1 = intersection.object->finish.ior;
		n2 = 1.f;
		d_dot_n = glm::dot(intersection.ray.d, N);

		refraction_ray.d = glm::normalize((n1/n2) * (intersection.ray.d - d_dot_n * N) - N * sqrtf(1 - powf(n1/n2, 2) * (1 - powf(d_dot_n, 2))));
		refraction_ray.origin = intersection.position + refraction_ray.d * Globals::EPSILON;


		// Get refraction color recursively
		refraction_color = Shader::getColor(scene, refraction_ray, currBounce + 1, maxBounce);

	}

	return refraction_color;
}

float Shader::getFresnel(Intersection &intersection, Scene* scene) {
	float fresnel = 0.f;
	if (!scene->fresnel) {
		return fresnel;
	}

	float ior = intersection.object->finish.ior;
	float F_0 = powf(ior - 1, 2) / powf(ior + 1, 2);

	float n_dot_v = glm::dot(intersection.getNormal(), -intersection.ray.d);
	if (n_dot_v < 0) {
		n_dot_v *= -1;
	}

	fresnel = F_0 + (1 - F_0) * powf(1 - n_dot_v, 5);

	return fresnel;
}

glm::vec3 Shader::getBeer(Intersection &intersection, Scene* scene, Ray &refraction_ray) {
	glm::vec3 absorbance = glm::vec3();
	glm::vec3 attenuation = glm::vec3(1.f);

	if (!scene->beers) {
		return attenuation;
	}

	Intersection feeler;
	feeler = scene->getFirstIntersection(refraction_ray);

	if (!feeler.object) {
		std::cerr << "Beer's law ray hit nothing... uh oh" << std::endl;
		return attenuation;
	}

	float distance = glm::distance(intersection.position, feeler.position);

	absorbance.x = (1 - intersection.object->pigment.r) * Globals::BEERS_ALPHA * -distance;
	absorbance.y = (1 - intersection.object->pigment.g) * Globals::BEERS_ALPHA * -distance;
	absorbance.z = (1 - intersection.object->pigment.b) * Globals::BEERS_ALPHA * -distance;

	attenuation.x = expf(absorbance.x);
	attenuation.y = expf(absorbance.y);
	attenuation.z = expf(absorbance.z);

	return attenuation;
}

Ray Shader::generateHemisphereSampleRay(Intersection &intersection, float u, float v) {
	Ray sample_ray = Ray();

	glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 normal = intersection.getNormal();

	glm::vec3 sample_point = Shader::generateHemisphereSamplePoint(u, v);

	if (up == normal) {
		sample_ray.d = sample_point;
		sample_ray.origin = intersection.position + Globals::EPSILON * sample_ray.d;

		return sample_ray;
	}
	if (up == -normal) {
		sample_ray.d = -sample_point;
		sample_ray.origin = intersection.position + Globals::EPSILON * sample_ray.d;

		return sample_ray;
	}

	float angle = acosf(glm::dot(up, normal));
	glm::vec3 axis = glm::cross(up, normal);

	glm::mat4 R = glm::rotate(glm::mat4(1.f), angle, axis);

	glm::vec3 aligned_sample_point = glm::vec3(R * (glm::vec4(sample_point, 1.f)));

	sample_ray.d = aligned_sample_point;
	sample_ray.origin = intersection.position + Globals::EPSILON * sample_ray.d;

	return sample_ray;
}

glm::vec3 Shader::generateHemisphereSamplePoint(float u, float v) {
	float radial = sqrtf(u);
	float theta = 2.f * Globals::PI * v;

	float x = radial * cosf(theta);
	float y = radial * sinf(theta);

	return glm::vec3(x, y, sqrtf(1.f - u));
}