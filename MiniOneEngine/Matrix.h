#pragma once
#include "DevicePlatform.h"
namespace mini
{
#ifdef WIN32

#define CMatrix D3DXMATRIX

#else
	class CMatrix
	{
	public:
		CMatrix();
		~CMatrix();
	};

#endif
}