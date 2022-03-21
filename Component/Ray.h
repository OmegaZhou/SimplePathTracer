#pragma once
#include "../ZMath/Zmath.h"
class Ray
{
public:
	Ray(const ZLib::Vec3f& _origin, const ZLib::Vec3f& _dir) :origin(_origin), dir(_dir)
	{
		dir.normalize();
		inv_dir = ZLib::Vec3f(1.0f / dir[0], 1.0f / dir[1], 1.0f / dir[2]);
	}
	ZLib::Vec3f origin;
	ZLib::Vec3f dir;
	ZLib::Vec3f inv_dir;
};