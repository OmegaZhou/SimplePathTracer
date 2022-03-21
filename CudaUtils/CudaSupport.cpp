#include "CudaSupport.h"
#include <cstring>
#include <cstdlib>
void ZLib::safeCopyMemory(void* des, void* src, size_t byte_num, bool use_cuda)
{
	if ((!src) || (!des)) {
		return;
	}
	if (!CHECK_USE_CUDA(use_cuda)) {
		std::memcpy(des, src, byte_num);
	}
}

void* ZLib::safeMallocMemory(size_t byte_num, bool use_cuda)
{
	if (byte_num == 0) {
		return nullptr;
	}
	if (!CHECK_USE_CUDA(use_cuda)) {
		return std::malloc(byte_num);
	} else {
		return nullptr;
	}
}

void ZLib::safeFreeMemory(void* m, bool use_cuda)
{
	if (m) {
		if (!CHECK_USE_CUDA(use_cuda)) {
			std::free(m);
		}
	}
}

void ZLib::safeMemset(void* des, int val, size_t num, bool use_cuda)
{
	if (!des) {
		return;
	}
	if (!CHECK_USE_CUDA(use_cuda)) {
		std::memset(des, val, num);
	}
}
