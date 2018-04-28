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

			if (nearest == NULL) {
				// Ray doesn't collide with an object
				image->setPixel(i, j, 0, 0, 0);
				color = vec3();
			}

			else {
				vec3 pos = vec3(ray->origin.x + nearest_t * ray->d.x, ray->origin.y + nearest_t * ray->d.y, ray->origin.z + nearest_t * ray->d.z);

				color = vec3();

				color.x += nearest->finish.ambient * nearest->pigment.color.rgb.x;
				color.y += nearest->finish.ambient * nearest->pigment.color.rgb.y;
				color.z += nearest->finish.ambient * nearest->pigment.color.rgb.z;

				// Check if in shadow
				for (unsigned int k = 0; k < lights.size(); k++) {
					Ray *light = new Ray(pos, lights[k]->location);

					// adjust origin of light to avoid SHADOW ACNE!
					light->origin.x = pos.x + light->d.x * EPSILON;
					light->origin.y = pos.y + light->d.y * EPSILON;
					light->origin.z = pos.z + light->d.z * EPSILON;

					Object* shadow_nearest = NULL;
					float shadow_t = 0, shadow_nearest_t = vec3::getDistance(light->origin, lights[k]->location);

					for (unsigned int shadow_i = 0; shadow_i < objects.size(); shadow_i++) {
						Object* o = objects[shadow_i];
						shadow_t = o->getFirstCollision(light);
						if (shadow_t != -1 && (shadow_t < shadow_nearest_t)) {
							shadow_nearest_t = shadow_t;
							shadow_nearest = o;
						}
					}

					if (shadow_nearest == NULL) {
						// No shadow here! Calculate BLINN_PHONG lighting

						if (nearest->pigment.colortype == COLOR_RGBF) {
							std::cerr << "Color type RGBF - Not yet implemented" << std::endl;
						}
						
						vec3 Kd = vec3(nearest->finish.diffuse * nearest->pigment.color.rgb.x, nearest->finish.diffuse * nearest->pigment.color.rgb.y, nearest->finish.diffuse * nearest->pigment.color.rgb.z);
						vec3 Ks = vec3(nearest->finish.specular * nearest->pigment.color.rgb.x, nearest->finish.specular * nearest->pigment.color.rgb.y, nearest->finish.specular * nearest->pigment.color.rgb.z);

						if (nearest->finish.diffuse > 0.f) {
							// Calculate diffuse!
							glm::vec3 g_Kd, g_N, g_L, g_Lc, D;

							g_Kd = glm::vec3(Kd.x, Kd.y, Kd.z);

							vec3 norm = nearest->getNormal(pos);
							g_N = glm::vec3(norm.x, norm.y, norm.z);

							g_L = glm::vec3(light->d.x, light->d.y, light->d.z);

							g_Lc = glm::vec3(lights[k]->pigment.color.rgb.x, lights[k]->pigment.color.rgb.y, lights[k]->pigment.color.rgb.z);

							D = g_Kd * glm::dot(g_N, g_L) * g_Lc;

							float *data = glm::value_ptr(D);
							color.x += data[0];
							color.y += data[1];
							color.z += data[2];
						}

						if (nearest->finish.specular > 0.f) {
							// Calculate specular!
							glm::vec3 g_Ks, g_N, g_L, g_pos, g_cam, g_V, g_H, g_Lc, S;

							g_Ks = glm::vec3(Ks.x, Ks.y, Ks.z);

							vec3 norm = nearest->getNormal(pos);
							g_N = glm::vec3(norm.x, norm.y, norm.z);

							g_L = glm::vec3(light->d.x, light->d.y, light->d.z);

							g_pos = glm::vec3(pos.x, pos.y, pos.z);
							g_cam = glm::vec3(camera->location.x, camera->location.y, camera->location.z);

							g_V = glm::normalize(g_cam - g_pos);

							g_H = glm::normalize(g_V + g_L);

							g_Lc = glm::vec3(lights[k]->pigment.color.rgb.x, lights[k]->pigment.color.rgb.y, lights[k]->pigment.color.rgb.z);

							S = g_Ks * pow(glm::dot(g_H, g_N), pow(nearest->finish.roughness, 2)) * g_Lc;

							float *data = glm::value_ptr(S);
							color.x += data[0];
							color.y += data[1];
							color.z += data[2];
						} 
					}
				}
			}
			if (color.x > 1) {
				color.x = 1;
			}
			if (color.y > 1) {
				color.y = 1;
			}
			if (color.z > 1) {
				color.z = 1;
			}

			unsigned int red = (unsigned int)std::round(color.x * 255.f);
			unsigned int green = (unsigned int)std::round(color.y * 255.f);
			unsigned int blue = (unsigned int)std::round(color.z * 255.f);

			image->setPixel(i, j, red, green, blue);
		}
	}

	image->writeToFile(outfilename);
}

void printPixelColor() {

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
	else {
		printPixelColor();
	}

	return EXIT_SUCCESS;
}