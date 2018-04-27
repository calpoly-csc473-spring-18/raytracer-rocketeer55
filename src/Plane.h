#ifndef PLANE_H
#define PLANE_H

#include "Object.h"

namespace Objects {
	struct Plane : public Object {
		vec3 normal;
		float distance;

		Plane();
		Plane(vec3 _normal, float _distance);

		std::string type();
		void print();
		float getFirstCollision(Ray *ray);
		vec3 getNormal(vec3 point);
	};
}

#endif