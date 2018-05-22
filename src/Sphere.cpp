#include <iostream>
#include "Sphere.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace Objects;

Sphere::Sphere() {
	center = glm::vec3();
	radius = 0.f;

	ModelMatrix = glm::mat4(1.f);
}

Sphere::Sphere(glm::vec3 _center, float _radius) {
	center = _center;
	radius = _radius;

	ModelMatrix = glm::mat4(1.f);
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
	glm::vec3 d, o;
	glm::vec4 temp_d = InverseModelMatrix * glm::vec4(ray->d, 0.f);
	glm::vec4 temp_o = InverseModelMatrix * glm::vec4(ray->origin, 1.f);

	d = glm::vec3(temp_d.x, temp_d.y, temp_d.z);
	o = glm::vec3(temp_o.x, temp_o.y, temp_o.z);

	float a, b, c, discriminant, t;

	a = glm::dot(d, d);
	b = glm::dot(2.f * d, o - center);
	c = glm::dot(o - center, o - center) - (radius * radius);

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
	glm::vec4 temp = NormalMatrix * glm::vec4(point - center, 0.f);
	return glm::normalize(glm::vec3(temp.x, temp.y, temp.z));
}

void Sphere::applyTranslate(glm::vec3 translate) {
	ModelMatrix = glm::translate(glm::mat4(1.f), translate) * ModelMatrix;
}

void Sphere::applyScale(glm::vec3 scale) {
	ModelMatrix = glm::scale(glm::mat4(1.f), scale) * ModelMatrix;
}

void Sphere::applyRotation(glm::vec3 rotation) {
	glm::mat4 Rotation = glm::mat4(1.f);

	if (rotation.z != 0.f) {
		Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0, 0, 1)) * Rotation;
	}
	if (rotation.y != 0) {
		Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0, 1, 0)) * Rotation;
	}
	if (rotation.x != 0) {
		Rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1, 0, 0)) * Rotation;
	}

	ModelMatrix = Rotation * ModelMatrix;
}

void Sphere::calculateInverseModelMatrix() {
	InverseModelMatrix = glm::inverse(ModelMatrix);
}

void Sphere::calculateNormalMatrix() {
	NormalMatrix = glm::transpose(InverseModelMatrix);
}