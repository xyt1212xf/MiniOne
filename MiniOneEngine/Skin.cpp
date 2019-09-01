#include "pch.h"
#include "skin.h"
namespace mini
{
	CSkin::CSkin()
	{

	}

	CSkin::~CSkin()
	{
		for (auto i : mMeshes)
		{
			SafeRelease(i);
		}
	}

	bool CSkin::initialise()
	{
		return true;
	}
}