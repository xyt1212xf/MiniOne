#include "pch.h"
#include "PostRender.h"
namespace mini
{
	CPostRender::CPostRender()
		: mpShader(nullptr)
	{
	}


	CPostRender::~CPostRender()
	{
		SafeRelease(mpShader);
	}
}
