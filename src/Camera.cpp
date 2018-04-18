#include <iostream>
#include "Camera.h"

using namespace Objects;

Camera::Camera() {
	location = vec3();
	up = vec3();
	right = vec3();
	look_at = vec3();
}

Camera::Camera(vec3 _location, vec3 _up, vec3 _right, vec3 _look_at) {
	location = _location;
	up = _up;
	right = _right;
	look_at = _look_at;
}

void Camera::print() {
	std::cout << "Camera:" << std::endl;
	std::cout << "- Location: {" << location.x << " " << location.y << " " << location.z << "}" << std::endl;
	std::cout << "- Up: {" << up.x << " " << up.y << " " << up.z << "}" << std::endl;
	std::cout << "- Right: {" << right.x << " " << right.y << " " << right.z << "}" << std::endl;
	std::cout << "- Look at: {" << look_at.x << " " << look_at.y << " " << look_at.z << "}" << std::endl;
}