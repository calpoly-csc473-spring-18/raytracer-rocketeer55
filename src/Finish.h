#ifndef FINISH_H
#define FINISH_H

namespace Objects {
	struct Finish {
		float ambient, diffuse;

		Finish();
		Finish(float _ambient, float _diffuse);
	};
}

#endif