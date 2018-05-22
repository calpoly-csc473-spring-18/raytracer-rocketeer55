#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

namespace Objects {
	struct Sphere : public Object {
		glm::vec3 center;
		glm::mat4 ModelMatrix, InverseModelMatrix, NormalMatrix;
		float radius;

		Sphere();
		Sphere(glm::vec3 _center, float _radius);

		std::string type();
		void print();
		float getFirstCollision(Ray *ray);
		glm::vec3 getNormal(glm::vec3 point);

		void applyTranslate(glm::vec3 translate);
		void applyScale(glm::vec3 scale);
		void applyRotation(glm::vec3 rotate);

		void calculateInverseModelMatrix();
		void calculateNormalMatrix();
	};
}

#endif