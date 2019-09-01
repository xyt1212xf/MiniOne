#pragma once
#include "EngineHead.h"
#include "ResourceBase.h"
#include "Shader.h"
namespace mini
{
	class CPostRender : public CResourceBase
	{
	public:
		CPostRender();
		virtual ~CPostRender();

	private:
		CShader*	mpShader;
	};
}
