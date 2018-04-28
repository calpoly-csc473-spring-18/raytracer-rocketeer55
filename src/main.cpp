#include "Headers.h"

using namespace Globals;
using namespace Objects;

std::stringstream buffer;
Camera* camera;

std::vector<Light*> lights;
std::vector<Object*> objects;

int x, y, width, height, mode;

int parseArgs(int &argc, char *argv[]) {
	if (argc < 3) {
		return 1;
	}

	std::string input = argv[1];

	if (input.compare("render") == 0) {
		mode = RENDER;
	}
	else if (input.compare("pixelcolor") == 0) {
		mode = PIXEL_COLOR;
	}
	else if (input.compare("pixelray") == 0) {
		mode = PIXEL_RAY;
	}
	else {
		return 1;
	}

	filename = argv[2];

	if (mode == RENDER) {
		if (argc < 5) {
			return 1;
		}
		width = atoi(argv[3]);
		height = atoi(argv[4]);
	}
	else if (mode == PIXEL_COLOR) {
		if (argc < 7) {
			return 1;
		}
		width = atoi(argv[3]);
		height = atoi(argv[4]);
		x = atoi(argv[5]);
		y = atoi(argv[6]);
	}
		
	return 0;
}

int openFile() {
	std::ifstream infile(filename);

	if (infile) {
		buffer << infile.rdbuf();
		infile.close();
	}
	else {
		return 1;
	}

	return 0;
}

int parseFile() {
	if (!buffer) {
		return 1;
	}

	std::string temp;
	while (buffer >> temp) {
		if (temp.compare("//") == 0) {
			// is a comment

			buffer.ignore(std::numeric_limits<std::streamsize>::max(), buffer.widen('\n'));
		}
		else if (temp.compare("camera") == 0) {
			// is a camera!

			camera = Parse::load_cam(buffer);
		}
		else if (temp.compare("light_source") == 0) {
			// is a light source!

			lights.push_back(Parse::load_light(buffer));
		}
		else if (temp.compare("sphere") == 0) {
			// is a sphere!

			objects.push_back(Parse::load_sphere(buffer));
		}
		else if (temp.compare("plane") == 0) {
			// is a plane!

			objects.push_back(Parse::load_plane(buffer));
		}
	}

	return 0;
}

void printSceneInfo()
{
	// Setup cout

	std::cout << std::setiosflags(std::ios::fixed);
	std::cout << std::setprecision(4);

	// Print Camera

	camera->print();

	// Print Light Sources

	std::cout << "\n---\n\n" << lights.size() << " light(s)" << std::endl;

	for (unsigned int i = 0; i < lights.size(); i++) {
		Light* l = lights[i];
		std::cout << "\nLight[" << i << "]:" << std::endl;
		
		l->print();
	}

	// Print Objects

	std::cout << "\n---\n\n" << objects.size() << " object(s)" << std::endl;

	for (unsigned int i = 0; i < objects.size(); i++) {
		Object* o = objects[i];

		std::cout << "\nObject[" << i << "]:" << std::endl;
		
		o->print();
	}
}

void printPixelRay() {
	Ray* ray = new Ray(x, y, width, height, camera);

	ray->print();
}

void printFirstHit() {
	Ray* ray = new Ray(x, y, width, height, camera);
	Object* nearest;
	float t, nearest_t = -1;

	ray->print();

	for (unsigned int i = 0; i < objects.size(); i++) {
		Object* o = objects[i];
		t = o->getFirstCollision(ray);
		if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
			nearest_t = t;
			nearest = o;
		}
	}

	if (nearest_t == -1) {
		std::cout << "No Hit" << std::endl;
	}
	else {
		std::cout << "T = " << nearest_t << std::endl;
		std::cout << "Object Type: " << nearest->type() << std::endl;
		std::cout << "Color: ";
		nearest->pigment.print();
	}
}

void raycast() {
	Ray* ray;
	Image* image = new Image(width, height);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			ray = new Ray(i, j, width, height, camera);

			Object* nearest = NULL;
			float t = 0, nearest_t = -1;

			for (unsigned int i = 0; i < objects.size(); i++) {
				Object* o = objects[i];
				t = o->getFirstCollision(ray);
				if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
					nearest_t = t;
					nearest = o;
				}
			}

			if (nearest_t == -1) {
				image->setPixel(i, j, 0, 0, 0);
			}
			else {
				unsigned int red = (unsigned int)std::round(nearest->pigment.color.rgb.x * 255.f);
				unsigned int green = (unsigned int)std::round(nearest->pigment.color.rgb.y * 255.f);
				unsigned int blue = (unsigned int)std::round(nearest->pigment.color.rgb.z * 255.f);

				image->setPixel(i, j, red, green, blue);
			}
		}
	}

	image->writeToFile(outfilename);
}

void renderScene() {
	Ray *ray;
	Image* image = new Image(width, height);
	vec3 color;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			ray = new Ray(i, j, width, height, camera);
			color = vec3();

			Object* nearest = NULL;
			float t = 0, nearest_t = -1;

			for (unsigned int k = 0; k < objects.size(); k++) {
				Object* o = objects[k];
				t = o->getFirstCollision(ray);

				if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
					nearest_t = t;
					nearest = o;
				}
			}

			if (nearest_t != -1) {
				// Hits an Object - Calculate color!
				Object* object = nearest;

				// First add base-ambient color
				color.x = object->finish.ambient * object->pigment.color.rgb.x;
				color.y = object->finish.ambient * object->pigment.color.rgb.y;
				color.z = object->finish.ambient * object->pigment.color.rgb.z;

				for (unsigned int l = 0; l < lights.size(); l++) {
					Light* light = lights[l];

					// INSTANTIATE COLOR VARIABLES

					// Calculate position of intersection
					glm::vec3 o = glm::vec3(ray->origin.x, ray->origin.y, ray->origin.z);
					glm::vec3 d = glm::vec3(ray->d.x, ray->d.y, ray->d.z);
					glm::vec3 pos = o + nearest_t * d;

					// Calculate normal on object at position
					vec3 position = vec3();
					float *data = glm::value_ptr(pos);
					position.x = data[0];
					position.y = data[1];
					position.z = data[2];

					vec3 norm = object->getNormal(position);
					glm::vec3 N = glm::vec3(norm.x, norm.y, norm.z);

					// Calculate light vector at position
					glm::vec3 light_pos = glm::vec3(light->location.x, light->location.y, light->location.z);
					glm::vec3 L = glm::normalize(light_pos - pos);

					// Calculate view vector at position
					glm::vec3 V = glm::vec3(-ray->d.x, -ray->d.y, -ray->d.z);

					// Calculate half vector at position
					glm::vec3 H = glm::normalize(V + L);

					// Calculate alpha
					float alpha = 2 / (object->finish.roughness * object->finish.roughness) - 2;

					// Calculate N dot L
					float NdotL = std::max(glm::dot(N, L), 0.f);

					// Calculate H dot N
					float HdotN = std::max(glm::dot(H, N), 0.f);

					// CHECK IF IN SHADOW
					Ray* shadow_ray = new Ray(position, light->location);
					glm::vec3 shadow_origin = pos + L * EPSILON;

					data = glm::value_ptr(shadow_origin);
					shadow_ray->origin.x = data[0];
					shadow_ray->origin.y = data[1];
					shadow_ray->origin.z = data[2];

					Object* shadow_nearest = NULL;
					float shadow_t = 0, shadow_nearest_t = vec3::getDistance(shadow_ray->origin, light->location);
					for (unsigned int shadow_i = 0; shadow_i < objects.size(); shadow_i++) {
						Object* o = objects[shadow_i];
						shadow_t = o->getFirstCollision(shadow_ray);
						if (shadow_t != -1 && shadow_t < shadow_nearest_t) {
							shadow_nearest_t = shadow_t;
							shadow_nearest = o;
						}
					}

					if (shadow_nearest == NULL) {
						// Now calculate and add diffuse
						glm::vec3 Kd = glm::vec3(object->finish.diffuse * object->pigment.color.rgb.x,
							object->finish.diffuse * object->pigment.color.rgb.y,
							object->finish.diffuse * object->pigment.color.rgb.z);

						color.x += Kd.x * NdotL * light->pigment.color.rgb.x;
						color.y += Kd.y * NdotL * light->pigment.color.rgb.y;
						color.z += Kd.z * NdotL * light->pigment.color.rgb.z;

						// Now calculate and add specular
						glm::vec3 Ks = glm::vec3(object->finish.specular * object->pigment.color.rgb.x,
							object->finish.specular * object->pigment.color.rgb.y,
							object->finish.specular * object->pigment.color.rgb.z);

						color.x += Ks.x * powf(HdotN, alpha) * light->pigment.color.rgb.x;
						color.y += Ks.y * powf(HdotN, alpha) * light->pigment.color.rgb.y;
						color.z += Ks.z * powf(HdotN, alpha) * light->pigment.color.rgb.z;
					}
				}

				// Clamp colors between 0 and 1
				if (color.x > 1) {
					color.x = 1;
				}
				if (color.x < 0) {
					color.x = 0;
				}
				if (color.y > 1) {
					color.y = 1;
				}
				if (color.y < 0) {
					color.y = 0;
				}
				if (color.z > 1) {
					color.z = 1;
				}
				if (color.z < 0) {
					color.z = 0;
				}

				// Change color to RGB
				unsigned int red = (unsigned int)std::round(color.x * 255.f);
				unsigned int green = (unsigned int)std::round(color.y * 255.f);
				unsigned int blue = (unsigned int)std::round(color.z * 255.f);

				image->setPixel(i, j, red, green, blue);
			}
			else {
				// No object

				image->setPixel(i, j, 0, 0, 0);
			}
		}
	}

	image->writeToFile(outfilename);
}

void printPixelColor() {
	Ray* ray = new Ray(x, y, width, height, camera);
	vec3 color = vec3();

	Object* nearest = NULL;
	float t = 0, nearest_t = -1;

	for (unsigned int i = 0; i < objects.size(); i++) {
		Object* o = objects[i];
		t = o->getFirstCollision(ray);

		if (t != -1 && (nearest_t == -1 || t < nearest_t)) {
			nearest_t = t;
			nearest = o;
		}
	}

	if (nearest_t != -1) {
		// Hits an Object - Calculate color!
		Object* object = nearest;

		// First add base-ambient color
		color.x = object->finish.ambient * object->pigment.color.rgb.x;
		color.y = object->finish.ambient * object->pigment.color.rgb.y;
		color.z = object->finish.ambient * object->pigment.color.rgb.z;
		
		for (unsigned int l = 0; l < lights.size(); l++) {
			Light* light = lights[l];

			// INSTANTIATE COLOR VARIABLES

			// Calculate position of intersection
			glm::vec3 o = glm::vec3(ray->origin.x, ray->origin.y, ray->origin.z);
			glm::vec3 d = glm::vec3(ray->d.x, ray->d.y, ray->d.z);
			glm::vec3 pos = o + nearest_t * d;

			// Calculate normal on object at position
			vec3 position = vec3();
			float *data = glm::value_ptr(pos);
			position.x = data[0];
			position.y = data[1];
			position.z = data[2];

			vec3 norm = object->getNormal(position);
			glm::vec3 N = glm::vec3(norm.x, norm.y, norm.z);

			// Calculate light vector at position
			glm::vec3 light_pos = glm::vec3(light->location.x, light->location.y, light->location.z);
			glm::vec3 L = glm::normalize(light_pos - pos);

			// Calculate view vector at position
			glm::vec3 V = glm::vec3(-ray->d.x, -ray->d.y, -ray->d.z);

			// Calculate half vector at position
			glm::vec3 H = glm::normalize(V + L);

			// Calculate alpha
			float alpha = object->finish.roughness * object->finish.roughness;

			// Calculate N dot L
			float NdotL = std::max(glm::dot(N, L), 0.f);

			// Calculate H dot N
			float HdotN = std::max(glm::dot(H, N), 0.f);

			// CHECK IF IN SHADOW
			Ray* shadow_ray = new Ray(position, light->location);
			glm::vec3 shadow_origin = pos + L * EPSILON;

			data = glm::value_ptr(shadow_origin);
			shadow_ray->origin.x = data[0];
			shadow_ray->origin.y = data[1];
			shadow_ray->origin.z = data[2];

			Object* shadow_nearest = NULL;
			float shadow_t = 0, shadow_nearest_t = vec3::getDistance(shadow_ray->origin, light->location);
			for (unsigned int shadow_i = 0; shadow_i < objects.size(); shadow_i++) {
				Object* o = objects[shadow_i];
				shadow_t = o->getFirstCollision(shadow_ray);
				if (shadow_t != -1 && shadow_t < shadow_nearest_t) {
					shadow_nearest_t = shadow_t;
					shadow_nearest = o;
				}
			}

			if (shadow_nearest == NULL) {
				// Now calculate and add diffuse
				glm::vec3 Kd = glm::vec3(object->finish.diffuse * object->pigment.color.rgb.x,
					object->finish.diffuse * object->pigment.color.rgb.y,
					object->finish.diffuse * object->pigment.color.rgb.z);

				color.x += Kd.x * NdotL * light->pigment.color.rgb.x;
				color.y += Kd.y * NdotL * light->pigment.color.rgb.y;
				color.z += Kd.z * NdotL * light->pigment.color.rgb.z;

				// Now calculate and add specular
				glm::vec3 Ks = glm::vec3(object->finish.specular * object->pigment.color.rgb.x,
					object->finish.specular * object->pigment.color.rgb.y,
					object->finish.specular * object->pigment.color.rgb.z);

				color.x += Ks.x * powf(HdotN, alpha) * light->pigment.color.rgb.x;
				color.y += Ks.y * powf(HdotN, alpha) * light->pigment.color.rgb.y;
				color.z += Ks.z * powf(HdotN, alpha) * light->pigment.color.rgb.z;
			}
		}

		// Clamp colors between 0 and 1
		if (color.x > 1) {
			color.x = 1;
		}
		if (color.x < 0) {
			color.x = 0;
		}
		if (color.y > 1) {
			color.y = 1;
		}
		if (color.y < 0) {
			color.y = 0;
		}
		if (color.z > 1) {
			color.z = 1;
		}
		if (color.z < 0) {
			color.z = 0;
		}

		// Change color to RGB
		unsigned int red = (unsigned int)std::round(color.x * 255.f);
		unsigned int green = (unsigned int)std::round(color.y * 255.f);
		unsigned int blue = (unsigned int)std::round(color.z * 255.f);

		ray->print();
		std::cout << "T = " << nearest_t << std::endl;
		std::cout << "Object Type: " << nearest->type() << std::endl;
		std::cout << "BRDF: Blinn-Phong" << std::endl;
		std::cout << "Color: (" << red << ", " << green << ", " << blue << ")" << std::endl;
	}
	else {
		// No object

		ray->print();
		std::cout << "No Hit" << std::endl;
	}
}


int main(int argc, char * argv[]) {
	if (parseArgs(argc, argv)) {
		std::cerr << "Incorrect argument usage - see README" << std::endl;
	}

	if (openFile()) {
		std::cerr << "File \"" << filename << "\" could not be read" << std::endl;
		return EXIT_FAILURE;
	}

	parseFile();

	if (mode == RENDER) {
		renderScene();
	}
	else if (mode == PIXEL_COLOR) {
		printPixelColor();
	}
	else {
		printPixelRay();
	}

	return EXIT_SUCCESS;
}