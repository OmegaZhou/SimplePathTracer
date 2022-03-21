#pragma once
#include "../ZMath/Zmath.h"
class Camera
{
	typedef ZLib::Vec3f Vec3f;
public:
	enum class Type
	{
		Perspective
	};
	Vec3f position;
	Vec3f lookat;
	Vec3f up;
	float fovy;
	int width;
	int height;
	Type type;
};