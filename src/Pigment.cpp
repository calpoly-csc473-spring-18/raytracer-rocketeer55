#include <iostream>

#include "Pigment.h"
#include "Globals.h"

using namespace Objects;

Color::Color() {}

Color::Color(vec3 _rgb) {
	rgb = _rgb;
}

Color::Color(vec4 _rgbf) {
	rgbf = _rgbf;
}

Pigment::Pigment() {
	color = Color();
	colortype = -1;
}

Pigment::Pigment(Color _color, int _colortype) {
	color = _color;
	colortype = _colortype;
}

void Pigment::print() {
	if (colortype == Globals::COLOR_RGB) {
		std::cout << color.rgb.x << " " << color.rgb.y << " " << color.rgb.z;
	}
	else {
		std::cout << color.rgbf.x << " " << color.rgbf.y << " " << color.rgbf.z << " " << color.rgbf.w;
	}
}