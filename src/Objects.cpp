#include <string>
#include <iostream>
#include "Objects.h"
#include "Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

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

void Camera::print() {
	std::cout << "Camera:" << std::endl;
	std::cout << "- Location: {" << location.x << " " << location.y << " " << location.z << "}" << std::endl;
	std::cout << "- Up: {" << up.x << " " << up.y << " " << up.z << "}" << std::endl;
	std::cout << "- Right: {" << right.x << " " << right.y << " " << right.z << "}" << std::endl;
	std::cout << "- Look at: {" << look_at.x << " " << look_at.y << " " << look_at.z << "}" << std::endl;
}

void Light::print() {
	std::cout << "- Location: {" << location.x << " " << location.y << " " << location.z << "}" << std::endl;
	if (pigment.colortype == Globals::COLOR_RGB) {
		std::cout << "- Color: {" << pigment.color.rgb.x << " " << pigment.color.rgb.y << " " << pigment.color.rgb.z << "}" << std::endl;
	}
	else {
		std::cout << "- Color: {" << pigment.color.rgbf.x << " " << pigment.color.rgbf.y << " " << pigment.color.rgbf.z << " " << pigment.color.rgbf.a << "}" << std::endl;
	}
}

void Sphere::print() {
	std::cout << "- Type: Sphere" << std::endl;
	std::cout << "- Center: {" << center.x << " " << center.y << " " << center.z << "}" << std::endl;
	std::cout << "- Radius: " << radius << std::endl;
	if (pigment.colortype == Globals::COLOR_RGB) {
		std::cout << "- Color: {" << pigment.color.rgb.x << " " << pigment.color.rgb.y << " " << pigment.color.rgb.z << "}" << std::endl;
	}
	else {
		std::cout << "- Color: {" << pigment.color.rgbf.x << " " << pigment.color.rgbf.y << " " << pigment.color.rgbf.z << " " << pigment.color.rgbf.a << "}" << std::endl;
	}
	std::cout << "- Material: " << std::endl;
	std::cout << "  - Ambient: " << finish.ambient << std::endl;
	std::cout << "  - Diffuse: " << finish.diffuse << std::endl;
}

void Plane::print() {
	std::cout << "- Type: Plane" << std::endl;
	std::cout << "- Normal: {" << normal.x << " " << normal.y << " " << normal.z << "}" << std::endl;
	std::cout << "- Distance: " << distance << std::endl;
	if (pigment.colortype == Globals::COLOR_RGB) {
		std::cout << "- Color: {" << pigment.color.rgb.x << " " << pigment.color.rgb.y << " " << pigment.color.rgb.z << "}" << std::endl;
	}
	else {
		std::cout << "- Color: {" << pigment.color.rgbf.x << " " << pigment.color.rgbf.y << " " << pigment.color.rgbf.z << " " << pigment.color.rgbf.a << "}" << std::endl;
	}
	std::cout << "- Material: " << std::endl;
	std::cout << "  - Ambient: " << finish.ambient << std::endl;
	std::cout << "  - Diffuse: " << finish.diffuse << std::endl;
}

Ray::Ray() {
	d.x = d.y = d.z = 0.f;
	origin.x = origin.y = origin.z = 0.f;
}

Ray::Ray(int x, int y, int width, int height, Camera* camera) {
	float U, V, W;
	glm::vec3 u, v, w;

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