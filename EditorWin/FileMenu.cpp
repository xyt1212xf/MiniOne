#include "stdafx.h"
#include "FileMenu.h"
#include "Engine.h"
#include "Terrain.h"
#include "RenderNode.h"
bool CFileMenu::wndProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	switch (wmId)
	{
	case ID_FILE_NEWSCENE:
	{
		mini::GEngine->stopRender();
		{
			/*
			mini::CRenderNode* pRenderNode = mini::GEngine->createRenderNode();
	
			mini::GEngine->createTerrain("Resource/Texture/heightmap.bmp");
			mini::CEntity* pTerrain = mini::GEngine->getTerrainActor();
			pRenderNode->bindActor(dynamic_cast<mini::CActor*>(pTerrain), mini::RendererType::FrontRenderer);
			mini::GEngine->addRenderNodeToScene(pRenderNode);
			SafeRelease(pTerrain);
			SafeRelease(pRenderNode);
			*/
		}
		{
			mini::CRenderNode* pRenderNode = mini::GEngine->createRenderNode();
			mini::CActor* pSkyDome = mini::GEngine->createActor("Resource/Model/skydome.txt", "Resource/Material/skyDomeMaterial.ma", mini::EntityType::SkyDome);
			pRenderNode->bindActor(pSkyDome, mini::RendererType::FrontRenderer);
			mini::GEngine->addRenderNodeToScene(pRenderNode);
			pSkyDome->setScale(CVector(1, 1, 1));
			SafeRelease(pSkyDome);
			SafeRelease(pRenderNode);
		}
		mini::GEngine->recoverRender();
	}
	return true;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		return true;
	}

	return false;
}

