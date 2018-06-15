#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "Pigment.h"
#include "Finish.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Objects {
	struct Object {
		Pigment pigment;
		Finish finish;

		glm::mat4 ModelMatrix;
		glm::mat4 InverseMatrix;

		BoundingBox boundingBox;

		bool isLight;

		virtual ~Object() {};
		
		virtual std::string type() = 0;
		virtual void print() = 0;
		virtual float getFirstCollision(Ray &ray) = 0;
		virtual glm::vec3 getNormal(glm::vec3 point) = 0;

		virtual void setPosition(glm::vec3 position) = 0;
		virtual glm::vec3 getPosition() = 0;

		virtual void calculateBoundingBox() = 0;
	};
}

#endif