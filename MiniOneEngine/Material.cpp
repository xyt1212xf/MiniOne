#include "pch.h"
#include "Material.h"
#include "ResourceManager.h"
#include "ScriptPlugin.h"
#include "RendererPlugin.h"
namespace mini
{
	CMaterial::CMaterial()
		: mpShader(nullptr)
	{
		mResourceType = Material;
	}

	CMaterial::~CMaterial()
	{
		SafeRelease(mpShader);
		for (auto item : mTextures)
		{
			SafeRelease(item);
		}
	}



	void CMaterial::render(CDevice* pDevice)
	{
		if (mpShader != nullptr)
		{
			mpShader->updatePixelConstBuffer("DiffuseColor", &mConstBuffer.mDiffuseColor);
			mpShader->updatePixelConstBuffer("AmbientColor", &mConstBuffer.mAmbientColor);
			auto item = mTextures.begin();
			for (size_t i = 0; i < mTextures.size(); ++i)
			{
				(*item)->commitToGpu(pDevice, i);
				++item;
			}
		}
	}
}

