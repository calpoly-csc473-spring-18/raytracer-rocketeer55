#ifndef RAY_H
#define RAY_H

#include "Vector.h"
#include "Camera.h"

namespace Objects {
	struct Ray {
		vec3 d, origin;
		int x, y, width, height;

		Ray();
		Ray(int _x, int _y, int _width, int _height, Camera* camera);

		void print();
	};
}

#endif