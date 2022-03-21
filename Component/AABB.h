#pragma once
#include "Ray.h"
#include "../ZMath/Zmath.h"
class AABB
{
public:
	bool isIntersect(const Ray& ray) const;
	ZLib::Vec3f min_value;
	ZLib::Vec3f max_value;
	AABB();
};