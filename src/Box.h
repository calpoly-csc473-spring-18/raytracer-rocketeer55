#ifndef BOX_H
#define BOX_H

#include "Object.h"

namespace Objects {
	struct Box : public Object {
		glm::vec3 min, max;

		Box();
		Box(glm::vec3 _min, glm::vec3 _max);
		Box(const Box &other);

		std::string type();
		
		float getFirstCollision(Ray &ray);
		glm::vec3 getNormal(glm::vec3 point);

		void setPosition(glm::vec3 position);
		glm::vec3 getPosition();

		void calculateBoundingBox();
	};
}

#endif