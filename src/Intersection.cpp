#include <iostream>
#include "Intersection.h"

Intersection::Intersection() {
	position = glm::vec3(0.f);
	color = glm::vec3(0.f);
	t = 0.f;
	object = NULL;
}

Intersection::Intersection(Ray* _ray, float _t, Object* _object) {
	ray = _ray;
	t = _t;
	object = _object;

	position = ray->origin + t * ray->d;
	color = glm::vec3(0.f);
}

void Intersection::print() {
	std::cout << "Hit Object: (ID #" << object->ID << " - " << object->type() << ")" << std::endl;
	std::cout << "Intersection: {" << position.x << " " << position.y << " " << position.z << "} ";
	std::cout << "at T = " << t << std::endl;
	glm::vec3 normal = object->getNormal(position);
	std::cout << "Normal: {" << normal.x << " " << normal.y << " " << normal.z << "}" << std::endl;
}

glm::vec3 Intersection::getNormal() {
	glm::vec3 new_point = object->InverseMatrix * glm::vec4(position, 1.0f);
	glm::vec3 normal = glm::transpose(object->InverseMatrix) * glm::vec4(object->getNormal(new_point), 0.f);

	return glm::normalize(normal);
}