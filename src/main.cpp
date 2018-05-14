#include "Headers.h"

using namespace Globals;
using namespace Objects;

std::stringstream buffer;
Scene* scene;

int x, y, mode;

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
	else if (input.compare("sceneinfo") == 0) {
		mode = SCENE_INFO;
	}
	else if (input.compare("printrays") == 0) {
		mode = PRINT_RAYS;
	}
	else {
		return 1;
	}

	filename = argv[2];

	if (mode == RENDER) {
		if (argc < 5) {
			return 1;
		}
		scene->width = atoi(argv[3]);
		scene->height = atoi(argv[4]);
	}
	else if (mode == PIXEL_RAY || mode == PRINT_RAYS) {
		if (argc < 7) {
			return 1;
		}
		scene->width = atoi(argv[3]);
		scene->height = atoi(argv[4]);
		x = atoi(argv[5]);
		y = atoi(argv[6]);
	}
	else if (mode == SCENE_INFO) {
		if (argc < 3) {
			return 1;
		}
	}
	else {
		if (argc < 7) {
			return 1;
		}
		scene->width = atoi(argv[3]);
		scene->height = atoi(argv[4]);
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
		if (temp.size() > 4 && temp.substr(0, 2).compare("//") == 0) {
			// is a comment

			buffer.ignore(std::numeric_limits<std::streamsize>::max(), buffer.widen('\n'));
		}
		else if (temp.compare("camera") == 0) {
			// is a camera!

			scene->camera = Parse::load_cam(buffer);
		}
		else if (temp.compare("light_source") == 0) {
			// is a light source!

			scene->lights.push_back(Parse::load_light(buffer));
		}
		else if (temp.compare("sphere") == 0) {
			// is a sphere!

			scene->objects.push_back(Parse::load_sphere(buffer));
		}
		else if (temp.compare("plane") == 0) {
			// is a plane!

			scene->objects.push_back(Parse::load_plane(buffer));
		}
		else if (temp.compare("triangle") == 0) {
			// is a triangle!

			scene->objects.push_back(Parse::load_triangle(buffer));
		}
	}

	return 0;
}

int main(int argc, char * argv[]) {
	scene = new Scene();

	if (parseArgs(argc, argv)) {
		std::cerr << "Incorrect argument usage - see README" << std::endl;
	}

	if (openFile()) {
		std::cerr << "File \"" << filename << "\" could not be read" << std::endl;
		return EXIT_FAILURE;
	}

	parseFile();

	if (mode == RENDER) {
		scene->renderScene();
	}
	else if (mode == PIXEL_COLOR || mode == PRINT_RAYS) {
		scene->printPixelColor(x, y);
	}
	else if (mode == SCENE_INFO) {
		scene->printSceneInfo();
	}
	else {
		Ray* ray = new Ray(x, y, scene->width, scene->height, scene->camera);
		ray->print();
	}
	delete(scene);

	return EXIT_SUCCESS;
}