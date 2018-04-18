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

vec4::vec4() {
	x = y = z = w = 0.f;
}

vec4::vec4(float _x, float _y, float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}