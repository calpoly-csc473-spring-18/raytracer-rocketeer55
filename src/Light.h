#ifndef LIGHT_H
#define LIGHT_H

#include "Pigment.h"
#include "glm/glm.hpp"

namespace Objects {
	struct Light {
		glm::vec3 location;
		Pigment pigment;

		Light();
		Light(glm::vec3 _location, Pigment _pigment);

		void print();
	};
}

#endif