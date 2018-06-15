#ifndef GLOBALS_H
#define GLOBALS_H

namespace Globals
{
	static const int RENDER = 0;
	static const int PIXEL_COLOR = 1;
	static const int PIXEL_RAY = 2;
	static const int SCENE_INFO = 3;
	static const int PRINT_RAYS = 4;
	static const int SCENE_1 = 5;
	static const int SCENE_2 = 6;
	static const int SCENE_3 = 7;

	static const int FPS = 60;
	static const int SCENE_1_FRAMES = 300;
	static const int SCENE_2_FRAMES = 600;
	static const int SCENE_3_FRAMES = 200;

	static const int SCENE_3_LIGHTS = 20;

	static const int COLOR_RGB = 0;
	static const int COLOR_RGBF = 1;

	static const int MAX_BOUNCE = 6;

	static const int FIRST_BOUNCE_GI = 64;
	static const int SECOND_BOUNCE_GI = 16;

	static const float EPSILON = 0.001f;

	static const float BEERS_ALPHA = 0.15;

	static const float PI = 3.14159265359f;

	static std::string filename;
	static const std::string outfilename = "output.png";
}

#endif
