#pragma once
#include <vector>
#include "AABB.h"
#include "Ray.h"
#include "../Interface/IShape.h"
namespace ZLib
{

	// type=0 ����Ҷ�ӽڵ㣬type=1�����Ҷ�ڵ�
	struct BVHNodes
	{
		int* fa;
		bool* l_types;
		int* lefts;
		bool* r_types;
		int* rights;
	};

	struct  LeafNodes
	{
		int* fa;
		int* data_ids;
		unsigned int* morton_codes;
	};

	// must be [0,1]
	unsigned int getMortonCode(double x, double y, double z);
	class BVH
	{
	public:
		BVH();
		void build(const std::vector<IShape*>& shapes, int thread_num = 1);
		int getTriangleNum()const;
		Intersection find(int root, const Ray& ray, int thread_id = 0);
		void free();
		~BVH();
	public:
		void allocDeviceMemory(const std::vector<IShape*>& shapes, int thread_num);
		LeafNodes leaf_nodes;
		BVHNodes internal_nodes;
		// ��Ҷ�ڵ�ʹ��ǰn-1���ڵ㣬Ҷ�ڵ�ʹ�ú�n���ڵ�
		AABB* aabbs;
		int triangle_num;
		int* stacks1, * stacks2;
		std::vector<IShape*> shapes;

	};

}
