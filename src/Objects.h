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

		void print();
	};

	struct Finish {
		float ambient, diffuse;
	};

	class Object {
	public:
		Pigment pigment;
		Finish finish;

		virtual std::string type() = 0;
		virtual void print() = 0;
		virtual float getFirstCollision(struct Ray* ray) = 0;
	};

	class Camera {
	public:
		vec3 location, up, right, look_at;
		
		std::string type();
		void print();
	};

	class Light {
	public:
		vec3 location;
		Pigment pigment;

		std::string type();
		void print();
	};

	class Sphere : public Object {
	public:
		vec3 center;
		float radius;

		std::string type();
		void print();
		float getFirstCollision(Ray* ray);
	};

	class Plane : public Object {
	public:
		vec3 normal;
		float distance;

		std::string type();
		void print();
		float getFirstCollision(Ray* ray);
	};

	struct Ray {
		vec3 d, origin;
		int x, y, width, height;

		Ray();
		Ray(int x, int y, int width, int height, Camera* camera);
		void print();
	};
}

#endif