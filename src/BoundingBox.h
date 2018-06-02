#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Ray.h"
#include "glm/glm.hpp"

struct BoundingBox {
	glm::vec3 min, max;

	BoundingBox();
	BoundingBox(glm::vec3 point);

	void reset(glm::vec3 point);
	void addPoint(glm::vec3 point);
	void addBox(BoundingBox* other);
	void rotate(glm::mat4 ModelMatrix);

	bool intersects(Objects::Ray &ray);
};

#endif