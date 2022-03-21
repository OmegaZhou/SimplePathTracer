#include "Triangle.h"
#include "RandomEngine.h"
bool Triangle::intersect(const Ray& ray, Intersection& ret) const
{
	auto e1 = v[1] - v[0], e2 = v[2] - v[0], s = ray.origin - v[0];
	auto q = ray.dir.cross(e2), r = s.cross(e1);
	auto denominator = q.dot(e1);
	if (std::abs(denominator) < 1e-6) {
		return false;
	}
	Vec3f w(r.dot(e2), q.dot(s), r.dot(ray.dir));
	w /= denominator;
	if (w[0] < 1e-5) {
		return false;
	}
	ret.t = w[0];
	w[0] = 1 - w[1] - w[2];
	
	ret.point = v[0] * w[0] + v[1] * w[1] + v[2] * w[2];
	//std::cout << ret.point - old << std::endl;
	for (int i = 0; i < 3; ++i) {
		if (w[i] < -1e-5) {
			return false;
		}
	}
	
	//Vec3f face_n = (v[2] - v[1]).cross(v[2] - v[0]);
	//ret.t = -face_n.dot(ray.origin - v[0]) / face_n.dot(ray.dir);
	//face_n.normalize();
	//if (std::abs(face_n.dot(ray.dir)) < 1e-5) {
	//	return false;
	//}
	////ret.t = -face_n.dot(ray.origin - v[0]) * face_n.dot(ray.inv_dir);
	//if (std::abs(face_n.dot(ray.origin - v[0])) < 1e-5) {
	//	return false;
	//}
	//
	////auto k = -face_n.dot(ray.origin - v[0]) / face_n.dot(ray.dir);
	//if (isnan(ret.t) || isinf(ret.t) || ret.t <= 1e-5) {
	//	return false;
	//}
	//ret.point = ray.origin + ray.dir * ret.t;
	//auto& p = ret.point;
	//Vec3f w = barycentricInterpolation(p);
	//for (int i = 0; i < 3; ++i) {
	//	if (w[i] < -1e-3) {
	//		return false;
	//	}
	//}
	ret.material = material;
	ret.normal = normal[0] * w[0] + normal[1] * w[1] + normal[2] * w[2];
	ret.uv = uv[0] * w[0] + uv[1] * w[1] + uv[2] * w[2];
	ret.obj = this;
	return true;
}

const AABB& Triangle::getBoundingBox() const
{
	return aabb;
}

Triangle::~Triangle()
{
}

void Triangle::initAABB()
{
	aabb.min_value = ZLib::min(v[0], ZLib::min(v[1], v[2]));
	aabb.max_value = ZLib::max(v[0], ZLib::max(v[1], v[2]));
	auto e1 = v[1] - v[0];
	auto e2 = v[2] - v[0];
	_area = e1.cross(e2).norm() * 0.5f;
}

Triangle::Triangle()
{}

Triangle::Vec3f Triangle::barycentricInterpolation(const Vec3f& p) const
{
	const float EPSILON = 1e-3;
	Vec3f w;
	Vec3f face_n = (v[2] - v[1]).cross(v[2] - v[0]);
	face_n.normalize();
	int i = 0, j = 1;
	int flag = true;
	for (int k = 0; k < 3 ; ++k) {
		if (std::abs(face_n[k]) < EPSILON) {
			i = k;
			for (j = i + 1; j < 2 ; ++j) {
				if (std::abs(face_n[j]) < EPSILON) {
					break;
					
				}
			}
			break;
		}
	}
	// i==z,j==y
	w[0] = ((p[i] - v[2][i]) * (v[1][j] - v[2][j]) - (v[1][i] - v[2][i]) * (p[j] - v[2][j])) / ((v[0][i] - v[2][i]) * (v[1][j] - v[2][j]) - (v[1][i] - v[2][i]) * (v[0][j] - v[2][j]));
	w[1] = ((p[i] - v[0][i]) * (v[2][j] - v[0][j]) - (v[2][i] - v[0][i]) * (p[j] - v[0][j])) / ((v[1][i] - v[0][i]) * (v[2][j] - v[0][j]) - (v[2][i] - v[0][i]) * (v[1][j] - v[0][j]));
	w[2] = 1 - w[0] - w[1];
	//Vec3f w;
	//Vec3f face_n = (v[2] - v[1]).cross(v[2] - v[0]);
	//face_n.normalize();
	//if (std::abs(face_n.dot(Vec3f(0, 1, 1))) < EPSILON) {
	//	w[0] = ((p.z() - v[2].z()) * (v[1].y() - v[2].y()) - (v[1].z() - v[2].z()) * (p.y() - v[2].y())) / ((v[0].z() - v[2].z()) * (v[1].y() - v[2].y()) - (v[1].z() - v[2].z()) * (v[0].y() - v[2].y()));
	//	w[1] = ((p.z() - v[0].z()) * (v[2].y() - v[0].y()) - (v[2].z() - v[0].z()) * (p.y() - v[0].y())) / ((v[1].z() - v[0].z()) * (v[2].y() - v[0].y()) - (v[2].z() - v[0].z()) * (v[1].y() - v[0].y()));
	//	w[2] = 1 - w[0] - w[1];
	//} else if (std::abs(face_n.dot(Vec3f(1, 0, 1))) < EPSILON) {
	//	w[0] = ((p.x() - v[2].x()) * (v[1].z() - v[2].z()) - (v[1].x() - v[2].x()) * (p.z() - v[2].z())) / ((v[0].x() - v[2].x()) * (v[1].z() - v[2].z()) - (v[1].x() - v[2].x()) * (v[0].z() - v[2].z()));
	//	w[1] = ((p.x() - v[0].x()) * (v[2].z() - v[0].z()) - (v[2].x() - v[0].x()) * (p.z() - v[0].z())) / ((v[1].x() - v[0].x()) * (v[2].z() - v[0].z()) - (v[2].x() - v[0].x()) * (v[1].z() - v[0].z()));
	//	w[2] = 1 - w[0] - w[1];
	//} else {
	//	w[0] = ((p.x() - v[2].x()) * (v[1].y() - v[2].y()) - (v[1].x() - v[2].x()) * (p.y() - v[2].y())) / ((v[0].x() - v[2].x()) * (v[1].y() - v[2].y()) - (v[1].x() - v[2].x()) * (v[0].y() - v[2].y()));
	//	w[1] = ((p.x() - v[0].x()) * (v[2].y() - v[0].y()) - (v[2].x() - v[0].x()) * (p.y() - v[0].y())) / ((v[1].x() - v[0].x()) * (v[2].y() - v[0].y()) - (v[2].x() - v[0].x()) * (v[1].y() - v[0].y()));
	//	w[2] = 1 - w[0] - w[1];
	//}

	return w;
}

void Triangle::sample(Intersection& pos, float& pdf, int engine_id) const
{
	auto& engine = RandomEngineManager::getInstance().getEngine(engine_id);
	float x = std::sqrt(engine.sampleUniform(0, 1)), y = engine.sampleUniform(0, 1);
	
	Vec3f w(1 - x, x * y, x - x * y);
	pos.point = v[0] * w[0] + v[1] * w[1] + v[2] * w[2];
	pos.material = material;
	pos.normal = normal[0] * w[0] + normal[1] * w[1] + normal[2] * w[2];
	pos.uv = uv[0] * w[0] + uv[1] * w[1] + uv[2] * w[2];
	pdf = 1.0f / _area;
}

float Triangle::area() const
{
	return _area;
}
