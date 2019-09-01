#include "pch.h"
#include "GPUBuffer.h"
namespace mini
{
#ifdef D3D11DEVICE
#else
	CGPUBuffer::CGPUBuffer()
	{
	}


	CGPUBuffer::~CGPUBuffer()
	{
	}
#endif
}