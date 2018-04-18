#include <iostream>
#include <iomanip>
#include "Ray.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Objects;

Ray::Ray() {
	d.x = d.y = d.z = 0.f;
	origin.x = origin.y = origin.z = 0.f;
	x = y = width = height = 0;
}

Ray::Ray(int _x, int _y, int _width, int _height, Camera* camera) {
	float U, V, W;
	glm::vec3 u, v, w;

	x = _x;
	y = _y;
	width = _width;
	height = _height;

	origin = camera->location;

	U = -0.5f + (x + 0.5f) / (float)width;
	V = -0.5f + (y + 0.5f) / (float)height;
	W = -1.f;

	u = glm::vec3(camera->right.x, camera->right.y, camera->right.z);
	v = glm::vec3(camera->up.x, camera->up.y, camera->up.z);

	glm::vec3 l = glm::vec3(camera->look_at.x - camera->location.x, camera->look_at.y - camera->location.y, camera->look_at.z - camera->location.z);
	l = glm::normalize(l);
	w = l * -1.f;

	glm::vec3 D = u * U + v * V + w * W;
	D = glm::normalize(D);

	float* data = glm::value_ptr(D);
	d.x = data[0];
	d.y = data[1];
	d.z = data[2];
}

void Ray::print() {
	std::cout << "Pixel: [" << x << ", " << y << "] Ray: {";
	std::cout << std::setiosflags(std::ios::fixed);
	std::cout << std::setprecision(4);
	std::cout << origin.x << " " << origin.y << " " << origin.z << "} -> {";
	std::cout << d.x << " " << d.y << " " << d.z << "}" << std::endl;
}