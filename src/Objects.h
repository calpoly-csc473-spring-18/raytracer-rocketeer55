#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

namespace Objects {

	struct vec3 {
		float x, y, z;
	};

	struct vec4 {
		float x, y, z, a;
	};

	union Color {
		vec3 rgb;
		vec4 rgbf;
	};

	struct Pigment {
		Color color;
		int colortype;
	};

	struct Finish {
		float ambient, diffuse;
	};

	class Object {
	public:
		Pigment pigment;
		Finish finish;

		virtual std::string type() = 0;
	};

	class Camera {
	public:
		vec3 location, up, right, look_at;
		
		virtual std::string type();
	};

	class Light {
	public:
		vec3 location;
		Pigment pigment;

		std::string type();
	};

	class Sphere : public Object {
	public:
		vec3 center;
		float radius;

		std::string type();
	};

	class Plane : public Object {
	public:
		vec3 normal;
		float distance;

		std::string type();
	};

	struct Ray {
		vec3 d, origin;

		Ray();
		Ray(int x, int y, int width, int height, Camera* camera);
	};
}

#endif