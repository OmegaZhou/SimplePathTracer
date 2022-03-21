#pragma once
#ifdef SUPPORT_CUDA
#define CHECK_USE_CUDA(state) state
#define FUNC_IDENTITY __host__ __device__
#else
#define CHECK_USE_CUDA(state) false
#define FUNC_IDENTITY
#endif
namespace ZLib
{
	void safeCopyMemory(void* des, void* src, size_t byte_num, use_);
	void* safeMallocMemory(size_t byte_num);
	void safeFreeMemory(void*& m);
}