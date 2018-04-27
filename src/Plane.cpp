#include <iostream>
#include "Plane.h"

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
	float t = (distance - ray->origin.x * normal.x + ray->origin.y * normal.y + ray->origin.z * normal.z) / (ray->d.x * normal.x + ray->d.y * normal.y + ray->d.z * normal.z);
	if (t < 0) {
		return -1;
	}
	return t;
}

vec3 Plane::getNormal(vec3 point) {
	return normal;
}