#include "ImageBuffer.hpp"
#include <fstream>
#include <exception>
#include <iostream>
#include <cstring>
#include <sstream>
namespace ZLib
{
	void outputNumberToBytes(u64 number, u8 len, std::ostream &os)
	{
		int k = 0;
		for (int i = 0; i < len; ++i, k += 8)
		{
			os << (u8)((number >> k) & 0xff);
		}
	}
}

size_t ZLib::getIndex(size_t x, size_t y, size_t w, size_t h)
{
	return w * y + x;
}

ZLib::ImageBuffer::ImageBuffer(int w, int h):frame_buffer(nullptr), width(0), height(0),max_buffer_size(0)
{
	resize(w, h);
	clearBuffer();
}

ZLib::ImageBuffer::~ImageBuffer()
{
	freeBuffer();
}

bool ZLib::ImageBuffer::saveImage(const std::string& path) const
{
	std::string str;
	std::ostringstream string_os(std::ostringstream::binary | std::ostringstream::out);
	std::ofstream os;
	try
	{

		// Bpm header
		{
			// OS
			string_os << "BM";
			// File size
			outputNumberToBytes((26 + width * height * 4), 4, string_os);
			// Reserved
			outputNumberToBytes(0, 2, string_os);
			outputNumberToBytes(0, 2, string_os);
			// Offset pixel data
			outputNumberToBytes(26, 4, string_os);
		}

		// DIB header
		{
			// DIB header size
			outputNumberToBytes(12, 4, string_os);
			// Image width
			outputNumberToBytes(width, 2, string_os);
			// Image height
			outputNumberToBytes(height, 2, string_os);
			// Must be 1
			outputNumberToBytes(1, 2, string_os);
			// Color bytes
			outputNumberToBytes(32, 2, string_os);
		}
		
		// Color Data
		{
			for (int j = 0; j < height; ++j)
			{
				for (int i = 0; i < width; ++i)
				{
					const Color &color = getColor(i, j);
					string_os << color.b << color.g << color.r << color.a;
				}
			}
		}
		os.open(path, std::ios::out | std::ios::binary);
		os << string_os.str();
		os.close();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
}

size_t ZLib::ImageBuffer::getIndex(int x, int y) const
{
	return ZLib::getIndex(x, y, width, height);
}

const ZLib::Color& ZLib::ImageBuffer::getColor(int x, int y) const
{
	return frame_buffer[getIndex(x, y)];
}

size_t ZLib::ImageBuffer::getWidth() const
{
	return width;
}

size_t ZLib::ImageBuffer::getHeight() const
{
	return height;
}

void ZLib::ImageBuffer::setColor(int x, int y, const Color &color)
{
	frame_buffer[getIndex(x, y)] = color;
}

void ZLib::ImageBuffer::clearBuffer()
{
	std::memset(frame_buffer, 0, sizeof(Color) * width * height);
}

void ZLib::ImageBuffer::resize(int w, int h)
{
	if (max_buffer_size < w * h||frame_buffer==nullptr) {
		freeBuffer();
		frame_buffer = new Color[(size_t)w * h];
		max_buffer_size = w * h;
	}
	width = w;
	height = h;
	
}

const ZLib::Color* ZLib::ImageBuffer::getBuffer() const
{
	return frame_buffer;
}

void ZLib::ImageBuffer::freeBuffer()
{
	if (frame_buffer) {
		delete[] frame_buffer;
		frame_buffer = nullptr;
	}
}

ZLib::ImageBuffer::ImageBuffer(const ImageBuffer& buffer) : frame_buffer(nullptr)
{
	*this = buffer;
}

ZLib::ImageBuffer& ZLib::ImageBuffer::operator=(const ImageBuffer& buffer)
{
	resize(buffer.width, buffer.height);
	memcpy(frame_buffer, buffer.frame_buffer, width * height * sizeof(Color));
	return *this;
}
