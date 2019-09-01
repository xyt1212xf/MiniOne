#include "pch.h"
#include "RenderToTextureActor.h"
#include "ResourceManager.h"
#include "ScriptPlugin.h"
namespace mini
{
	CRenderToTextureActor::CRenderToTextureActor()
	{
		mType = RenderToTexture;
		mpShader = nullptr;
		mShaderName = "";
	}

	CRenderToTextureActor::~CRenderToTextureActor()
	{
		for (auto item : mRendererActors)
		{
			SafeRelease(item);
		}
		SafeRelease(mpShader);
	}

	bool CRenderToTextureActor::initlalise(const std::string& file, const std::string& materialName)
	{
		return true;
	}

	void CRenderToTextureActor::addActor(CActor* pActor)
	{
		if (mRendererActors.find(pActor) != mRendererActors.end())
		{
			pActor->AddRef();
			mRendererActors.insert(pActor);
		}
	}

	void CRenderToTextureActor::removeActor(CActor* pActor)
	{
		auto item = mRendererActors.find(pActor);
		if (item != mRendererActors.end())
		{
			SafeRelease(pActor);
			mRendererActors.erase(item);
		}
	}

	bool CRenderToTextureActor::setShader(const std::string& shaderName)
	{
		if (shaderName.length() > 1 && shaderName != mShaderName)
		{
			CResourceBase* pShder = CResourceManager::getSinglePtr()->createResource(shaderName, Shader);
			return setShader(dynamic_cast<CShader*>(pShder));
		}
		return false;
	}

	bool CRenderToTextureActor::setShader(CShader* pShader)
	{
		if (mpShader != pShader)
		{
			SafeRelease(mpShader);
			mpShader = pShader;
			pShader->AddRef();
			return true;
		}
		return false;
	}

}
