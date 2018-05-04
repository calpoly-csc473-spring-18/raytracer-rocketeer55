#include <iostream>
#include "Sphere.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Objects;

Sphere::Sphere() {
	center = glm::vec3();
	radius = 0.f;
}

Sphere::Sphere(glm::vec3 _center, float _radius) {
	center = _center;
	radius = _radius;
}

std::string Sphere::type() {
	return "Sphere";
}

void Sphere::print() {
	std::cout << "- Type: Sphere" << std::endl;
	std::cout << "- Center: {" << center.x << " " << center.y << " " << center.z << "}" << std::endl;
	std::cout << "- Radius: " << radius << std::endl;
	std::cout << "- Color: {";
	pigment.print();
	std::cout << "}" << std::endl;
	std::cout << "- Material: " << std::endl;
	std::cout << "  - Ambient: " << finish.ambient << std::endl;
	std::cout << "  - Diffuse: " << finish.diffuse << std::endl;
}

float Sphere::getFirstCollision(Ray* ray) {
	float a, b, c, discriminant, t;

	a = glm::dot(ray->d, ray->d);
	b = glm::dot(2.f * ray->d, ray->origin - center);
	c = glm::dot(ray->origin - center, ray->origin - center) - (radius * radius);

	discriminant = (b * b) - (4 * a * c);
	if (discriminant == 0) {
		// One collision
		t = -b / (2 * a);
		if (t >= 0) {
			return t;
		}
		return -1;
	}
	else if (discriminant > 0) {
		// Two solutions
		t = (-b - sqrt(discriminant)) / (2 * a);
		if (t < 0) {
			t = (-b + sqrt(discriminant)) / (2 * a);
			if (t < 0) {
				return -1;
			}
			return t;
		}
		return t;
	}
	// No solutions

	return -1;
}

glm::vec3 Sphere::getNormal(glm::vec3 point) {
	return glm::normalize(point - center);
}