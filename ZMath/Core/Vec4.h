#pragma once
#include "../../CudaUtils/CudaSupport.h"
#include <cmath>
namespace ZLib
{
	template<class T>
	class Vec4
	{
	public:
		CUDA_SUPPORT_FUNCTION Vec4();
		CUDA_SUPPORT_FUNCTION Vec4(T x, T y, T z, T w);
		CUDA_SUPPORT_FUNCTION template<class R>
		CUDA_SUPPORT_FUNCTION Vec4(const Vec4<R>& v);
		CUDA_SUPPORT_FUNCTION T& x();
		CUDA_SUPPORT_FUNCTION const T& x() const;
		CUDA_SUPPORT_FUNCTION T& y();
		CUDA_SUPPORT_FUNCTION const T& y() const;
		CUDA_SUPPORT_FUNCTION T& z();
		CUDA_SUPPORT_FUNCTION const T& z() const;
		CUDA_SUPPORT_FUNCTION T& w();
		CUDA_SUPPORT_FUNCTION const T& w() const;
		CUDA_SUPPORT_FUNCTION T& operator[](size_t i);
		CUDA_SUPPORT_FUNCTION const T& operator[](size_t i) const;

		/// <summary>
		/// 该函数默认此时Vec4代表三维向量，即w==0
		/// </summary>
		/// <param name="v"></param>
		/// <returns></returns>
		CUDA_SUPPORT_FUNCTION Vec4<T> cross(const Vec4<T>& v) const;
		CUDA_SUPPORT_FUNCTION T dot(const Vec4<T>& v)const;
		CUDA_SUPPORT_FUNCTION T norm() const;
		CUDA_SUPPORT_FUNCTION T squaredNorm() const;
		CUDA_SUPPORT_FUNCTION void normalize();
		CUDA_SUPPORT_FUNCTION Vec4<T>& operator=(const Vec4<T>& v);
		CUDA_SUPPORT_FUNCTION Vec4<T>& operator+=(const Vec4<T>& v);
		CUDA_SUPPORT_FUNCTION Vec4<T> operator+(const Vec4<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec4<T>& operator-=(const Vec4<T>& v);
		CUDA_SUPPORT_FUNCTION Vec4<T> operator-(const Vec4<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec4<T>& operator*=(const Vec4<T>& v);
		CUDA_SUPPORT_FUNCTION Vec4<T> operator*(const Vec4<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec4<T>& operator/=(const Vec4<T>& v);
		CUDA_SUPPORT_FUNCTION Vec4<T> operator/(const Vec4<T>& v) const;
		CUDA_SUPPORT_FUNCTION Vec4<T>& operator*=(T v);
		CUDA_SUPPORT_FUNCTION Vec4<T> operator*(T v) const;
		CUDA_SUPPORT_FUNCTION Vec4<T>& operator/=(T v);
		CUDA_SUPPORT_FUNCTION Vec4<T> operator/(T v) const;
		CUDA_SUPPORT_FUNCTION Vec4<T> operator-() const;
	private:
		T data[4];
	};
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>::Vec4()
	{
		data[0] = data[1] = data[2] = data[3] = 0;
	}

	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>::Vec4(T x, T y, T z, T w)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	}
	template<class T>
	template<class R>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>::Vec4(const Vec4<R>& v)
	{
		for (int i = 0; i < 4; ++i) {
			data[i] = v[i];
		}
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec4<T>::x()
	{
		return data[0];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec4<T>::x() const
	{
		return data[0];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec4<T>::y()
	{
		return data[1];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec4<T>::y() const
	{
		return data[1];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec4<T>::z()
	{
		return data[2];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec4<T>::z() const
	{
		return data[2];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec4<T>::w()
	{
		return data[3];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec4<T>::w() const
	{
		return data[3];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec4<T>::operator[](size_t i)
	{
		return data[i];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec4<T>::operator[](size_t i) const
	{
		return data[i];
	}

	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T> Vec4<T>::cross(const Vec4<T>& v) const
	{
		return Vec4<T>(data[1] * v[2] - data[2] * v[1], data[2] * v[0] - data[0] * v[2], data[0] * v[1] - data[1] * v[0], 0);
	}

	template<class T>
	CUDA_SUPPORT_FUNCTION inline T Vec4<T>::dot(const Vec4<T>& v) const
	{
		return data[0] * v[0] + data[1] * v[1] + data[2] * v[2] + data[3] * v[3];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T Vec4<T>::norm() const
	{
		return std::sqrt(squaredNorm());
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T Vec4<T>::squaredNorm() const
	{
		return dot(*this);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline void Vec4<T>::normalize()
	{
		T l = norm();
		*this /= l;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>& Vec4<T>::operator=(const Vec4<T>& v)
	{
		for (int i = 0; i < 4; ++i) {
			data[i] = v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& v)
	{
		for (int i = 0; i < 4; ++i) {
			data[i] += v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T> Vec4<T>::operator+(const Vec4<T>& v) const
	{
		Vec4<T> re = *this;
		re += v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& v)
	{
		for (int i = 0; i < 4; ++i) {
			data[i] -= v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T> Vec4<T>::operator-(const Vec4<T>& v) const
	{
		Vec4<T> re = *this;
		re -= v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>& Vec4<T>::operator*=(const Vec4<T>& v)
	{
		for (int i = 0; i < 4; ++i) {
			data[i] *= v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T> Vec4<T>::operator*(const Vec4<T>& v) const
	{
		Vec4<T> re = *this;
		re *= v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>& Vec4<T>::operator/=(const Vec4<T>& v)
	{
		for (int i = 0; i < 4; ++i) {
			data[i] /= v[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T> Vec4<T>::operator/(const Vec4<T>& v) const
	{
		Vec4<T> re = *this;
		re /= v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>& Vec4<T>::operator*=(T v)
	{
		for (int i = 0; i < 4; ++i) {
			data[i] *= v;
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T> Vec4<T>::operator*(T v) const
	{
		Vec4<T> re = *this;
		re *= v;
		return re;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T>& Vec4<T>::operator/=(T v)
	{
		return operator*=(1 / v);
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec4<T> Vec4<T>::operator/(T v) const
	{
		return (*this) * (1 / v);
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION Vec4<T> Vec4<T>::operator-() const
	{
		return operator*(-1);
	}
	using Vec4f = Vec4<float>;
	using Vec4i = Vec4<int>;
}
