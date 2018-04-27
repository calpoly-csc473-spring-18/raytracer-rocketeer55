#include "Vector.h"

using namespace Objects;

vec3::vec3() {
	x = y = z = 0.f;
}

vec3::vec3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

float vec3::getDistance(vec3 v1, vec3 v2) {
	return (float)sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
}

vec4::vec4() {
	x = y = z = w = 0.f;
}

vec4::vec4(float _x, float _y, float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}