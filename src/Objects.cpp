#include <string>
#include <iostream>
#include <math.h>
#include <iomanip>
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

void Pigment::print() {
	if (colortype == Globals::COLOR_RGB) {
		std::cout << color.rgb.x << " " << color.rgb.y << " " << color.rgb.z;
	}
	else {
		std::cout << color.rgbf.x << " " << color.rgbf.y << " " << color.rgbf.z << " " << color.rgbf.a;
	}
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
	std::cout << "- Color: {";
	pigment.print();
	std::cout << "}" << std::endl;
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

void Plane::print() {
	std::cout << "- Type: Plane" << std::endl;
	std::cout << "- Normal: {" << normal.x << " " << normal.y << " " << normal.z << "}" << std::endl;
	std::cout << "- Distance: " << distance << std::endl;
	std::cout << "- Color: {";
	pigment.print();
	std::cout << "}" << std::endl;
	std::cout << "- Material: " << std::endl;
	std::cout << "  - Ambient: " << finish.ambient << std::endl;
	std::cout << "  - Diffuse: " << finish.diffuse << std::endl;
}

float Sphere::getFirstCollision(Ray* ray) {
	float a, b, c, discriminant, t;

	glm::vec3 _d = glm::vec3(ray->d.x, ray->d.y, ray->d.z);
	glm::vec3 _origin = glm::vec3(ray->origin.x, ray->origin.y, ray->origin.z);
	glm::vec3 _center = glm::vec3(center.x, center.y, center.z);

	a = glm::dot(_d, _d);
	b = glm::dot(2.f * _d, _origin - _center);
	c = glm::dot(_origin - _center, _origin - _center) - (radius * radius);

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

float Plane::getFirstCollision(Ray* ray) {
	float t = (distance - ray->origin.x * normal.x + ray->origin.y * normal.y + ray->origin.z * normal.z) / (ray->d.x * normal.x + ray->d.y * normal.y + ray->d.z * normal.z);
	if (t < 0) {
		return -1;
	}
	return t;
}

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