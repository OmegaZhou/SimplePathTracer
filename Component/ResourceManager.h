#pragma once
#include "../Interface/IMaterial.h"
#include "../Interface/ITexture.h"
#include "../Interface/IShape.h"
#include <vector>
class ResourceManager
{
public:
	static ResourceManager& getInstance();
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(const ResourceManager&) = delete;
	~ResourceManager();
	void free();
	template<class T>
	T* createMaterial();
	template<class T>
	T* createShape();
	template<class T>
	T* createTexture();
private:
	ResourceManager();
	std::vector<IMaterial*> materials;
	std::vector<IShape*> shapes;
	std::vector<ITexture*> textures;
};

template<class T>
inline T* ResourceManager::createMaterial()
{
	T* obj = new T();
	materials.push_back(obj);
	return obj;
}

template<class T>
inline T* ResourceManager::createShape()
{
	T* obj = new T();
	shapes.push_back(obj);
	return obj;
}

template<class T>
inline T* ResourceManager::createTexture()
{
	T* obj = new T();
	textures.push_back(obj);
	return obj;
}
