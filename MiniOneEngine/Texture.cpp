#include "pch.h"
#include "Texture.h"
#include "RendererPlugin.h"
namespace mini
{
	CTexture::CTexture()
		: m_ShaderView(nullptr)
		, m_sampleState(nullptr)
	{
		mResourceType = Texture;
		ZeroMemory(&mDesc, sizeof(mDesc));
	}


	CTexture::~CTexture()
	{
		SafeRelease(m_ShaderView);
		SafeRelease(mpTexture2D);
		SafeRelease(m_sampleState);
	}

	bool CTexture::initialiseSampler(D3D11_SAMPLER_DESC* pSampler)
	{
		// Load the texture in.
		CDevice* pDevice = CRendererPlugin::getSinglePtr()->getDevice();

		// Create the texture sampler state.
		HRESULT result = pDevice->getDevice()->CreateSamplerState(pSampler, &m_sampleState);
		if (FAILED(result))
		{
			return false;
		}
		if (m_ShaderView == nullptr)
		{
			initialise();
		}
		return true;
	}


	std::vector<UINT8> CTexture::saveAlpha(INT32 x, INT32 y, INT32 width, INT32 height)
	{
		std::vector<UINT8> alpha;
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		CDevice* pDevice = CRendererPlugin::getSinglePtr()->getDevice();
		D3D11_TEXTURE2D_DESC description;
		mpTexture2D->GetDesc(&description);
		description.BindFlags = 0;
		description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		description.Usage = D3D11_USAGE_STAGING;
		UINT32 Width = width;
		UINT32 Height = height;
		UINT32 X = x * 4;
		UINT32 Y = y + 4;
		ID3D11Texture2D* texTemp = NULL;
		HRESULT hr = pDevice->getDevice()->CreateTexture2D(&description, NULL, &texTemp);
		if (S_OK == hr)
		{
			pDevice->getDeviceContext()->CopyResource(texTemp, mpTexture2D);
			hr = pDevice->getDeviceContext()->Map(texTemp, 0, D3D11_MAP_READ, 0, &MappedResource);
			byte* pRow = (byte*)MappedResource.pData;
			int offset = 0;
			for (UINT32 i = y; i < Height; ++i)
			{
				for (UINT32 j = x; j < Width; ++j)
				{
					alpha.push_back(pRow[offset + j * 4 + 3]);
				}
				offset += MappedResource.RowPitch;
			}
		}
		else
		{
			texTemp->Release();
			texTemp = NULL;
		}
		pDevice->getDeviceContext()->Unmap(texTemp, 0);
		SafeRelease(texTemp);
		return alpha;
	}

	std::vector<UINT8> CTexture::saveAlpha()
	{
		return saveAlpha(0, 0, mDesc.Width, mDesc.Height);
	}

	bool CTexture::initialise()
	{
		if (mResourceName.length() > 1)
		{
			// Load the texture in.
			CDevice* pDevice = CRendererPlugin::getSinglePtr()->getDevice();
			HRESULT result = D3DX11CreateShaderResourceViewFromFile(pDevice->getDevice(),
				mResourceName.c_str(), NULL, NULL, &m_ShaderView, NULL);
			if (FAILED(result))
			{
				return false;
			}

			ID3D11Resource* pTexture2D = NULL;
			ID3D11ShaderResourceView *pSRView = NULL;
			D3DX11_IMAGE_LOAD_INFO imageInfo;
			result = D3DX11CreateTextureFromFile(pDevice->getDevice(), mResourceName.c_str(), nullptr, NULL, &pTexture2D, NULL);
			if (FAILED(result))
			{
				return false;
			}
			mpTexture2D = (ID3D11Texture2D*)pTexture2D;
			mpTexture2D->GetDesc(&mDesc);

			return true;
		}
		else
		{
			return false;
		}
	}

	void CTexture::commitToGpu(CDevice* pDevice, INT32 startSlot)
	{
		ID3D11DeviceContext* pDeviceContext = pDevice->getDeviceContext();
		if (m_ShaderView != nullptr)
		{
			pDeviceContext->PSSetShaderResources(startSlot, 1, &m_ShaderView);
		}
		if (m_sampleState != nullptr)
		{
			pDeviceContext->PSSetSamplers(0, 1, &m_sampleState);
		}
	}

	UINT32 CTexture::getImageWidth()
	{
		return mDesc.Width;
	}

	UINT32 CTexture::getImageHeight()
	{
		return mDesc.Height;
	}
}
