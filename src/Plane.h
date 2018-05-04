#ifndef PLANE_H
#define PLANE_H

#include "Object.h"

namespace Objects {
	struct Plane : public Object {
		glm::vec3 normal;
		float distance;

		Plane();
		Plane(glm::vec3 _normal, float _distance);

		std::string type();
		void print();
		float getFirstCollision(Ray *ray);
		glm::vec3 getNormal(glm::vec3 point);
	};
}

#endif