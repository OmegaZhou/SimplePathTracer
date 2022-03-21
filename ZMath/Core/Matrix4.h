#pragma once
#include "Vec3.h"
#include "Vec4.h"
#include "../../CudaUtils/CudaSupport.h"
#include <cstring>
namespace ZLib
{
	template<class T>
	class Matrix4
	{
	public:
		static Matrix4<T> identity();
		CUDA_SUPPORT_FUNCTION Matrix4();
		template<class U>
		CUDA_SUPPORT_FUNCTION Matrix4(const Matrix4<U>& m);
		template<class U>
		CUDA_SUPPORT_FUNCTION Matrix4<T>& operator=(const Matrix4<U>& m);
		template<class U>
		CUDA_SUPPORT_FUNCTION Vec3<U> operator*(const Vec3<U>& v) const;
		template<class U>
		CUDA_SUPPORT_FUNCTION Vec4<U> operator*(const Vec4<U>& v) const;
		CUDA_SUPPORT_FUNCTION Matrix4<T> operator*(const Matrix4<T>& v) const;
		CUDA_SUPPORT_FUNCTION Matrix4<T> operator*(T v) const;
		CUDA_SUPPORT_FUNCTION Matrix4<T>& operator*=(const Matrix4<T>& v);
		CUDA_SUPPORT_FUNCTION Matrix4<T>& operator*=(T v);
		CUDA_SUPPORT_FUNCTION Matrix4<T> operator+(const Matrix4<T>& v) const;
		CUDA_SUPPORT_FUNCTION Matrix4<T>& operator+=(const Matrix4<T>& v);
		CUDA_SUPPORT_FUNCTION T* operator[](size_t i);
		CUDA_SUPPORT_FUNCTION const T* operator[](size_t i) const;
	private:
		T data[16];
	};
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T> Matrix4<T>::identity()
	{
		Matrix4<T> ret;
		ret[0][0] = 1;
		ret[1][1] = 1;
		ret[2][2] = 1;
		ret[3][3] = 1;
		return ret;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T>::Matrix4()
	{
		std::memset(data, 0, 16 * sizeof(T));
	}
	template<class T>
	template<class U>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T>::Matrix4(const Matrix4<U>& m)
	{
		*this = m;
	}
	template<class T>
	template<class U>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T>& Matrix4<T>::operator=(const Matrix4<U>& m)
	{
		auto arr = m[0];
		for (int i = 0; i < 16; ++i) {
			data[i] = arr[i];
		}
		return *this;
	}
	template<class T>
	template<class U>
	CUDA_SUPPORT_FUNCTION inline Vec3<U> Matrix4<T>::operator*(const Vec3<U>& v) const
	{
		Vec3<U> ret;
		for (int i = 0; i < 3; ++i) {
			ret[i] = (*this)[i][0] * v[0] + (*this)[i][1] * v[1] + (*this)[i][2] * v[2];
		}
		return ret;
	}
	template<class T>
	template<class U>
	CUDA_SUPPORT_FUNCTION inline Vec4<U> Matrix4<T>::operator*(const Vec4<U>& v) const
	{
		Vec4<U> ret;
		for (int i = 0; i < 4; ++i) {
			ret[i] = (*this)[i][0] * v[0] + (*this)[i][1] * v[1] + (*this)[i][2] * v[2] + (*this)[i][3] * v[3];
		}
		return ret;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& v) const
	{
		Matrix4<T> ret;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				ret[i][j] = (*this)[i][0] * v[0][j] + (*this)[i][1] * v[1][j] + (*this)[i][2] * v[2][j] + (*this)[i][3] * v[3][j];
			}
		}
		return ret;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T> Matrix4<T>::operator*(T v) const
	{
		Matrix4<T> ret = *this;
		ret *= v;
		return ret;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T>& v)
	{
		*this = (*this) * v;
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T>& Matrix4<T>::operator*=(T v)
	{
		for (int i = 0; i < 16; ++i) {
			data[i] *= v;
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T> Matrix4<T>::operator+(const Matrix4<T>& v) const
	{
		Matrix4<T> ret = *this;
		ret += v;
		return ret;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T>& v)
	{
		auto arr = v[0];
		for (int i = 0; i < 16; ++i) {
			data[i] += arr[i];
		}
		return *this;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline T* Matrix4<T>::operator[](size_t i)
	{
		return data + i * 4;
	}
	template<class T>
	CUDA_SUPPORT_FUNCTION inline const T* Matrix4<T>::operator[](size_t i) const
	{
		return data + i * 4;
	}
	using Matrix4f = Matrix4<float>;
}

