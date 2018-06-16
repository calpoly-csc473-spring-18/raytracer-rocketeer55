#include <iostream>
#include "Pigment.h"

using namespace Objects;

Pigment::Pigment() {
	isRGB = true;
	r = 0.f;
	g = 0.f;
	b = 0.f;
	f = 0.f;
}

Pigment::Pigment(float _r, float _g, float _b) {
	isRGB = true;
	r = _r;
	g = _g;
	b = _b;
	f = 0.f;
}

Pigment::Pigment(float _r, float _g, float _b, float _f) {
	isRGB = false;
	r = _r;
	g = _g; 
	b = _b;
	f = _f;
}