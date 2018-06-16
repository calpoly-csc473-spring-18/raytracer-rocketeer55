#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

namespace Objects {
	struct Camera {
		glm::vec3 location, up, right, look_at;

		Camera();
		Camera(glm::vec3 _location, glm::vec3 _up, glm::vec3 _right, glm::vec3 _look_at);
		Camera(const Camera &other);

		void setPosition(glm::vec3 position);
	};
}

#endif