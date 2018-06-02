#include <iostream>
#include "Plane.h"
#include "glm/glm.hpp"

using namespace Objects;

Plane::Plane() {
	normal = glm::vec3(0.f);
	distance = 0.f;

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);
}

Plane::Plane(glm::vec3 _normal, float _distance) {
	normal = _normal;
	distance = _distance;

	ModelMatrix = glm::mat4(1.f);
	InverseMatrix = glm::mat4(1.f);
}

std::string Plane::type() {
	return "Plane";
}

void Plane::print() {
	std::cout << "- Type: Plane" << std::endl;
	std::cout << "- Normal: {" << normal.x << " " << normal.y << " " << normal.z << "}" << std::endl;
	std::cout << "- Distance: " << distance << std::endl;
	std::cout << "- Color: {";
	pigment.print();
	std::cout << "}" << std::endl;
	std::cout << "- Material: " << std::endl;
	std::cout << "  - Ambient: " << finish.ambient << std::endl;
	std::cout << "  - Diffuse: " << finish.diffuse << std::endl;
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
	boundingBox = NULL;
}