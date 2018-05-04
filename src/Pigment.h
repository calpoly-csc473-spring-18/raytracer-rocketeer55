#ifndef PIGMENT_H
#define PIGMENT_H

#include <vector>

namespace Objects {
	struct Pigment {
		float r, g, b, f = 0.f;
		bool isRGB = true;

		Pigment();
		Pigment(float r, float g, float b);
		Pigment(float r, float g, float b, float f);

		void print();
	};
}

#endif
