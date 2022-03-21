#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include <iostream>
namespace ZLib
{
	constexpr float Epsilon = 1e-5;
	template<size_t Size, class Vec>
	std::ostream& printVec(std::ostream& os, Vec t);
	template<class T>
	CUDA_SUPPORT_FUNCTION Vec4<T> createPoint(const Vec3<T>& v)
	{
		return Vec4<T>(v.x(), v.y(), v.z(), 1);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION Vec4<T> createPoint(T x, T y, T z)
	{
		return Vec4<T>(x, y, z, 1);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION Vec4<T> createVector(const Vec3<T>& v)
	{
		return Vec4<T>(v.x(), v.y(), v.z(), 0);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION Vec4<T> createVector(T x, T y, T z)
	{
		return Vec4<T>(x, y, z, 0);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION Vec3<T> toVec3(const Vec4<T>& v)
	{
		if (std::abs(v.w()) < Epsilon) {
			return Vec3<T>(v.x(), v.y(), v.z());
		} else {
			T inv_w = 1 / v.w();
			return Vec3<T>(v.x() * inv_w, v.y() * inv_w, v.z() * inv_w);
		}
		
	}

	template<class T>
	CUDA_SUPPORT_FUNCTION Matrix4<T> lookat(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up)
	{
		Matrix4<T> re = translate(-eye);
		Matrix4<T> tmp = Matrix4<T>::identity();
		Vec3<T> dir = target - eye;
		Vec3<T> x = dir.cross(up);
		Vec3<T> y = x.cross(dir);
		Vec3<T> z = -dir;
		x.normalize();
		y.normalize();
		z.normalize();
		for (int i = 0; i < 3; ++i) {
			tmp[0][i] = x[i];
			tmp[1][i] = y[i];
			tmp[2][i] = z[i];
		}
		return tmp * re;
	}

	template<class T>
	std::ostream& operator<<(std::ostream& os, Vec2<T> v)
	{
		return printVec<2>(os, v);
	}
	template<class T>
	std::ostream& operator<<(std::ostream& os, Vec3<T> v)
	{
		return printVec<3>(os, v);
	}

	template<class T>
	std::ostream& operator<<(std::ostream& os, Vec4<T> v)
	{
		return printVec<4>(os, v);
	}

	template<size_t Size, class Vec>
	std::ostream& printVec(std::ostream& os, Vec t)
	{
		os << "(";
		for (int i = 0; i < Size; ++i) {
			if (i != 0) {
				os << ", ";
			}
			os << t[i];
		}
		os << ")";
		return os;
	}
	template<class T>
	void clamp(T min_v, T max_v, Vec4<T>& v)
	{
		for (int i = 0; i < 4; ++i) {
			v[i] = std::max(min_v, v[i]);
			v[i] = std::min(max_v, v[i]);
		}
	}
	template<class T>
	void clamp(T min_v, T max_v, Vec3<T>& v)
	{
		for (int i = 0; i < 3; ++i) {
			v[i] = std::max(min_v, v[i]);
			v[i] = std::min(max_v, v[i]);
		}
	}
	template<class T>
	void clamp(T min_v, T max_v, Vec2<T>& v)
	{
		for (int i = 0; i < 2; ++i) {
			v[i] = std::max(min_v, v[i]);
			v[i] = std::min(max_v, v[i]);
		}
	}

	template<class T>
	Vec2<T> max(const Vec2<T>& a, const Vec2<T>& b)
	{
		Vec2<T> ret;
		for (int i = 0; i < 2; ++i) {
			ret[i] = std::max(a[i], b[i]);
		}
		return ret;
	}
	template<class T>
	Vec2<T> min(const Vec2<T>& a, const Vec2<T>& b)
	{
		Vec2<T> ret;
		for (int i = 0; i < 2; ++i) {
			ret[i] = std::min(a[i], b[i]);
		}
		return ret;
	}
	template<class T>
	Vec3<T> max(const Vec3<T>& a, const Vec3<T>& b)
	{
		Vec3<T> ret;
		for (int i = 0; i < 3; ++i) {
			ret[i] = std::max(a[i], b[i]);
		}
		return ret;
	}
	template<class T>
	Vec3<T> min(const Vec3<T>& a, const Vec3<T>& b)
	{
		Vec3<T> ret;
		for (int i = 0; i < 3; ++i) {
			ret[i] = std::min(a[i], b[i]);
		}
		return ret;
	}
	template<class T>
	Vec4<T> max(const Vec4<T>& a, const Vec4<T>& b)
	{
		Vec4<T> ret;
		for (int i = 0; i < 4; ++i) {
			ret[i] = std::max(a[i], b[i]);
		}
		return ret;
	}
	template<class T>
	Vec4<T> min(const Vec4<T>& a, const Vec4<T>& b)
	{
		Vec3<T> ret;
		for (int i = 0; i < 4; ++i) {
			ret[i] = std::min(a[i], b[i]);
		}
		return ret;
	}
	template<class T>
	Vec3<T> pow(const Vec3<T>& a, T b)
	{
		Vec3<T> ret;
		for (int i = 0; i < 3; ++i) {
			ret[i] = std::pow(a[i], b);
		}
		return ret;
	}
}