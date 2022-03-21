#include "AABB.h"

bool AABB::isIntersect(const Ray& ray) const
{
	ZLib::Vec3f t1 = (min_value - ray.origin) / ray.dir;
	ZLib::Vec3f t2 = (max_value - ray.origin) / ray.dir;
	ZLib::Vec3f max_t = ZLib::max(t1, t2);
	ZLib::Vec3f min_t = ZLib::min(t1, t2);
	float total_mint = std::max(min_t[0], std::max(min_t[1], min_t[2]));
	float total_maxt = std::min(max_t[0], std::min(max_t[1], max_t[2]));
	return total_maxt >= 0 && total_mint <= total_maxt;
}

AABB::AABB()
{}
