#ifndef LIGHT_H
#define LIGHT_H

#include "Vector.h"
#include "Pigment.h"

namespace Objects {
	struct Light {
		vec3 location;
		Pigment pigment;

		Light();
		Light(vec3 _location, Pigment _pigment);

		void print();
	};
}

#endif