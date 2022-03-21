#pragma once
#include "../../CudaUtils/CudaSupport.h"
#include <cmath>
namespace ZLib
{
	template<class T>
	class Vec3
	{
	public:
		CUDA_SUPPORT_FUNCTION Vec3();
		CUDA_SUPPORT_FUNCTION Vec3(T x, T y, T z);
		template<class R>
		CUDA_SUPPORT_FUNCTION Vec3(const Vec3<R>& v);
		CUDA_SUPPORT_FUNCTION T& x();
		CUDA_SUPPORT_FUNCTION const T& x() const;
		CUDA_SUPPORT_FUNCTION T& y();
		CUDA_SUPPORT_FUNCTION const T& y() const;
		CUDA_SUPPORT_FUNCTION T& z();
		CUDA_SUPPORT_FUNCTION const T& z() const;
		CUDA_SUPPORT_FUNCTION T& operator[](size_t i);
		CUDA_SUPPORT_FUNCTION const T& operator[](size_t i) const;
		CUDA_SUPPORT_FUNCTION Vec3<T> cross(const Vec3<T>& v)const;
		CUDA_SUPPORT_FUNCTION T dot(const Vec3<T>& v)const;
		CUDA_SUPPORT_FUNCTION T norm() const;
		CUDA_SUPPORT_FUNCTION T squaredNorm() const;
		CUDA_SUPPORT_FUNCTION void normalize();
		CUDA_SUPPORT_FUNCTION bool operator==(const Vec3<T>& v) const;
		CUDA_SUPPORT_FUNCTION bool operator!=(const Vec3<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec3<T>& operator=(const Vec3<T>& v);
		CUDA_SUPPORT_FUNCTION Vec3<T>& operator+=(const Vec3<T>& v);
		CUDA_SUPPORT_FUNCTION Vec3<T> operator+(const Vec3<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec3<T>& operator-=(const Vec3<T>& v);
		CUDA_SUPPORT_FUNCTION Vec3<T> operator-(const Vec3<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec3<T>& operator*=(const Vec3<T>& v);
		CUDA_SUPPORT_FUNCTION Vec3<T> operator*(const Vec3<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec3<T>& operator/=(const Vec3<T>& v);
		CUDA_SUPPORT_FUNCTION Vec3<T> operator/(const Vec3<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec3<T>& operator*=(T v);
		CUDA_SUPPORT_FUNCTION Vec3<T> operator*(T v) const;
		CUDA_SUPPORT_FUNCTION Vec3<T>& operator/=(T v);
		CUDA_SUPPORT_FUNCTION Vec3<T> operator/(T v) const;
		CUDA_SUPPORT_FUNCTION Vec3<T> operator-() const;
	private:
		T data[3];
	};
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>::Vec3()
	{
		data[0] = data[1] = data[2] = 0;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>::Vec3(T x, T y, T z)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}
	template<class T>
	template<class R>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>::Vec3(const Vec3<R>& v)
	{
		for (int i = 0; i < 3; ++i) {
			data[i] = v[i];
		}
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec3<T>::x()
	{
		return data[0];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec3<T>::x() const
	{
		return data[0];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec3<T>::y()
	{
		return data[1];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec3<T>::y() const
	{
		return data[1];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec3<T>::z()
	{
		return data[2];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec3<T>::z() const
	{
		return data[2];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec3<T>::operator[](size_t i)
	{
		return data[i];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec3<T>::operator[](size_t i) const
	{
		return data[i];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T> Vec3<T>::cross(const Vec3<T>& v) const
	{
		return Vec3<T>(data[1] * v[2] - data[2] * v[1], data[2] * v[0] - data[0] * v[2], data[0] * v[1] - data[1] * v[0]);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T Vec3<T>::dot(const Vec3<T>& v) const
	{
		return data[0] * v[0] + data[1] * v[1] + data[2] * v[2];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T Vec3<T>::norm() const
	{
		return std::sqrt(squaredNorm());
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T Vec3<T>::squaredNorm() const
	{
		return dot(*this);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline void Vec3<T>::normalize()
	{
		T l = norm();
		*this /= l;
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION bool Vec3<T>::operator==(const Vec3<T>& v) const
	{
		return data[0] == v[0] && data[1] == v[1] && data[2] == v[2];
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION bool Vec3<T>::operator!=(const Vec3<T>& v) const
	{
		return !operator==(v);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>& Vec3<T>::operator=(const Vec3<T>& v)
	{
		for (int i = 0; i < 3; ++i) {
			data[i] = v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& v)
	{
		for (int i = 0; i < 3; ++i) {
			data[i] += v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T> Vec3<T>::operator+(const Vec3<T>& v) const
	{
		Vec3<T> re = *this;
		re += v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& v)
	{
		for (int i = 0; i < 3; ++i) {
			data[i] -= v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T> Vec3<T>::operator-(const Vec3<T>& v) const
	{
		Vec3<T> re = *this;
		re -= v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>& Vec3<T>::operator*=(const Vec3<T>& v)
	{
		for (int i = 0; i < 3; ++i) {
			data[i] *= v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T> Vec3<T>::operator*(const Vec3<T>& v) const
	{
		Vec3<T> re = *this;
		re *= v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>& Vec3<T>::operator/=(const Vec3<T>& v)
	{
		for (int i = 0; i < 3; ++i) {
			data[i] /= v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T> Vec3<T>::operator/(const Vec3<T>& v) const
	{
		Vec3<T> re = *this;
		re /= v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>& Vec3<T>::operator*=(T v)
	{
		for (int i = 0; i < 3; ++i) {
			data[i] *= v;
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T> Vec3<T>::operator*(T v) const
	{
		Vec3<T> re = *this;
		re *= v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T>& Vec3<T>::operator/=(T v)
	{
		return operator*=(1 / v);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec3<T> Vec3<T>::operator/(T v) const
	{
		return (*this) * (1 / v);
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION Vec3<T> Vec3<T>::operator-() const
	{
		return (*this)*(-1);
	}
	using Vec3f = Vec3<float>;
	using Vec3i = Vec3<int>;
}