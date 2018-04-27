#ifndef GLOBALS_H
#define GLOBALS_H

namespace Globals
{
	static const int RENDER = 0;
	static const int PIXEL_COLOR = 1;

	static const int COLOR_RGB = 0;
	static const int COLOR_RGBF = 1;

	static const float EPSILON = 0.0001f;

	static std::string filename;
	static const std::string outfilename = "output.png";
}

#endif
