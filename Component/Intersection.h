#pragma once
#include "../ZMath/ZMath.h"
#include "../Interface/IMaterial.h"
#include <memory>
class IShape;
class Intersection
{
public:
	ZLib::Vec3f point;
	ZLib::Vec3f normal;
	ZLib::Vec2f uv;
	const IShape* obj;
	const IMaterial* material;
	float t;
};