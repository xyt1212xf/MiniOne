#pragma once
#include "Actor.h"
namespace mini
{
	class CRenderToTextureActor : public CActor
	{
	public:
		CRenderToTextureActor();
		~CRenderToTextureActor();
		virtual bool initlalise(const std::string& file, const std::string& materialName) override;
		void addActor(CActor* pActor);
		void removeActor(CActor* pActor);
		bool setShader(const std::string& shaderName);
		bool setShader(CShader* pShader);

	private:
		CShader*			mpShader;
		std::set<CActor*>	mRendererActors;
		std::string			mShaderName;
	};
}
