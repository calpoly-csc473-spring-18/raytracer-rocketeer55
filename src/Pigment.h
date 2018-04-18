#ifndef PIGMENT_H
#define PIGMENT_H

#include "Vector.h"

namespace Objects {
	union Color {
		vec3 rgb;
		vec4 rgbf;

		Color();
		Color(vec3 _rgb);
		Color(vec4 _rgbf);
	};

	struct Pigment {
		Color color;
		int colortype;

		Pigment();
		Pigment(Color _color, int _colortype);

		void print();
	};
}

#endif
