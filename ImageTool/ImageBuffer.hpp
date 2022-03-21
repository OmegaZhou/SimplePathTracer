#pragma once
#include "Color.hpp"
#include <string>
namespace ZLib
{
	size_t getIndex(size_t x, size_t y, size_t w, size_t h);

	class ImageBuffer
	{
	public:
		ImageBuffer(int w, int h);
		ImageBuffer(const ImageBuffer& buffer);
		ImageBuffer& operator=(const ImageBuffer& buffer);
		~ImageBuffer();
		bool saveImage(const std::string& path) const;
		size_t getIndex(int x, int y) const;
		const Color& getColor(int x, int y) const;
		size_t getWidth() const;
		size_t getHeight() const;
		void setColor(int x, int y, const Color& color);
		void clearBuffer();
		virtual void resize(int w, int h);
		const Color* getBuffer() const;
	protected:
		virtual void freeBuffer();
		Color* frame_buffer;
		size_t width;
		size_t height;
		size_t max_buffer_size;
	};


}



