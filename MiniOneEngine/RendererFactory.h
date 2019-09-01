#pragma once
#include "EngineHead.h"
#include "Renderer.h"
namespace mini
{
	class CRendererFactory : public TSingle<CRendererFactory>
	{
	public:
		CRendererFactory();
		virtual ~CRendererFactory();
		CRenderer*	createRenderer(RendererType type);

	};
}
