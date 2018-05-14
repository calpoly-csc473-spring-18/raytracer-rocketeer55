#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glm/glm.hpp"
#include "Scene.h"

namespace Shader {
	glm::vec3 getColor(Scene* scene, int i, int j, bool print);
	glm::vec3 getColor(Scene* scene, Ray* ray, bool print, int currBounce, int maxBounce, std::string iterationType);
	glm::vec3 getAmbient(Object* object);
	glm::vec3 getDiffuse(Intersection* intersection, Light* light);
	glm::vec3 getSpecular(Intersection* intersection, Light* light);
}

#endif