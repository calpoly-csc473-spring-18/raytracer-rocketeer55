#include <iostream>
#include "Intersection.h"

Intersection::Intersection() {
	position = glm::vec3(0.f);
	color = glm::vec3(0.f);
	ray = new Ray();
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
	std::cout << "T = " << t << std::endl;
	std::cout << "Object Type: " << object->type() << std::endl;

}