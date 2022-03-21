#pragma once

namespace ZLib
{
	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned int u32;
	typedef unsigned long long u64;
	struct Color
	{
	public:
		u8 b;
		u8 g;
		u8 r;
		u8 a;
	};
	const Color Red{ 0, 0, 255, 255 };
	const Color Green{ 0, 255, 0, 255 };
	const Color Blue{ 255, 0, 0, 255 };
	const Color Black{ 0, 0, 0, 255 };
	const Color White{ 255, 255, 255, 255 };
}
