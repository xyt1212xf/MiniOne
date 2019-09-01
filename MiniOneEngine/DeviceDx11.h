#pragma once
#include "DevicePlatform.h"
#include "EngineHead.h"

#include "Matrix.h"
#include "Vector.h"
#define  GriphaDevice ID3D11Device
namespace mini
{
	class CDeviceDx11
	{
	public:
		CDeviceDx11();
		virtual ~CDeviceDx11();
		virtual bool initDevice(DeviceConfig& config);
		void shutDownDevice();
		void beginScene();
		void endinScene();
		void setProjectMatrix(CMatrix& matrix, int index = 0);
		void setWorldMatrix(CMatrix& matrix, int index = 0);
		void setViewMatrix(CMatrix& matrix, int index);
		void makeProjectMatrix(CMatrix& matrix);
		void makeWorldMatrix(CMatrix& matrix);
		void makeViewMatrix(CMatrix& matrix);
		bool createSwapChain(HWND hWnd);
		void changeSwapChain(HWND hWnd);
		void setDepth(bool value, UINT32 stencilRef = 1);
		void turnOnAlphaBlending();
		void turnOffAlphaBlending();
		void TurnOnCulling();
		void TurnOffCulling();

		GriphaDevice* getDevice()const;
		ID3D11DeviceContext* getDeviceContext()const;

		HWND getHWND();
	private:
		DeviceConfig*		mpDeviceConfig;
		IDXGISwapChain* m_CurrentSwapChain;
		std::map<HWND, IDXGISwapChain*>	mMultipleWindowSwapChain;
		std::map<HWND, ID3D11RenderTargetView*>	mRenderTargetViews;
		std::map<HWND, D3D11_VIEWPORT> mRenderTargetViewports;
		GriphaDevice* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;

		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilState* m_depthStencilStateNoDepth;
		ID3D11BlendState* m_alphaEnableBlendingState = nullptr;
		ID3D11BlendState* m_alphaDisableBlendingState = nullptr;
		
		ID3D11RasterizerState* m_rasterState = nullptr;
		ID3D11RasterizerState* m_rasterStateFrontCulling = nullptr;

		ID3D11DepthStencilView* m_depthStencilView;

		CMatrix mWorldMatrix[10];
		CMatrix mViewMatrix[2];
		CMatrix mProjectionMatrix[2];
	};

}
