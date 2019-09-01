#pragma once
#include "Device.h"
#include "plugin.h"
#include "EngineHead.h"
#include "ScriptPlugin.h"
#include "Renderer.h"
#include "Scene.h"
#include "RenderNode.h"
namespace mini
{
	extern void renderer(void *lp);

	class CRendererPlugin : public CPlugin, public TSingle<CRendererPlugin>
	{
		friend class CPluginFactory;
		friend void renderer(void *lp);

	public:
		virtual ~CRendererPlugin();
		virtual bool onStart();
		virtual void onRun();
		virtual void onShutDown();
		CRenderer*	addRenderer(RendererType type);
		void	removeRenderer(RendererType type);
		CDevice* getDevice();
		CRenderNode *createRender();

		void	setScene(CScene* pScene);
		void	setWorldMatrix(CMatrix* pMatrix);
		void	setViewMatrix(CMatrix* pMatrix);
		void	setProjectMatrix(CMatrix* pMatrix);
		void	setCamera(CCamera* pCamera);
		void	addRenderNode(CRenderNode* pRenderNode);
		void	removeRenderNode(CRenderNode* pRenderNode);

		void	activeRenderer(UINT32 uflag);
		void	disableRenderer(UINT32 uflag);

		void	activeRendererEditor(UINT32 uflag);
		void	disableRendererEditor(UINT32 uflag);

		const CMatrix* const getWorldMatrix();
		const CMatrix* const getViewMatrix();

		void	stopRender();
		void	recoverRender();
		std::vector<CMatrix>&	getTransposeMatrices();

	private:
		CRendererPlugin(CScriptPlugin* pPlugin);
		virtual bool initialize();
		void	internalRenderer();
		bool	isExistRenderer(RendererType type);
		void	flushToGpu();

	private:
		std::atomic_bool 	mbOverRenderer;
		std::atomic_bool	mbRender;
		std::atomic_bool	mbRunning;

		CDevice*			mpDevice;
		CScriptPlugin*		mpPlugin;
		CScene*				mpScene;
		std::mutex			mMutex;
		UINT32				mLockCount = 0;   
		RendererType		mCurrentSceneRenderType;
		std::vector<CMatrix*>	mbuildGpuMatrxis;
		std::vector<CMatrix>		mbindMatrixsTranspose;

		CMatrix				mIdentityMatrix;
		CMatrix*				mpProjectMatrix;
		CMatrix*				mpViewMatrix;
		CMatrix*				mpWorldMatrix;
		std::map<UINT32, CRenderer*> mRenderers;
		std::set<CRenderNode*>		mRenderNodes;
		bool					mbUpdateTranspose;

	};
#define GRendererPlugin CRendererPlugin::getSinglePtr()
}
