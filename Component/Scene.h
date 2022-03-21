#pragma once
#include "../Interface/IShape.h"
#include "../Interface/IMaterial.h"
#include "../Interface/ITexture.h"
#include "Bvh.h"
#include "Camera.h"
#include <vector>
#include <map>
#include <memory>
class Scene
{
public:
	ITexture* getTexture(const std::string& name);
	IMaterial* getMaterial(const std::string& name);
	void loadObj(const std::string& file_path, const std::string& mtl_dir);
	void render(int spp = 1,const std::string& image_path="a.bmp");
	Intersection find(const Ray& ray, int thread_id);
	Intersection sampleLight(float& pdf, int engine_id);
	Intersection sampleLight2(float& pdf, int engine_id);
private:
	std::string base_dir;
	std::vector<IShape*> shapes;
	std::vector<IShape*> lights;
	std::map<std::string, IMaterial*> materials;
	std::map<std::string, ITexture* > textures;
	ZLib::BVH bvh;
	Camera camera;
	ZLib::Vec3f castRay(const Ray& ray, int thread_id = 0, int depth = 0);
	ZLib::Vec3f castRay2(const Ray& ray, int thread_id);
	ZLib::Vec3f castRay3(const Ray& ray, int thread_id);
};