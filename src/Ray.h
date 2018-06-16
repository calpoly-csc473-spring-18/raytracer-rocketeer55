#ifndef RAY_H
#define RAY_H

#include "Camera.h"

namespace Objects {
	struct Ray {
		glm::vec3 d, origin;
		int x, y, width, height;

		bool isShadowRay = false;
		
		Ray();
		Ray(int _x, int _y, int _width, int _height, int q, int r, int s, Camera* camera);
		Ray(glm::vec3 _origin, glm::vec3 _destination);
	};
}

#endif