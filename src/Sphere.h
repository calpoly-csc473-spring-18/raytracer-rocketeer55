#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

namespace Objects {
	struct Sphere : public Object {
		vec3 center;
		float radius;

		Sphere();
		Sphere(vec3 _center, float _radius);

		std::string type();
		void print();
		float getFirstCollision(Ray *ray);
		vec3 getNormal(vec3 point);
	};
}

#endif