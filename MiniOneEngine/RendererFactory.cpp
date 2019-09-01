#include "pch.h"
#include "RendererFactory.h"
#include "GuiRenderer.h"
#include "ShadowRenderer.h"
#include "FrontRenderer.h"
#include "DeferredRenderer.h"
#include "PostRenderer.h"
namespace mini
{
	CRendererFactory::CRendererFactory()
	{
	}


	CRendererFactory::~CRendererFactory()
	{
	}

	CRenderer* CRendererFactory::createRenderer(RendererType type)
	{
		CRenderer* pRenderer = nullptr;
		switch (type)
		{
		case RendererType::GuiRenderer:
			pRenderer = new ui::CGuiRenderer(GEngine->getDevice());
			break;
		case RendererType::ShadowRenderer:
			pRenderer = new CShadowRenderer(GEngine->getDevice());
			break;
		case RendererType::DeferredRenderer:
			pRenderer = new CDeferredRenderer(GEngine->getDevice());
			break;
		case RendererType::FrontRenderer:
			pRenderer = new CFrontRenderer(GEngine->getDevice());
			break;
		case RendererType::PostRenderer:
			pRenderer = new CPostRenderer(GEngine->getDevice());
			break;
		}
		return pRenderer;
	}
}
