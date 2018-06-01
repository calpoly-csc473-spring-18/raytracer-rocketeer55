
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"

namespace Objects {
	struct Triangle : public Object{
		glm::vec3 a, b, c;

		Triangle();
		Triangle(glm::vec3 _a, glm::vec3 _b, glm::vec3 _c);

		std::string type();
		void print();
		float getFirstCollision(Ray *ray);
		glm::vec3 getNormal(glm::vec3 point);

		void calculateBoundingBox();
	};
}

#endif