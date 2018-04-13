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
	else if (mode == FIRST_HIT) {
		printFirstHit();
	}

	return EXIT_SUCCESS;
}