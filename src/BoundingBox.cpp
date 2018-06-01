#include "BoundingBox.h"
#include <algorithm>

using namespace Objects;

BoundingBox::BoundingBox() {
	min = max = glm::vec3();
}

BoundingBox::BoundingBox(glm::vec3 point) {
	min = max = point;
}

void BoundingBox::reset(glm::vec3 point) {
	min = max = point;
}

void BoundingBox::addPoint(glm::vec3 point) {
	min.x = std::min(min.x, point.x);
	max.x = std::max(max.x, point.x);

	min.y = std::min(min.y, point.y);
	max.y = std::max(max.y, point.y);

	min.z = std::min(min.z, point.z);
	max.z = std::max(max.z, point.z);
}

void BoundingBox::addBox(BoundingBox* other) {
	addPoint(other->min);
	addPoint(other->max);
}

void BoundingBox::rotate(glm::mat4 ModelMatrix) {
	std::vector<glm::vec3> corners;

	corners.push_back(glm::vec3(min.x, min.y, min.z));
	corners.push_back(glm::vec3(min.x, min.y, max.z));
	corners.push_back(glm::vec3(min.x, max.y, min.z));
	corners.push_back(glm::vec3(min.x, max.y, max.z));
	corners.push_back(glm::vec3(max.x, min.y, min.z));
	corners.push_back(glm::vec3(max.x, min.y, max.z));
	corners.push_back(glm::vec3(max.x, max.y, min.z));
	corners.push_back(glm::vec3(max.x, max.y, max.z));

	reset(corners[0]);
	for (unsigned int i = 0; i < corners.size(); i++) {
		addPoint(glm::vec3(ModelMatrix * glm::vec4(corners[i], 1)));
	}
}

bool BoundingBox::intersects(Ray* ray) {
	float t_xMin, t_xMax;
	float t_yMin, t_yMax;
	float t_zMin, t_zMax;

	t_xMin = (min.x - ray->origin.x) / ray->d.x;
	t_xMax = (max.x - ray->origin.x) / ray->d.x;
	t_yMin = (min.y - ray->origin.y) / ray->d.y;
	t_yMax = (max.y - ray->origin.y) / ray->d.y;
	t_zMin = (min.z - ray->origin.z) / ray->d.z;
	t_zMax = (max.z - ray->origin.z) / ray->d.z;

	if (t_xMin > t_xMax) {
		float temp = t_xMin;
		t_xMin = t_xMax;
		t_xMax = temp;
	}

	if (t_yMin > t_yMax) {
		float temp = t_yMin;
		t_yMin = t_yMax;
		t_yMax = temp;
	}

	if (t_zMin > t_zMax) {
		float temp = t_zMin;
		t_zMin = t_zMax;
		t_zMax = temp;
	}

	float smallestMax = std::min(t_xMax, std::min(t_yMax, t_zMax));
	float largestMin = std::max(t_xMin, std::max(t_yMin, t_zMin));

	if (smallestMax < largestMin || smallestMax < 0) {
		return false;
	}

	return true;
}