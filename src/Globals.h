#ifndef GLOBALS_H
#define GLOBALS_H

namespace Globals
{
	static const int RENDER = 0;
	static const int PIXEL_COLOR = 1;
	static const int PIXEL_RAY = 2;
	static const int SCENE_INFO = 3;
	static const int PRINT_RAYS = 4;

	static const int COLOR_RGB = 0;
	static const int COLOR_RGBF = 1;

	static const int MAX_BOUNCE = 6;

	static const float EPSILON = 0.001f;

	static std::string filename;
	static const std::string outfilename = "output.png";
}

#endif
