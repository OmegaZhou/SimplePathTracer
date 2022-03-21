#include "ResourceManager.h"

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager manager;
	return manager;
}

ResourceManager::~ResourceManager()
{
	free();
}

void ResourceManager::free()
{
	for (auto i : materials) {
		delete i;
	}
	for (auto i : textures) {
		delete i;
	}
	for (auto i : shapes) {
		delete i;
	}
	materials.clear();
	shapes.clear();
	textures.clear();
}

ResourceManager::ResourceManager()
{}
