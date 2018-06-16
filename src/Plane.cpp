#include <iostream>
#include "Plane.h"
#include "glm/glm.hpp"

using namespace Objects;

Plane::Plane() {
	normal = glm::vec3(0.f);
	distance = 0.f;

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);

	isLight = false;
}

Plane::Plane(glm::vec3 _normal, float _distance) {
	normal = _normal;
	distance = _distance;

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);

	isLight = false;
}

Plane::Plane(const Plane &other) {
	normal = other.normal;
	distance = other.distance;

	pigment = other.pigment;
	finish = other.finish;

	ModelMatrix = other.ModelMatrix;
	InverseMatrix = other.InverseMatrix;

	boundingBox = other.boundingBox;

	isLight = other.isLight;
}

std::string Plane::type() {
	return "Plane";
}

float Plane::getFirstCollision(Ray &ray) {
	float t = (distance - glm::dot(ray.origin, normal)) / glm::dot(ray.d, normal);
	if (t < 0) {
		return -1;
	}
	return t;
}

glm::vec3 Plane::getNormal(glm::vec3 point) {
	return normal;
}

void Plane::calculateBoundingBox() {
	// Planes are unbounded
	boundingBox = BoundingBox();
}

void Plane::setPosition(glm::vec3 position) {}

glm::vec3 Plane::getPosition() {return glm::vec3();}