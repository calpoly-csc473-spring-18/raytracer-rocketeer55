#ifndef FINISH_H
#define FINISH_H

namespace Objects {
	struct Finish {
		float ambient, diffuse, specular, roughness, reflection, refraction, metallic, ior;

		Finish();
	};
}

#endif