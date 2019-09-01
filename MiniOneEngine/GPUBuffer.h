#pragma once
#include "DevicePlatform.h"
namespace mini
{
#ifdef D3D11DEVICE
#define CGPUBuffer ID3D11Buffer
#else
	class CGPUBuffer
	{
	public:
		CGPUBuffer();
		virtual ~CGPUBuffer();
	};

#endif // D3D11DEVICE

}
