#pragma once

#include "../Interface/ITexture.h"
#include "../Lib/stb_image.h"
#include "../ZMath/Zmath.h"
#include <string>
class BaseTexture :public ITexture
{
public:
	BaseTexture();
	BaseTexture(const std::string& image_path);
	void load(const std::string& image_path);
	virtual ~BaseTexture();
	// Í¨¹ý ITexture ¼Ì³Ð
	virtual ZLib::Vec3f sample(float u, float v) const override;
private:
	unsigned char* data;
	int width;
	int height;
	int n;
};