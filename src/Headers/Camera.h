#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"

namespace Objects {
	struct Camera {
		vec3 location, up, right, look_at;

		Camera();
		Camera(vec3 _location, vec3 _up, vec3 _right, vec3 _look_at);

		void print();
	};
}

#endif