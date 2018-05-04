#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "Pigment.h"
#include "Finish.h"
#include "Ray.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Objects {
	struct Object {
		Pigment pigment;
		Finish finish;
		
		virtual std::string type() = 0;
		virtual void print() = 0;
		virtual float getFirstCollision(Ray* ray) = 0;
		virtual glm::vec3 getNormal(glm::vec3 point) = 0;
	};
}

#endif