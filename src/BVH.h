#ifndef BVH_H
#define BVH_H

#include "Object.h"
#include <vector>

using namespace Objects;

struct BVH_Node {
	BVH_Node* left;
	BVH_Node* right;

	Object* object;
	BoundingBox box;

	BVH_Node();

	void buildTree(std::vector<Object*> objects, int axis);
	void sortObjects(std::vector<Object*> &objects, int axis);

private:
	static bool sortObjectsX(Object* obj1, Object* obj2);
	static bool sortObjectsY(Object* obj1, Object* obj2);
	static bool sortObjectsZ(Object* obj1, Object* obj2);
};

#endif