#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

namespace Objects {

	struct vec3 {
		float x, y, z;

		vec3();
		vec3(float _x, float _y, float _z);

		static float getDistance(vec3 v1, vec3 v2);
	};

	struct vec4 {
		float x, y, z, w;

		vec4();
		vec4(float _x, float _y, float _z, float _w);
	};
}

#endif
