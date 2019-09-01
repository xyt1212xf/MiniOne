#include "pch.h"
#include "Shader.h"
#include "DevicePlatform.h"
#include "RendererPlugin.h"
namespace mini
{
	CShader::CShader()
		: mpDevice(nullptr)
		, m_vertexShader(nullptr)
		, m_pixelShader(nullptr)
		, m_layout(nullptr)
		, mPixelConstBuffer(nullptr)
		, mVertexConstBufferSize(0)
		, mPixelConstBufferSize(0)
	{
		mResourceType = Shader;
	}

	CShader::~CShader()
	{
		for (auto item : mVertexConstBuffers)
		{
			SafeRelease(item.second);
		}
		SafeRelease(mPixelConstBuffer);
		SafeRelease(m_vertexShader);
		SafeRelease(m_pixelShader);
		SafeRelease(m_layout);
	}

	void CShader::setDevice(CDevice* pDevice)
	{
		mpDevice = pDevice;
	}

	bool CShader::createVextexShader(const std::string& fileName, const std::string& strMain, const std::string& strVersion, D3D11_INPUT_ELEMENT_DESC* pInputElement, UINT32 uNum)
	{
		ID3D10Blob* errorMessage = nullptr;
		ID3D10Blob* vertexShaderBuffer = nullptr;
		// Compile the vertex shader code.
		HRESULT result = D3DX11CompileFromFile(fileName.c_str(), NULL, NULL, strMain.c_str(), strVersion.c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&vertexShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
#ifdef WIN32
			// If the shader failed to compile it should have writen something to the error message.	
			MessageBox(mpDevice->getHWND(), (char*)(errorMessage->GetBufferPointer()), "Missing Shader File", MB_OK);
#endif
			return false;
		}
		// Create the vertex shader from the buffer.
		result = mpDevice->getDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
		if (FAILED(result))
		{
			return false;
		}
		if (pInputElement != nullptr)
		{
			// Create the vertex input layout.
			result = mpDevice->getDevice()->CreateInputLayout(pInputElement, uNum, vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), &m_layout);
			if (FAILED(result))
			{
				return false;
			}
		}
		SafeRelease(vertexShaderBuffer);

		return true;
	}

	bool CShader::createPixelShader(const std::string& fileName, const std::string& strMain, const std::string& strVersion)
	{
		ID3D10Blob* pixelShaderBuffer = nullptr;
		ID3D10Blob* errorMessage = nullptr;

		// Compile the pixel shader code.
		HRESULT result = D3DX11CompileFromFile(fileName.c_str(), NULL, NULL, strMain.c_str(), strVersion.c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&pixelShaderBuffer, &errorMessage, NULL);
		if (FAILED(result))
		{
#ifdef WIN32
			MessageBox(mpDevice->getHWND(), (char*)(errorMessage->GetBufferPointer()), "Missing Shader File", MB_OK);
#endif // WIN32
			return false;
		}
		// Create the pixel shader from the buffer.
		result = mpDevice->getDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
		if (FAILED(result))
		{
			return false;
		}
		SafeRelease(pixelShaderBuffer);
		return true;
	}

	bool CShader::createVertexConstBuffer(INT32 nSize, UINT32 uStartSlot)
	{
		if (nSize != 0 && nullptr == mVertexConstBuffers[uStartSlot])
		{
			// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
			D3D11_BUFFER_DESC matrixBufferDesc;
			matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			matrixBufferDesc.ByteWidth = nSize;
			matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			matrixBufferDesc.MiscFlags = 0;
			matrixBufferDesc.StructureByteStride = 0;

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			HRESULT result = mpDevice->getDevice()->CreateBuffer(&matrixBufferDesc, NULL, &mVertexConstBuffers[uStartSlot]);
			if (FAILED(result))
			{
				return false;
			}
			mVertexConstBufferSize = nSize;
		}
		return true;
	}

	bool CShader::createPixelConstBuffer(INT32 nSize)
	{
		if (nSize != 0)
		{
			// Setup the description of the light dynamic constant buffer that is in the pixel shader.
			// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
			D3D11_BUFFER_DESC pixelBufferDesc;
			pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			pixelBufferDesc.ByteWidth = nSize;
			pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pixelBufferDesc.MiscFlags = 0;
			pixelBufferDesc.StructureByteStride = 0;

			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			HRESULT result = mpDevice->getDevice()->CreateBuffer(&pixelBufferDesc, NULL, &mPixelConstBuffer);
			if (FAILED(result))
			{
				return false;
			}
			mPixelConstBufferSize = nSize;
		}
		return true;
	}

	void CShader::commitToGpu()
	{
		CRendererPlugin* pRendererPlugin = CRendererPlugin::getSinglePtr();

		ID3D11DeviceContext* pDeviceContext = pRendererPlugin->getDevice()->getDeviceContext();
		if (nullptr != m_layout)
		{
			pDeviceContext->IASetInputLayout(m_layout);
		}
		// Set the vertex and pixel shaders that will be used to render this triangle.
		pDeviceContext->VSSetShader(m_vertexShader, NULL, 0);
		pDeviceContext->PSSetShader(m_pixelShader, NULL, 0);
	}

	bool CShader::updateVertexConstBuffer(void* pData, UINT32 uStartSlot)
	{
		if (mVertexConstBufferSize > 0)
		{
			ID3D11DeviceContext* pContext = mpDevice->getDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			HRESULT result = pContext->Map(mVertexConstBuffers[uStartSlot], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			memcpy(mappedResource.pData, pData, mVertexConstBufferSize);

			pContext->Unmap(mVertexConstBuffers[uStartSlot], 0);
			// Finanly set the constant buffer in the vertex shader with the updated values.
			pContext->VSSetConstantBuffers(uStartSlot, 1, &mVertexConstBuffers[uStartSlot]);
			return true;
		}
		else
		{
			return true;
		}
	}

	bool CShader::updatePixelConstBuffer(const std::string& name, void* pDadta)
	{
		auto& item = mPixelParamMap.find(name);
		if( item != mPixelParamMap.end() )
		{
			ID3D11DeviceContext* pContext = mpDevice->getDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			auto& pair = item->second;
			
			HRESULT result = pContext->Map(std::get<2>(pair), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			memcpy(mappedResource.pData, pDadta, std::get<1>(pair));

			// Unlock the constant buffer.
			pContext->Unmap(std::get<2>(pair), 0);

			pContext->PSSetConstantBuffers(std::get<0>(pair), 1, &std::get<2>(pair));
		}
		return true;
	}

	bool CShader::addPSParam(const std::string& name, INT32 size, INT32 nRegister)
	{
		if (size != 0)
		{
			// Setup the description of the light dynamic constant buffer that is in the pixel shader.
			// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
			D3D11_BUFFER_DESC pixelBufferDesc;
			pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			pixelBufferDesc.ByteWidth = size;
			pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pixelBufferDesc.MiscFlags = 0;
			pixelBufferDesc.StructureByteStride = 0;
			ID3D11Buffer* pixelConstBuffer = nullptr;
			// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
			HRESULT result = mpDevice->getDevice()->CreateBuffer(&pixelBufferDesc, NULL, &pixelConstBuffer);
			if (FAILED(result))
			{
				return false ;
			}
			mPixelConstBufferSize = size;
			mPixelParamMap[name] = std::tuple<UINT32, UINT32, ID3D11Buffer*>(nRegister, size, pixelConstBuffer);
			mPixelConstBufferStride += size;
		}
		return true;
	}

	bool CShader::addVSParam(const std::string& name, INT32 size, INT32 nRegister)
	{
		return true;
	}

}
