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

	int width, height, s, thread;
	bool ss, fresnel, beers, sds, gi;
	bool progress;

	Scene();
	Scene(const Scene &other);
	~Scene();

	void printSceneInfo();
	void renderScene();
	void renderScene(std::string filename);
	void renderSceneThreaded(const int numThreads);

	void renderScene1(const int numThreads);
	void renderScene1Frame(Scene* copy, const int frame);

	void renderScene2(const int numThreads);
	void renderScene2Frame(Scene* copy, const int frame);

	void renderScene3(const int numThreads);
	void renderScene3Frame(Scene* copy, const int frame);

	void printPixelColor(int x, int y);
	Intersection getFirstIntersection(Ray &ray);
	bool isInShadow(Intersection &intersection, Light* light);

private:
	Intersection getFirstIntersectionSDS(BVH_Node* node, Ray &ray);
	void initSDS();
};

#endif