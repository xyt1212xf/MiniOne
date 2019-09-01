#pragma once
#include "Renderer.h"
namespace mini
{
	namespace ui
	{
		class CGuiRenderer : public CRenderer
		{
			struct renderGroup
			{
				CTexture* pTexture;
				UINT32		begin;
				UINT32		count;
				union
				{
					INT32		needScissor = -1;
					RECT		scissorRt;
				};
			};
			friend class CGuiPlugin;
			friend class CRendererPlugin;
		public:
			CGuiRenderer(CDevice* pDevice);
			virtual ~CGuiRenderer();

		private:
			void _render(CWnd* pWnd, UINT32 layer);
			virtual void render();
			virtual void clearAllUnit();
			virtual void commitToGpu();
			void fillRenderBuffer(CWnd* pWnd);
			bool initShaderWithGpuBuffer();
			bool initSamplrState();
			VertexFormatVTC* applyVertexBuffer(INT32 nCount);
			void drawBuffer(ID3D11DeviceContext* pDeviceContext);
			void drawLineBuffer(ID3D11DeviceContext* pDeviceContext);

		private:
			INT32			mFrameHalfWidth;
			INT32			mFrameHalfHeight;
			ID3D11Buffer *m_vertexBuffer;
			ID3D11Buffer *m_indexBuffer;
			ID3D11DepthStencilState* m_depthStencilState;
			ID3D11SamplerState*	m_sampleState = nullptr;
			std::list<ui::CWnd*>*		mpWnds = nullptr;
			CShader*		mpShader = nullptr;
			INT32			mVertexCount;
			INT32			mFreeStartPoint;
			CMatrix			mWorldMatrix;
			std::vector<VertexFormatVTC>	m_Verteices;
			std::list<std::tuple<GuiStyle*, INT32>>	mRenderInfo;
			std::map<UINT32, std::list<CWnd*>> mRenderWnds;
			std::list<renderGroup> mRenderGroups;
#ifdef EDITOR
			bool			mbDrawLine;
			CShader*		mpShaderLine;
			INT32		mDrawLinePointCount;
			ID3D11Buffer *m_linesBuffer;
			std::vector<VertexFormatVC>	m_lines;
#endif
		};
	}
}
