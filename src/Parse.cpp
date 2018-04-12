#include "Parse.h"
#include "Globals.h"
#include <limits>
#include <iostream>
#include <sstream>

vec3 Parse::Vector(std::stringstream & Stream)
{
	vec3 v;
v.x = v.y = v.z = 0.f;
std::stringbuf buf;

Stream.ignore(1, '<');
Stream.get(buf, '>');
Stream.ignore(std::numeric_limits<std::streamsize>::max(), '>');

std::string line = buf.str();
int read = sscanf(line.c_str(), "%f, %f, %f", &v.x, &v.y, &v.z);

if (read != 3) {
	std::cerr << "Expected to read 3 vector elements but found '" << line << "'" << std::endl;
}

return v;
}

vec4 Parse::Vector4(std::stringstream & Stream)
{
	vec4 v;
	v.x = v.y = v.z = v.a = 0.f;
	std::stringbuf buf;

	Stream.ignore(1, '<');
	Stream.get(buf, '>');
	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '>');

	std::string line = buf.str();
	int read = sscanf(line.c_str(), "%f, %f, %f, %f", &v.x, &v.y, &v.z, &v.a);

	if (read != 4) {
		std::cerr << "Expected to read 4 vector elements but found '" << line << "'" << std::endl;
	}

	return v;
}

Camera* Parse::load_cam(std::stringstream & Stream)
{
	Camera* camera = new Camera();
	std::string temp;

	// is a camera!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	Stream >> temp;
	// loading location!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	camera->location = Parse::Vector(Stream);

	Stream >> temp;

	// up!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	camera->up = Parse::Vector(Stream);

	Stream >> temp;
	// right!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	camera->right = Parse::Vector(Stream);

	Stream >> temp;
	// look_at!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	camera->look_at = Parse::Vector(Stream);

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '}');

	return camera;
}

Light* Parse::load_light(std::stringstream &Stream)
{
	Light* light = new Light();
	std::string temp;

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	light->location = Parse::Vector(Stream);

	Stream >> temp;
	Stream >> temp;
	if (temp.compare("rgb") == 0) {
		// color is rgb
		
		light->colortype = Globals::COLOR_RGB;

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		light->color.rgb = Parse::Vector(Stream);
	}
	else if (temp.compare("rgbf") == 0) {
		// color is rgbf

		light->colortype = Globals::COLOR_RGBF;

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		light->color.rgbf = Parse::Vector4(Stream);
	}
	else {
		std::cerr << "Unexpected color type '" << temp << "'" << std::endl;
	}

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '}');
	return light;
}

Sphere* Parse::load_sphere(std::stringstream & Stream)
{
	Sphere* sphere = new Sphere();
	std::string temp;
	std::stringbuf buf;

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	sphere->center = Parse::Vector(Stream);

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
	Stream.get(buf);

	temp = buf.str();
	int read = sscanf(temp.c_str(), "%f", &(sphere->radius));

	if (read != 1) {
		std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
	}

	Stream >> temp;
	// PIGMENT!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	Stream >> temp;
	Stream >> temp;
	if (temp.compare("rgb") == 0) {
		// color is rgb

		sphere->colortype = Globals::COLOR_RGB;

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		sphere->color.rgb = Parse::Vector(Stream);
	}
	else if (temp.compare("rgbf") == 0) {
		// color is rgbf

		sphere->colortype = Globals::COLOR_RGBF;

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		sphere->color.rgbf = Parse::Vector4(Stream);
	}
	else {
		std::cerr << "Unexpected color type '" << temp << "'" << std::endl;
	}

	Stream >> temp;
	// FINISH! (Not yet implemented)

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '}');

	return sphere;
}

Plane* Parse::load_plane(std::stringstream &Stream)
{
	Plane* plane = new Plane();
	std::string temp;
	std::stringbuf buf;

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	plane->normal = Parse::Vector(Stream);

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
	Stream.get(buf);

	temp = buf.str();
	int read = sscanf(temp.c_str(), "%f", &(plane->distance));

	if (read != 1) {
		std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
	}

	Stream >> temp;
	// PIGMENT!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	Stream >> temp;
	Stream >> temp;
	if (temp.compare("rgb") == 0) {
		// color is rgb

		plane->colortype = Globals::COLOR_RGB;

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		plane->color.rgb = Parse::Vector(Stream);
	}
	else if (temp.compare("rgbf") == 0) {
		// color is rgbf

		plane->colortype = Globals::COLOR_RGBF;

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		plane->color.rgbf = Parse::Vector4(Stream);
	}
	else {
		std::cerr << "Unexpected color type '" << temp << "'" << std::endl;
	}

	Stream >> temp;
	// FINISH! (Not yet implemented)

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '}');

	return plane;
}
