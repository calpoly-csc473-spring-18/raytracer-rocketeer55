#include <iostream>
#include "Plane.h"
#include "glm\glm.hpp"

using namespace Objects;

Plane::Plane() {
	normal = vec3();
	distance = 0.f;
}

Plane::Plane(vec3 _normal, float _distance) {
	normal = _normal;
	distance = _distance;
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

float Plane::getFirstCollision(Ray* ray) {
	glm::vec3 p0 = glm::vec3(ray->origin.x, ray->origin.y, ray->origin.z);
	glm::vec3 n = glm::vec3(normal.x, normal.y, normal.z);
	glm::vec3 dir = glm::vec3(ray->d.x, ray->d.y, ray->d.z);
	float t = (distance - glm::dot(p0, n)) / glm::dot(dir, n);
	if (t < 0) {
		return -1;
	}
	return t;
}

vec3 Plane::getNormal(vec3 point) {
	return normal;
}