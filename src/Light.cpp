#include <iostream>
#include "Light.h"

using namespace Objects;

Light::Light() {
	location = vec3();
	pigment = Pigment();
}

Light::Light(vec3 _location, Pigment _pigment) {
	location = _location;
	pigment = _pigment;
}

void Light::print() {
	std::cout << "- Location: {" << location.x << " " << location.y << " " << location.z << "}" << std::endl;
	std::cout << "- Color: {";
	pigment.print();
	std::cout << "}" << std::endl;
}