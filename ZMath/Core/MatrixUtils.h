#pragma once
#include "Matrix4.h"
#include <iostream>
namespace ZLib
{
	template<class T>
	std::ostream& operator<<(std::ostream& os, const Matrix4<T>& m);
	template<class T>
	std::ostream& operator<<(std::ostream& os, const Matrix4<T>& m)
	{
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (j != 0) {
					os << ", ";
				}
				os << m[i][j];
			}
			os << std::endl;
		}
		return os;
	}
	template<class T>
	Matrix4<T> scale(const Vec3<T>& s)
	{
		auto ret = Matrix4<T>::identity();
		ret[0][0] = s[0];
		ret[1][1] = s[1];
		ret[2][2] = s[2];
		return ret;
	}

	template<class T>
	Matrix4<T> translate(const Vec3<T>& s)
	{
		auto ret = Matrix4<T>::identity();
		ret[0][3] = s[0];
		ret[1][3] = s[1];
		ret[2][3] = s[2];
		return ret;
	}
}