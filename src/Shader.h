#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glm/glm.hpp"
#include "Scene.h"

namespace Shader {
	glm::vec3 getColor(Scene* scene, int i, int j, int q, int r, int s);
	glm::vec3 getColor(Scene* scene, Ray* ray, int currBounce, int maxBounce, std::string iterationType);

	glm::vec3 getAmbient(Object* object);
	glm::vec3 getDiffuse(Intersection* intersection, Light* light);
	glm::vec3 getSpecular(Intersection* intersection, Light* light);

	glm::vec3 getLocal(Intersection* intersection, Scene* scene);
	glm::vec3 getReflection(Intersection* intersection, Scene* scene, int currBounce, int maxBounce);
	glm::vec3 getRefraction(Intersection* intersection, Scene* scene, int currBounce, int maxBounce);

	float getFresnel(Intersection* intersection, Scene* scene);
}

#endif