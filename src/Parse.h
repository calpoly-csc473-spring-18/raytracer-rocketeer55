#ifndef PARSE_H
#define PARSE_H

#include <string>
#include "Vector.h"
#include "Pigment.h"
#include "Finish.h"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"

using namespace Objects;

class Parse {
public:
	Parse() {};

	static vec3 Vector(std::stringstream &Stream);
	static vec4 Vector4(std::stringstream &Stream);
	static Pigment load_pigment(std::stringstream &Stream);
	static Finish load_finish(std::stringstream &Stream);
	static Camera* load_cam(std::stringstream &Stream);
	static Light* load_light(std::stringstream &Stream);
	static Sphere* load_sphere(std::stringstream &Stream);
	static Plane* load_plane(std::stringstream &Stream);
};

#endif