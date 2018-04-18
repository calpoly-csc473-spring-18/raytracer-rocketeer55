#include "Finish.h"

using namespace Objects;

Finish::Finish() {
	ambient = diffuse = 0.f;
}

Finish::Finish(float _ambient, float _diffuse) {
	ambient = _ambient;
	diffuse = _diffuse;
}