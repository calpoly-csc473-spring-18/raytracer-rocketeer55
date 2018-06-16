#include <iostream>
#include "Light.h"

using namespace Objects;

Light::Light() {
	location = glm::vec3(0.f);
	pigment = Pigment();
}

Light::Light(glm::vec3 _location, Pigment _pigment) {
	location = _location;
	pigment = _pigment;
}

Light::Light(const Light &other) {
	location = other.location;
	pigment = other.pigment;
}

void Light::setPosition(glm::vec3 position) {
	location = position;
}