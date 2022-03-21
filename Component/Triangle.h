#pragma once
#include "../Interface\IShape.h"
class Triangle :public IShape
{
	typedef ZLib::Vec3f Vec3f;
	typedef ZLib::Vec2f Vec2f;
public:
	// Í¨¹ý IShape ¼Ì³Ð
	virtual bool intersect(const Ray& ray, Intersection& ret) const override;
	virtual const AABB& getBoundingBox() const override;
	virtual float area() const override;
	virtual void sample(Intersection& pos, float& pdf, int engine_id) const override;
	virtual ~Triangle();
	void initAABB();
	Triangle();
	AABB aabb;
	Vec3f v[3];
	Vec3f normal[3];
	Vec2f uv[3];
	Vec3f barycentricInterpolation(const Vec3f& p) const;
private:
	float _area;
};