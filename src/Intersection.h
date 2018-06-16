#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Ray.h"
#include "Object.h"
#include "glm/glm.hpp"

using namespace Objects;

struct Intersection {
	glm::vec3 position, color;
	Ray ray;
	float t;
	Object* object;

	Intersection();
	Intersection(Ray &ray, float t, Object* object);

	glm::vec3 getNormal();
};

#endif