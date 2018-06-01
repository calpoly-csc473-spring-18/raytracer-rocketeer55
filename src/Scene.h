#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Object.h"
#include "Light.h"
#include "Camera.h"
#include "Intersection.h"
#include "BVH.h"

using namespace Objects;
struct Scene {
	Camera* camera;
	std::vector<Light*> lights;
	std::vector<Object*> objects;

	BVH_Node* root;
	std::vector<Object*> planes;
	std::vector<Object*> objects_without_planes;

	int width, height, s;
	bool ss, fresnel, beers, sds;

	Scene();
	~Scene();

	void printSceneInfo();
	void renderScene();
	void printPixelColor(int x, int y);
	Intersection* getFirstIntersection(Ray* ray);
	bool isInShadow(Intersection* intersection, Light* light);

private:
	Intersection* getFirstIntersectionSDS(BVH_Node* node, Ray* ray);
	void initSDS();
};

#endif