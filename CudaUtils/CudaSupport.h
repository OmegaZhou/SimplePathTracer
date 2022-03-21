#pragma once
#ifdef SUPPORT_CUDA
#define CHECK_USE_CUDA(state) state
#define CUDA_SUPPORT_FUNCTION __host__ __device__
#else
#define CHECK_USE_CUDA(state) false
#define CUDA_SUPPORT_FUNCTION
#endif
namespace ZLib
{
	void safeCopyMemory(void* des, void* src, size_t byte_num, bool use_cuda);
	void* safeMallocMemory(size_t byte_num, bool use_cuda);
	void safeFreeMemory(void* m, bool use_cuda);
	void safeMemset(void* des, int val, size_t num, bool use_cuda);
}