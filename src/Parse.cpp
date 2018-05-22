#include "Parse.h"
#include "Globals.h"
#include <limits>
#include <iostream>
#include <sstream>
#include "glm/gtc/matrix_transform.hpp"

glm::vec3 Parse::Vector(std::stringstream & Stream) {
	glm::vec3 v = glm::vec3(0.f);
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

glm::vec4 Parse::Vector4(std::stringstream & Stream) {
	glm::vec4 v = glm::vec4(0.f);
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

Pigment Parse::load_pigment(std::stringstream & Stream) {
	Pigment pigment;
	std::string temp;

	Stream >> temp;

	if (temp.compare("color") != 0) {
		std::cerr << "Unexpected pigment type '" << temp << "'" << std::endl;
	}

	Stream >> temp;
	if (temp.compare("rgb") == 0) {
		// color is rgb
		glm::vec3 color = glm::vec3(0.f);

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();
		
		color = Parse::Vector(Stream);
		pigment = Pigment(color.x, color.y, color.z);
	}
	else if (temp.compare("rgbf") == 0) {
		// color is rgbf
		glm::vec4 color = glm::vec4(0.f);

		Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
		Stream.unget();

		color = Parse::Vector4(Stream);
		pigment = Pigment(color.x, color.y, color.z, color.w);
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

	glm::mat4 ModelMatrix = glm::mat4(1.f);

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
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 translate = Parse::Vector(Stream);

			ModelMatrix = glm::translate(glm::mat4(1.f), translate) * ModelMatrix;
		}
		else if (temp.compare("scale") == 0) {
			// SCALE!
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 scale = Parse::Vector(Stream);

			ModelMatrix = glm::scale(glm::mat4(1.f), scale) * ModelMatrix;
		}
		else if (temp.compare("rotate") == 0) {
			// ROTATE!
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 rotate = Parse::Vector(Stream);

			glm::mat4 Rotate = glm::mat4(1.f);
			if (rotate.z != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.z), glm::vec3(0, 0, 1)) * Rotate;
			}
			if (rotate.y != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.y), glm::vec3(0, 1, 0)) * Rotate;
			}
			if (rotate.x != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.x), glm::vec3(1, 0, 0)) * Rotate;
			}

			ModelMatrix = Rotate * ModelMatrix;
		}
		else {
			std::cerr << "Expected either 'pigment', 'finish', 'scale', 'rotate', or 'translate' but found '" << temp << "'" << std::endl;
		}

		Stream >> temp;
	}

	sphere->InverseMatrix = glm::inverse(ModelMatrix);

	return sphere;
}

Plane* Parse::load_plane(std::stringstream &Stream)
{
	Plane* plane = new Plane();
	std::string temp;
	std::stringbuf buf;

	glm::mat4 ModelMatrix = glm::mat4(1.f);

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
		else if (temp.compare("translate") == 0) {
			// TRANSLATE!
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 translate = Parse::Vector(Stream);

			ModelMatrix = glm::translate(glm::mat4(1.f), translate) * ModelMatrix;
		}
		else if (temp.compare("scale") == 0) {
			// SCALE!
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 scale = Parse::Vector(Stream);

			ModelMatrix = glm::scale(glm::mat4(1.f), scale) * ModelMatrix;
		}
		else if (temp.compare("rotate") == 0) {
			// ROTATE!
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 rotate = Parse::Vector(Stream);

			glm::mat4 Rotate = glm::mat4(1.f);
			if (rotate.z != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.z), glm::vec3(0, 0, 1)) * Rotate;
			}
			if (rotate.y != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.y), glm::vec3(0, 1, 0)) * Rotate;
			}
			if (rotate.x != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.x), glm::vec3(1, 0, 0)) * Rotate;
			}

			ModelMatrix = Rotate * ModelMatrix;
		}
		else {
			std::cerr << "Expected either 'pigment', 'finish', 'scale', 'rotate', or 'translate' but found '" << temp << "'" << std::endl;
		}

		Stream >> temp;
	}

	plane->InverseMatrix = glm::inverse(ModelMatrix);

	return plane;
}

Triangle* Parse::load_triangle(std::stringstream &Stream) {
	Triangle* triangle = new Triangle();
	std::string temp;

	glm::mat4 ModelMatrix = glm::mat4(1.f);

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

	// Point A!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	triangle->a = Parse::Vector(Stream);

	// Point B!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	triangle->b = Parse::Vector(Stream);

	// Point C!

	Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
	Stream.unget();

	triangle->c = Parse::Vector(Stream);

	Stream >> temp;
	while (temp.compare("}") != 0) {
		if (temp.compare("pigment") == 0) {
			// PIGMENT!

			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

			triangle->pigment = Parse::load_pigment(Stream);
		}
		else if (temp.compare("finish") == 0) {
			// FINISH!

			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '{');

			triangle->finish = Parse::load_finish(Stream);
		}
		else if (temp.compare("translate") == 0) {
			// TRANSLATE!
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 translate = Parse::Vector(Stream);

			ModelMatrix = glm::translate(glm::mat4(1.f), translate) * ModelMatrix;
		}
		else if (temp.compare("scale") == 0) {
			// SCALE!
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 scale = Parse::Vector(Stream);

			ModelMatrix = glm::scale(glm::mat4(1.f), scale) * ModelMatrix;
		}
		else if (temp.compare("rotate") == 0) {
			// ROTATE!
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '<');
			Stream.unget();

			glm::vec3 rotate = Parse::Vector(Stream);

			glm::mat4 Rotate = glm::mat4(1.f);
			if (rotate.z != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.z), glm::vec3(0, 0, 1)) * Rotate;
			}
			if (rotate.y != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.y), glm::vec3(0, 1, 0)) * Rotate;
			}
			if (rotate.x != 0) {
				Rotate = glm::rotate(glm::mat4(1.f), glm::radians(rotate.x), glm::vec3(1, 0, 0)) * Rotate;
			}

			ModelMatrix = Rotate * ModelMatrix;
		}
		else {
			std::cerr << "Expected either 'pigment', 'finish', 'scale', 'rotate', or 'translate' but found '" << temp << "'" << std::endl;
		}

		Stream >> temp;
	}

	triangle->InverseMatrix = glm::inverse(ModelMatrix);

	return triangle;
}