#pragma once
#include "../../CudaUtils/CudaSupport.h"
namespace ZLib
{
	template<class T>
	class Vec2
	{
	public:
		CUDA_SUPPORT_FUNCTION Vec2();
		CUDA_SUPPORT_FUNCTION Vec2(T x, T y);
		template<class R>
		CUDA_SUPPORT_FUNCTION Vec2(const Vec2<R>& v);
		CUDA_SUPPORT_FUNCTION T& x();
		CUDA_SUPPORT_FUNCTION const T& x() const;
		CUDA_SUPPORT_FUNCTION T& y();
		CUDA_SUPPORT_FUNCTION const T& y() const;
		CUDA_SUPPORT_FUNCTION T& operator[](size_t i);
		CUDA_SUPPORT_FUNCTION const T& operator[](size_t i) const;
		CUDA_SUPPORT_FUNCTION Vec2<T>& operator=(const Vec3<T>& v);
		CUDA_SUPPORT_FUNCTION Vec2<T>& operator*=(T v);
		CUDA_SUPPORT_FUNCTION Vec2<T> operator*(T v) const;
		CUDA_SUPPORT_FUNCTION Vec2<T>& operator+=(const Vec2<T>& v);
		CUDA_SUPPORT_FUNCTION Vec2<T> operator+(const Vec2<T>& v) const;
	private:
		T data[2];
	};
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec2<T>::Vec2()
	{
		data[0] = data[1] = 0;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Vec2<T>::Vec2(T x, T y)
	{
		data[0] = x;
		data[1] = y;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec2<T>::x()
	{
		return data[0];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec2<T>::x() const
	{
		return data[0];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec2<T>::y()
	{
		return data[1];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec2<T>::y() const
	{
		return data[1];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T& Vec2<T>::operator[](size_t i)
	{
		return data[i];
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T& Vec2<T>::operator[](size_t i) const
	{
		return data[i];
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION Vec2<T>& Vec2<T>::operator=(const Vec3<T>& v)
	{
		data[0] = v[0];
		data[1] = v[1];
		return *this;
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION Vec2<T>& Vec2<T>::operator*=(T v)
	{
		for (int i = 0; i < 2; ++i) {
			data[i] *= v;
		}
		return *this;
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION Vec2<T> Vec2<T>::operator*(T v) const
	{
		Vec2<T> ret = *this;
		ret *= v;
		return ret;
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& v)
	{
		for (int i = 0; i < 2; ++i) {
			data[i] += v[i];
		}
		return *this;
	}
	template<class T>
	inline CUDA_SUPPORT_FUNCTION Vec2<T> Vec2<T>::operator+(const Vec2<T>& v) const
	{
		Vec2<T> ret = *this;
		ret += v;
		return ret;
	}
	template<class T>
	template<class R>
	CUDA_SUPPORT_FUNCTION inline Vec2<T>::Vec2(const Vec2<R>& v)
	{
		data[0] = v[0];
		data[1] = v[1];
	}
	using Vec2f = Vec2<float>;
	using Vec2i = Vec2<int>;
}