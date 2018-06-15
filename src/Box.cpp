#include "Box.h"
#include <iostream>
#include <algorithm>
#include "glm/gtc/epsilon.hpp"

using namespace Objects;

Box::Box() {
	min = glm::vec3();
	max = glm::vec3();

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);

	isLight = false;
}

Box::Box(glm::vec3 _min, glm::vec3 _max) {
	min = _min;
	max = _max;

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);

	isLight = false;
}

Box::Box(const Box &other) {
	min = other.min;
	max = other.max;

	pigment = other.pigment;
	finish = other.finish;

	ModelMatrix = other.ModelMatrix;
	InverseMatrix = other.InverseMatrix;

	boundingBox = other.boundingBox;

	isLight = other.isLight;
}

std::string Box::type() {
	return "Box";
}

void Box::print() {
	std::cout << "Not printint boxes -- ayyyyyyylmao" << std::endl;
}

float Box::getFirstCollision(Ray &ray) {
	float t_xMin, t_xMax;
	float t_yMin, t_yMax;
	float t_zMin, t_zMax;

	t_xMin = (min.x - ray.origin.x) / ray.d.x;
	t_xMax = (max.x - ray.origin.x) / ray.d.x;
	t_yMin = (min.y - ray.origin.y) / ray.d.y;
	t_yMax = (max.y - ray.origin.y) / ray.d.y;
	t_zMin = (min.z - ray.origin.z) / ray.d.z;
	t_zMax = (max.z - ray.origin.z) / ray.d.z;

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
		return -1;
	}

	if (largestMin > 0) {
		return largestMin;
	}
	else {
		return smallestMax;
	}
}

glm::vec3 Box::getNormal(glm::vec3 point) {
	float epsilon = 0.001f;

	if (glm::epsilonEqual(point.x, min.x, epsilon)) {
		return glm::vec3(-1, 0, 0);
	}
	if (glm::epsilonEqual(point.x, max.x, epsilon)) {
		return glm::vec3(1, 0, 0);
	}
	if (glm::epsilonEqual(point.y, min.y, epsilon)) {
		return glm::vec3(0, -1, 0);
	}
	if (glm::epsilonEqual(point.y, max.y, epsilon)) {
		return glm::vec3(0, 1, 0);
	}
	if (glm::epsilonEqual(point.z, min.z, epsilon)) {
		return glm::vec3(0, 0, -1);
	}
	if (glm::epsilonEqual(point.z, max.z, epsilon)) {
		return glm::vec3(0, 0, 1);
	}

	std::cerr << "Box -- No normal?" << std::endl;
	return glm::vec3();
}

void Box::calculateBoundingBox() {
	boundingBox = BoundingBox(min);
	boundingBox.addPoint(max);

	boundingBox.rotate(ModelMatrix);
}


void Box::setPosition(glm::vec3 position) {}
glm::vec3 Box::getPosition() {return glm::vec3();}