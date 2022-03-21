#pragma once
#include "IMaterial.h"
#include "../Component/Intersection.h"
#include "../Component/Ray.h"
#include "../Component/AABB.h"
#include <memory>
class IShape
{
public:
	virtual bool intersect(const Ray& ray, Intersection& ret) const = 0;
	virtual const AABB& getBoundingBox() const = 0;
	virtual float area() const = 0;
	virtual void sample(Intersection& pos, float& pdf, int engine_id) const = 0;
	virtual ~IShape(){}
	const IMaterial* material;
};