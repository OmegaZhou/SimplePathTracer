#pragma once
#include <string>
class ObjLoader
{
public:
	void LoadObj(const std::string& file_path, const std::string& mtl_dir = "./");
};

