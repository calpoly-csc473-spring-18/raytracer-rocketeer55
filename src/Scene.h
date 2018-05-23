#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Object.h"
#include "Light.h"
#include "Camera.h"
#include "Intersection.h"

using namespace Objects;
struct Scene {
	Camera* camera;
	std::vector<Light*> lights;
	std::vector<Object*> objects;

	int width, height, s;
	bool ss, fresnel, beers;


	Scene();

	~Scene();

	void printSceneInfo();
	void renderScene();
	void printPixelColor(int x, int y);
	Intersection* getFirstIntersection(Ray* ray);
	bool isInShadow(Intersection* intersection, Light* light);
};

#endif