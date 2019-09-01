#include "pch.h"
#include "DeviceDx11.h"
namespace mini
{
	CDeviceDx11::CDeviceDx11()
		: mpDeviceConfig(nullptr)
	{
		m_CurrentSwapChain = nullptr;
		m_device = nullptr;
		m_deviceContext = nullptr;
		m_renderTargetView = nullptr;
		m_depthStencilBuffer = nullptr;
		m_depthStencilState = nullptr;
		m_depthStencilStateNoDepth = nullptr;
		m_depthStencilView = nullptr;
		m_rasterState = nullptr;
	}


	CDeviceDx11::~CDeviceDx11()
	{
		shutDownDevice();
	}

	bool CDeviceDx11::initDevice(DeviceConfig& config)
	{
		mpDeviceConfig = &config;
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator, denominator;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* backBufferPtr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_VIEWPORT viewport;
		float fieldOfView, screenAspect;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)config.screenWidth)
			{
				if (displayModeList[i].Height == (unsigned int)config.screenHeight)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			return false;
		}

		// Store the dedicated video card memory in megabytes.
		config.videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		size_t stringLength;

		char videoCardInfo[128];
		error = wcstombs_s(&stringLength, videoCardInfo, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}
		config.videoCardInfo = videoCardInfo;

		// Release the display mode list.
		SafeDeleteArray(displayModeList);

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = nullptr;

		// Release the adapter.
		adapter->Release();
		adapter = nullptr;

		// Release the factory.
		factory->Release();
		factory = nullptr;

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = config.screenWidth;
		swapChainDesc.BufferDesc.Height = config.screenHeight;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if (config.vsync)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = config.hWnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;


		// Set to full screen or windowed mode.
		swapChainDesc.Windowed = !config.fullScreen;

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		UINT32 creationFlags = 0;
#ifdef DEBUG
		creationFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_CurrentSwapChain, &m_device, NULL, &m_deviceContext);
		if (FAILED(result))
		{
			return false;
		}
		// Get the pointer to the back buffer.
		result = m_CurrentSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
		{
			return false;
		}

		mMultipleWindowSwapChain[config.hWnd] = m_CurrentSwapChain;

		// Create the render target view with the back buffer pointer.
		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
		if (FAILED(result))
		{
			return false;
		}
		mRenderTargetViews[config.hWnd] = m_renderTargetView;
		// Release pointer to the back buffer as we no longer need it.
		SafeRelease(backBufferPtr);

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = config.screenWidth;
		depthBufferDesc.Height = config.screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
		if (FAILED(result))
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////
		//建立一个不写深度的模板，但是最终都要修改，不在这里来处理,暂时先这么处理，等shaderEffect出来之后，删除这里，DX11不熟悉的原因，只熟悉DX9
		depthStencilDesc.DepthEnable = false;
		result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStateNoDepth);
		if (FAILED(result))
		{
			return false;
		}

		D3D11_BLEND_DESC blendStateDescription;
		// Clear the blend state description.
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		// Create an alpha enabled blend state description.
		blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		// Create the blend state using the description.
		result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
		if (FAILED(result))
		{
			return false;
		}

		// Modify the description to create an alpha disabled blend state description.
		blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

		// Create the blend state using the description.
		result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
		if (FAILED(result))
		{
			return false;
		}

		//////////////////////////////////////////////////////////////////////////

		// Set the depth stencil state.
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = true;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
		if (FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		m_deviceContext->RSSetState(m_rasterState);

		// Setup a raster description which turns off back face culling.
		rasterDesc.CullMode = D3D11_CULL_FRONT;

		// Create the no culling rasterizer state.
		result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateFrontCulling);
		if (FAILED(result))
		{
			return false;
		}


		// Setup the viewport for rendering.
		viewport.Width = (float)config.screenWidth;
		viewport.Height = (float)config.screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		m_deviceContext->RSSetViewports(1, &viewport);
		
		// Create the ScissorRects;
		RECT rects = { 0, 0, viewport.Width, viewport.Height };
		m_deviceContext->RSSetScissorRects(1, &rects);

		mRenderTargetViewports[config.hWnd] = viewport;
		// Setup the projection matrix.
		fieldOfView = (float)D3DX_PI / 4.0f;
		screenAspect = (float)config.screenWidth / (float)config.screenHeight;
		return true;
	}

	void CDeviceDx11::beginScene()
	{
		static float color[4] = { 0,0,0,1.0f };
		// Clear the back buffer.
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

		// Clear the depth buffer.
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void CDeviceDx11::endinScene()
	{
		if (mpDeviceConfig->vsync)
		{
			m_CurrentSwapChain->Present(1, 0);
		}
		else
		{
			m_CurrentSwapChain->Present(0, 0);
		}
	}

	void CDeviceDx11::setProjectMatrix(CMatrix& matrix, int index)
	{
		mProjectionMatrix[index] = matrix;
	}

	void CDeviceDx11::setWorldMatrix(CMatrix& matrix, int index)
	{
		mWorldMatrix[index] = matrix;
	}

	void CDeviceDx11::setViewMatrix(CMatrix& matrix, int index)
	{
		mViewMatrix[index] = matrix;
	}

	void CDeviceDx11::makeProjectMatrix(CMatrix& matrix)
	{

	}

	void CDeviceDx11::makeWorldMatrix(CMatrix& matrix)
	{

	}

	void CDeviceDx11::makeViewMatrix(CMatrix& matrix)
	{

	}

	bool CDeviceDx11::createSwapChain(HWND hWnd)
	{
		auto item = mMultipleWindowSwapChain.find(hWnd);
		if (item != mMultipleWindowSwapChain.end())
		{
			return false;
		}
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(hWnd, &rc);

		UINT width = mpDeviceConfig->screenWidth;
		UINT height = mpDeviceConfig->screenHeight;
		//UINT width = rc.right - rc.left;
		//UINT height = rc.bottom - rc.top;
		// Obtain DXGI factory from device (since we used nullptr for pAdapter above) 
		IDXGIFactory* dxgiFactory = nullptr;
		IDXGIDevice* dxgiDevice = nullptr;
		hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
		if (FAILED(hr))
		{
			return false;
		}
		DXGI_SWAP_CHAIN_DESC swapChainDesc;

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = mpDeviceConfig->screenWidth;;
		swapChainDesc.BufferDesc.Height = mpDeviceConfig->screenHeight;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if (mpDeviceConfig->vsync)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = hWnd;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;


		// Set to full screen or windowed mode.
		swapChainDesc.Windowed = !mpDeviceConfig->fullScreen;

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		hr = dxgiFactory->CreateSwapChain(m_device, &swapChainDesc, &mMultipleWindowSwapChain[hWnd]);
		dxgiFactory->Release();

		if (FAILED(hr))
		{
			return false;
		}
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = mMultipleWindowSwapChain[hWnd]->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
		if (FAILED(hr))
		{
			return false;
		}
		hr = m_device->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetViews[hWnd]);
		pBackBuffer->Release();
		if (FAILED(hr))
		{
			return false;
		}
		D3D11_VIEWPORT viewport;
		m_renderTargetView = mRenderTargetViews[hWnd];
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		mRenderTargetViewports[hWnd] = viewport;
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		//m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		return true;
	}

	void CDeviceDx11::changeSwapChain(HWND hWnd)
	{
		auto itemSwapChain = mMultipleWindowSwapChain.find(hWnd);
		if (itemSwapChain != mMultipleWindowSwapChain.end())
		{
			m_CurrentSwapChain = itemSwapChain->second;
		}
		auto itemRenderTargetWnd = mRenderTargetViews.find(hWnd);
		if (itemRenderTargetWnd != mRenderTargetViews.end())
		{
			m_renderTargetView = itemRenderTargetWnd->second;
		}
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		auto itemRenderTargetViewport = mRenderTargetViewports.find(hWnd);
		if (itemRenderTargetViewport != mRenderTargetViewports.end())
		{
			m_deviceContext->RSSetViewports(1, &(itemRenderTargetViewport->second));
		}
	}

	void CDeviceDx11::setDepth(bool value, UINT32 stencilRef /* stencilRef = 1 */)
	{
		if (value)
		{
			// Set the depth stencil state.
			m_deviceContext->OMSetDepthStencilState(m_depthStencilState, stencilRef);
		}
		else
		{
			// Set the depth stencil state.
			m_deviceContext->OMSetDepthStencilState(m_depthStencilStateNoDepth, stencilRef);
		}
	}

	void CDeviceDx11::turnOnAlphaBlending()
	{
		// Setup the blend factor.
		float blendFactor[4] = { 0, 0, 0, 0 };

		// Turn on the alpha blending.
		m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
	}


	void CDeviceDx11::turnOffAlphaBlending()
	{	
		// Setup the blend factor.
		float blendFactor[4] = { 0, 0, 0, 0 };

		// Turn off the alpha blending.
		m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
	}

	void CDeviceDx11::TurnOnCulling()
	{
		m_deviceContext->RSSetState(m_rasterState);
	}

	void CDeviceDx11::TurnOffCulling()
	{
		m_deviceContext->RSSetState(m_rasterStateFrontCulling);
	}

	GriphaDevice* CDeviceDx11::getDevice() const
	{
		return m_device;
	}

	ID3D11DeviceContext* CDeviceDx11::getDeviceContext() const
	{
		return m_deviceContext;
	}

	HWND CDeviceDx11::getHWND()
	{
		return mpDeviceConfig->hWnd;
	}

	void CDeviceDx11::shutDownDevice()
	{
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		if (mpDeviceConfig->fullScreen && m_CurrentSwapChain)
		{
			m_CurrentSwapChain->SetFullscreenState(false, NULL);
			m_CurrentSwapChain = nullptr;
		}

		if (m_rasterState)
		{
			m_rasterState->Release();
			m_rasterState = 0;
		}

		if (m_depthStencilView)
		{
			m_depthStencilView->Release();
			m_depthStencilView = 0;
		}

		SafeRelease(m_depthStencilState);
		SafeRelease(m_depthStencilStateNoDepth);

		SafeRelease(m_alphaEnableBlendingState);
		SafeRelease(m_alphaDisableBlendingState);

		SafeRelease(m_rasterState);
		SafeRelease(m_rasterStateFrontCulling);

		if (m_depthStencilBuffer)
		{
			m_depthStencilBuffer->Release();
			m_depthStencilBuffer = 0;
		}

		if (m_renderTargetView)
		{
			m_renderTargetView = 0;
		}

		if (m_deviceContext)
		{
			m_deviceContext->Release();
			m_deviceContext = 0;
		}

		for (auto item : mMultipleWindowSwapChain)
		{
			(item.second)->Release();
		}
		mMultipleWindowSwapChain.clear();

		for (auto item : mRenderTargetViews)
		{
			(item.second)->Release();
		}
		mRenderTargetViews.clear();

		if (m_device)
		{
			m_device->Release();
			m_device = 0;
		}
	}
}
