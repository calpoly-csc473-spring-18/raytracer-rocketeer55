#ifndef BOX_H
#define BOX_H

#include "Object.h"

namespace Objects {
	struct Box : public Object {
		glm::vec3 min, max;

		Box();
		Box(glm::vec3 _min, glm::vec3 _max);

		std::string type();
		void print();
		float getFirstCollision(Ray &ray);
		glm::vec3 getNormal(glm::vec3 point);

		void calculateBoundingBox();
	};
}

#endif