#include <string>
#include "Objects.h"

using namespace Objects;

std::string Camera::type() {
	return "Camera";
}

std::string Light::type() {
	return "Light";
}

std::string Sphere::type() {
	return "Sphere";
}

std::string Plane::type() {
	return "Plane";
}