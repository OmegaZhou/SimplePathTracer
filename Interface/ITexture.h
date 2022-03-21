#pragma once
#include "../ZMath/Zmath.h"
class ITexture
{
public:
	virtual ZLib::Vec3f sample(float u, float v) const = 0;
	virtual ~ITexture()
	{};
};