#pragma once
#include "../ZMath/Zmath.h"
#include <string>
class IMaterial
{
public:
	virtual ZLib::Vec3f sample(const ZLib::Vec3f& wi, const ZLib::Vec3f& normal, float& pdf, bool& is_refract, int engine_id) const = 0;
	virtual ZLib::Vec3f brdf(const ZLib::Vec3f& wi, const ZLib::Vec3f& wo, const ZLib::Vec2f& uv, const ZLib::Vec3f& normal) const = 0;
	virtual const std::string& name() const = 0;
	virtual bool hasEmission() const = 0;
	virtual ZLib::Vec3f emit() const = 0;
	virtual bool canRefract() const = 0;
	virtual ~IMaterial(){}
};