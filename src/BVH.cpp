#include "BVH.h"
#include <iostream>
#include <algorithm>

using namespace Objects;

BVH_Node::BVH_Node() {
	left = NULL;
	right = NULL;
	object = NULL;
	box = BoundingBox();
}

void BVH_Node::buildTree(std::vector<Object*> objects, int axis) {
	if (objects.size() == 1) {
		object = objects[0];
		box = object->boundingBox;
		return;
	}

	sortObjects(objects, axis);

	left = new BVH_Node();
	right = new BVH_Node();

	int half = objects.size() / 2;
	std::vector<Object*> objects_left(objects.begin(), objects.begin() + half);
	std::vector<Object*> objects_right(objects.begin() + half, objects.end());

	left->buildTree(objects_left, (axis + 1) % 3);
	right->buildTree(objects_right, (axis + 1) % 3);

	box = BoundingBox(left->box.min);
	box.addBox(left->box);
	box.addBox(right->box);
}

void BVH_Node::sortObjects(std::vector<Object*> &objects, int axis) {
	if (axis == 0) {
		// X-axis
		std::sort(objects.begin(), objects.end(), sortObjectsX);
	}
	else if (axis == 1) {
		// Y-axis
		std::sort(objects.begin(), objects.end(), sortObjectsY);
	}
	else {
		// Z-axis
		std::sort(objects.begin(), objects.end(), sortObjectsZ);
	}
}

bool BVH_Node::sortObjectsX(Object* obj1, Object* obj2) {
	BoundingBox box1 = obj1->boundingBox;
	BoundingBox box2 = obj2->boundingBox;

	float center1 = (box1.max.x + box1.min.x) / 2.f;
	float center2 = (box2.max.x + box2.min.x) / 2.f;

	return center1 < center2;
}

bool BVH_Node::sortObjectsY(Object* obj1, Object* obj2) {
	BoundingBox box1 = obj1->boundingBox;
	BoundingBox box2 = obj2->boundingBox;

	float center1 = (box1.max.y + box1.min.y) / 2.f;
	float center2 = (box2.max.y + box2.min.y) / 2.f;

	return center1 < center2;
}

bool BVH_Node::sortObjectsZ(Object* obj1, Object* obj2) {
	BoundingBox box1 = obj1->boundingBox;
	BoundingBox box2 = obj2->boundingBox;

	float center1 = (box1.max.z + box1.min.z) / 2.f;
	float center2 = (box2.max.z + box2.min.z) / 2.f;

	return center1 < center2;
}