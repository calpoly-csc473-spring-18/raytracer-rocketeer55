#include "Headers.h"

using namespace Globals;

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

	if (input.compare("raycast") == 0) {
		mode = RAYCAST;
	}
	else if (input.compare("sceneinfo") == 0) {
		mode = SCENE_INFO;
	}
	else if (input.compare("pixelray") == 0) {
		mode = PIXEL_RAY;
	}
	else if (input.compare("firsthit") == 0) {
		mode = FIRST_HIT;
	}
	else {
		return 1;
	}

	filename = argv[2];

	if (mode == RAYCAST) {
		if (argc != 5) {
			return 1;
		}
		width = atoi(argv[3]);
		height = atoi(argv[4]);
	}
	else if (mode == SCENE_INFO) {
		if (argc != 3) {
			return 1;
		}
	}
	else if (mode == PIXEL_RAY || mode == FIRST_HIT) {
		if (argc != 7) {
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

	std::cout << "Camera:" << std::endl;
	std::cout << "- Location: {" << camera->location.x << " " << camera->location.y << " " << camera->location.z << "}" << std::endl;
	std::cout << "- Up: {" << camera->up.x << " " << camera->up.y << " " << camera->up.z << "}" << std::endl;
	std::cout << "- Right: {" << camera->right.x << " " << camera->right.y << " " << camera->right.z << "}" << std::endl;
	std::cout << "- Look at: {" << camera->look_at.x << " " << camera->look_at.y << " " << camera->look_at.z << "}" << std::endl;

	// Print Light Sources

	std::cout << "\n---\n\n" << lights.size() << " light(s)" << std::endl;

	int i = 0;
	for each (Light* l in lights)
	{
		std::cout << "\nLight[" << i << "]:" << std::endl;
		std::cout << "- Location: {" << l->location.x << " " << l->location.y << " " << l->location.z << "}" << std::endl;
		if (l->pigment.colortype == COLOR_RGB) {
			std::cout << "- Color: {" << l->pigment.color.rgb.x << " " << l->pigment.color.rgb.y << " " << l->pigment.color.rgb.z << "}" << std::endl;
		}
		else {
			std::cout << "- Color: {" << l->pigment.color.rgbf.x << " " << l->pigment.color.rgbf.y << " " << l->pigment.color.rgbf.z << " " << l->pigment.color.rgbf.a << "}" << std::endl;
		}

		i++;
	}

	// Print Objects

	std::cout << "\n---\n\n" << objects.size() << " object(s)" << std::endl;

	i = 0;
	for each (Object *o in objects)
	{
		std::cout << "\nObject[" << i << "]:" << std::endl;
		if (o->type().compare("Sphere") == 0) {
			// is a sphere!
			Sphere* s = dynamic_cast<Sphere*>(o);
			std::cout << "- Type: Sphere" << std::endl;
			std::cout << "- Center: {" << s->center.x << " " << s->center.y << " " << s->center.z << "}" << std::endl;
			std::cout << "- Radius: " << s->radius << std::endl;
			if (s->pigment.colortype == COLOR_RGB) {
				std::cout << "- Color: {" << s->pigment.color.rgb.x << " " << s->pigment.color.rgb.y << " " << s->pigment.color.rgb.z << "}" << std::endl;
			}
			else {
				std::cout << "- Color: {" << s->pigment.color.rgbf.x << " " << s->pigment.color.rgbf.y << " " << s->pigment.color.rgbf.z << " " << s->pigment.color.rgbf.a << "}" << std::endl;
			}
			std::cout << "- Material: " << std::endl;
			std::cout << "  - Ambient: " << s->finish.ambient << std::endl;
			std::cout << "  - Diffuse: " << s->finish.diffuse << std::endl;
		}
		else if (o->type().compare("Plane") == 0) {
			// is a plane!

			Plane* p = dynamic_cast<Plane*>(o);
			std::cout << "- Type: Plane" << std::endl;
			std::cout << "- Normal: {" << p->normal.x << " " << p->normal.y << " " << p->normal.z << "}" << std::endl;
			std::cout << "- Distance: " << p->distance << std::endl;
			if (p->pigment.colortype == COLOR_RGB) {
				std::cout << "- Color: {" << p->pigment.color.rgb.x << " " << p->pigment.color.rgb.y << " " << p->pigment.color.rgb.z << "}" << std::endl;
			}
			else {
				std::cout << "- Color: {" << p->pigment.color.rgbf.x << " " << p->pigment.color.rgbf.y << " " << p->pigment.color.rgbf.z << " " << p->pigment.color.rgbf.a << "}" << std::endl;
			}
			std::cout << "- Material: " << std::endl;
			std::cout << "  - Ambient: " << p->finish.ambient << std::endl;
			std::cout << "  - Diffuse: " << p->finish.diffuse << std::endl;
		}

		i++;
	}
}

void printPixelRay() {
	Ray* ray = new Ray(x, y, width, height, camera);

	std::cout << "Pixel: [" << x << ", " << y << "] Ray: {";
	std::cout << std::setiosflags(std::ios::fixed);
	std::cout << std::setprecision(4);
	std::cout << ray->origin.x << " " << ray->origin.y << " " << ray->origin.z << "} -> {";
	std::cout << ray->d.x << " " << ray->d.y << " " << ray->d.z << "}" << std::endl;
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

	if (mode == SCENE_INFO) {
		printSceneInfo();
	}
	else if (mode == PIXEL_RAY) {
		printPixelRay();
	}

	return EXIT_SUCCESS;
}