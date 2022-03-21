#define STB_IMAGE_IMPLEMENTATION
#include "BaseTexture.h"
#include <algorithm>
BaseTexture::BaseTexture():data(nullptr)
{}
BaseTexture::BaseTexture(const std::string& image_path)
{
    load(image_path);
}

void BaseTexture::load(const std::string& image_path)
{
    data = stbi_load(image_path.c_str(), &width, &height, &n, 0);
}

BaseTexture::~BaseTexture()
{
    if (data) {
        stbi_image_free(data);
    }
    
}

ZLib::Vec3f BaseTexture::sample(float u, float v) const
{
    int x = u * width;
    int y = v * height;
    x = (x % width + width) % width;
    y = (y % height + height) % height;
    y = height - 1 - y;
    float r = data[width * n * y + n * x + 0] / 255.0f;
    float g = data[width * n * y + n * x + 1] / 255.0f;
    float b = data[width * n * y + n * x + 2] / 255.0f;
    return ZLib::Vec3f(r, g, b);
}
