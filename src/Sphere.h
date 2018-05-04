#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

namespace Objects {
	struct Sphere : public Object {
		glm::vec3 center;
		float radius;

		Sphere();
		Sphere(glm::vec3 _center, float _radius);

		std::string type();
		void print();
		float getFirstCollision(Ray *ray);
		glm::vec3 getNormal(glm::vec3 point);
	};
}

#endif