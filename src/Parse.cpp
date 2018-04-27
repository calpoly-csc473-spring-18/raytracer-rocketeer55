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
	v.x = v.y = v.z = v.w = 0.f;
	std::stringbuf buf;

	Stream.ignore(1, '<');
	Stream.get(buf, '>');
	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '>');

	std::string line = buf.str();
	int read = sscanf(line.c_str(), "%f, %f, %f, %f", &v.x, &v.y, &v.z, &v.w);

	if (read != 4) {
		std::cerr << "Expected to read 4 vector elements but found '" << line << "'" << std::endl;
	}

	return v;
}

Pigment Parse::load_pigment(std::stringstream & Stream)
{
	Pigment pigment;

	pigment.colortype = -1;
	std::string temp;

	Stream >> temp;

	if (temp.compare("color") != 0) {
		std::cerr << "Unexpected pigment type '" << temp << "'" << std::endl;
	}

	Stream >> temp;
	if (temp.compare("rgb") == 0) {
		// color is rgb

		pigment.colortype = Globals::COLOR_RGB;

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		pigment.color.rgb = Parse::Vector(Stream);
	}
	else if (temp.compare("rgbf") == 0) {
		// color is rgbf

		pigment.colortype = Globals::COLOR_RGBF;

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		pigment.color.rgbf = Parse::Vector4(Stream);
	}
	else {
		std::cerr << "Unexpected color type '" << temp << "'" << std::endl;
	}

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '}');
	return pigment;
}

Finish Parse::load_finish(std::stringstream & Stream)
{
	Finish finish;
	finish.ambient = finish.diffuse = finish.specular = finish.roughness = finish.reflection = finish.refraction = finish.metallic = finish.ior = 0.f;
	std::string temp;
	std::stringbuf buf;
	std::stringstream newstream;

	Stream.get(buf, '}');
	newstream << buf.str();
	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '}');

	while (newstream >> temp) {

		if (temp.compare("ambient") == 0) {
			// AMBIENT!

			newstream >> temp;
			int read = sscanf(temp.c_str(), "%f", &(finish.ambient));

			if (read != 1) {
				std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
			}
		}

		else if (temp.compare("diffuse") == 0) {
			// DIFFUSE!

			newstream >> temp;
			int read = sscanf(temp.c_str(), "%f", &(finish.diffuse));

			if (read != 1) {
				std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
			}
		}

		else if (temp.compare("specular") == 0) {
			// SPECULAR!

			newstream >> temp;
			int read = sscanf(temp.c_str(), "%f", &(finish.specular));

			if (read != 1) {
				std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
			}
		}

		else if (temp.compare("roughness") == 0) {
			// ROUGHNESS!

			newstream >> temp;
			int read = sscanf(temp.c_str(), "%f", &(finish.roughness));

			if (read != 1) {
				std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
			}
		}

		else if (temp.compare("reflection") == 0) {
			// REFLECTION!

			newstream >> temp;
			int read = sscanf(temp.c_str(), "%f", &(finish.reflection));

			if (read != 1) {
				std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
			}
		}

		else if (temp.compare("refraction") == 0) {
			// REFRACTION!

			newstream >> temp;
			int read = sscanf(temp.c_str(), "%f", &(finish.refraction));

			if (read != 1) {
				std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
			}
		}

		else if (temp.compare("metallic") == 0) {
			// METALLIC!

			newstream >> temp;
			int read = sscanf(temp.c_str(), "%f", &(finish.refraction));

			if (read != 1) {
				std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
			}
		}

		else if (temp.compare("ior") == 0) {
			// IOR!

			newstream >> temp;
			int read = sscanf(temp.c_str(), "%f", &(finish.ior));

			if (read != 1) {
				std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
			}
		}

		else {
			std::cerr << "Expected either 'ambient' or 'diffuse' but found '" << temp << "'" << std::endl;
		}
	}

	return finish;
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

	light->pigment = Parse::load_pigment(Stream);

	return light;
}

Sphere* Parse::load_sphere(std::stringstream & Stream)
{
	Sphere* sphere = new Sphere();
	std::string temp;
	std::stringbuf buf;

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	// Center!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	sphere->center = Parse::Vector(Stream);

	// Radius!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
	Stream.get(buf);

	temp = buf.str();
	int read = sscanf(temp.c_str(), "%f", &(sphere->radius));

	if (read != 1) {
		std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
	}

	Stream >> temp;
	while (temp.compare("}") != 0) {

		if (temp.compare("pigment") == 0) {
			// PIGMENT!

			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

			sphere->pigment = Parse::load_pigment(Stream);
		}
		else if (temp.compare("finish") == 0) {
			// FINISH!

			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

			sphere->finish = Parse::load_finish(Stream);
		}
		else if (temp.compare("translate") == 0) {
			// TRANSLATE!

			// Not yet implemented
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '>');
		}
		else if (temp.compare("scale") == 0) {
			// SCALE!

			// Not yet implemented
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '>');
		}
		else if (temp.compare("rotate") == 0) {
			// ROTATE!

			// Not yet implemented
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '>');
		}
		else {
			std::cerr << "Expected either 'pigment', 'finish', 'scale', 'rotate', or 'translate' but found '" << temp << "'" << std::endl;
		}

		Stream >> temp;
	}
	return sphere;
}

Plane* Parse::load_plane(std::stringstream &Stream)
{
	Plane* plane = new Plane();
	std::string temp;
	std::stringbuf buf;

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	// Normal!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	plane->normal = Parse::Vector(Stream);

	// Distance!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
	Stream.get(buf);

	temp = buf.str();
	int read = sscanf(temp.c_str(), "%f", &(plane->distance));

	if (read != 1) {
		std::cerr << "Expected to read 1 float element but found '" << temp << "'" << std::endl;
	}

	Stream >> temp;
	while (temp.compare("}") != 0) {

		if (temp.compare("pigment") == 0) {
			// PIGMENT!

			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

			plane->pigment = Parse::load_pigment(Stream);
		}
		else if (temp.compare("finish") == 0) {
			// FINISH!

			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

			plane->finish = Parse::load_finish(Stream);
		}
		else {
			std::cerr << "Expected either 'pigment' or 'finish' but found '" << temp << "'" << std::endl;
		}

		Stream >> temp;
	}
	return plane;
}
