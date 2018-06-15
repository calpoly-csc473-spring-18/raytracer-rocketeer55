#include <iostream>
#include "Camera.h"

using namespace Objects;

Camera::Camera() {
	location = glm::vec3(0.f);
	up = glm::vec3(0.f);
	right = glm::vec3(0.f);
	look_at = glm::vec3(0.f);
}

Camera::Camera(glm::vec3 _location, glm::vec3 _up, glm::vec3 _right, glm::vec3 _look_at) {
	location = _location;
	up = _up;
	right = _right;
	look_at = _look_at;
}

Camera::Camera(const Camera &other) {
	location = other.location;
	up = other.up;
	right = other.right;
	look_at = other.look_at;
}

void Camera::setPosition(glm::vec3 position) {
	location = position;
}

void Camera::print() {
	std::cout << "Camera:" << std::endl;
	std::cout << "- Location: {" << location.x << " " << location.y << " " << location.z << "}" << std::endl;
	std::cout << "- Up: {" << up.x << " " << up.y << " " << up.z << "}" << std::endl;
	std::cout << "- Right: {" << right.x << " " << right.y << " " << right.z << "}" << std::endl;
	std::cout << "- Look at: {" << look_at.x << " " << look_at.y << " " << look_at.z << "}" << std::endl;
}