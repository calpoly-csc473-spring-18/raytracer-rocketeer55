#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

namespace Objects {
	struct Sphere : public Object {
		glm::vec3 center;
		float radius;

		Sphere();
		Sphere(glm::vec3 _center, float _radius);
		Sphere(const Sphere &other);

		std::string type();
		
		float getFirstCollision(Ray &ray);
		glm::vec3 getNormal(glm::vec3 point);

		void setPosition(glm::vec3 position);
		glm::vec3 getPosition();

		void calculateBoundingBox();
	};
}

#endif