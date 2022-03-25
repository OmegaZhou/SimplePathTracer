#define _USE_MATH_DEFINES
#include "Component/Scene.h"
#include "Component\BlinnPhongMaterial.h"
#include <ctime>
#include <cmath>
void safeIncrease(int& i, int max_v)
{
	++i;
	if (i >= max_v) {
		throw std::exception("Need parameter");
	}
}
int main(int argc,char* argv[])
{
	//std::string scene_name = "bedroom";
	std::string scene_name = "cornell-box";
	//std::string scene_name = "veach-mis";
	std::string base_dir = "example-scenes-cg21\\";
	int spp = 64;
	std::string out_path;
	for (int i = 1; i < argc; ++i) {
		try {
			if (std::strcmp(argv[i], "-spp")==0) {
				safeIncrease(i, argc);
				sscanf(argv[i], "%d", &spp);
			} else if (std::strcmp(argv[i], "-scene") == 0) {
				safeIncrease(i, argc);
				scene_name = argv[i];
			} else if (std::strcmp(argv[i], "-dir") == 0) {
				safeIncrease(i, argc);
				base_dir = argv[i];
			} else if (std::strcmp(argv[i], "-o") == 0) {
				safeIncrease(i, argc);
				out_path = argv[i];
			}
		} catch (std::exception e) {
			std::cerr << "Invalid parameter" << std::endl;
		}
		
	}
	if (out_path.empty()) {
		out_path = scene_name + ".bmp";
	}
	if (base_dir.back() != '\\') {
		base_dir.push_back('\\');
	}
	
	Scene loader;
	std::cout << "Based Directory: " << base_dir << std::endl;
	std::cout << "Use Scene Name: " << scene_name << std::endl;
	loader.loadObj(scene_name, "example-scenes-cg21\\" + scene_name);
	//loader.loadObj("veach-mis", "example-scenes-cg21\\veach-mis");
	//loader.loadObj("cornell-box", "example-scenes-cg21\\cornell-box");
	std::clock_t start = std::clock();
	//BlinnPhongMaterial m;
	//m.ks = ZLib::Vec3f(1, 1, 1);
	//m.kd = ZLib::Vec3f();
	//m.ns = 100;
	//ZLib::Vec3f sum;
	//const int n = 1e7;
	//for (int i = 0; i < n; ++i) {
	//	float pdf;
	//	bool tmp;
	//	ZLib::Vec3f v(0, 0, 1);
	//	auto dir = m.sample(v, v, pdf, tmp, 0);
	//	if (dir.dot(v) > 0 && pdf > 0) {
	//		auto val = m.brdf(v, dir, ZLib::Vec2f(), v) * std::max(0.f, dir.dot(v));
	//		sum += val / pdf;
	//	}
	//	
	//}
	//sum /= n;;
	//std::cout << sum << std::endl;
	loader.render(spp, out_path);
	/*loader.render(16, scene_name + "16.bmp");
	loader.render(64, scene_name + "64.bmp");
	loader.render(256, scene_name + "256.bmp");
	loader.render(1024, scene_name + "1024.bmp");
	loader.render(4096, scene_name + "4096.bmp");*/
	std::clock_t end = std::clock();
	std::printf("%lfs", (double)(end - start) / CLOCKS_PER_SEC);
	return 0;
}