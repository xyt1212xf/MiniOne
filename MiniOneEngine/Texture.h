#pragma once
#include "EngineHead.h"
#include "ResourceBase.h"
#include "Device.h"
namespace mini
{
	class CTexture : public CResourceBase
	{
		friend class CResourceManager;
	public:
		CTexture();
		virtual ~CTexture();
		virtual bool initialiseSampler(D3D11_SAMPLER_DESC* pSampler);
		virtual bool initialise() override;
		void commitToGpu(CDevice* pDevice, INT32 startSlot);
		std::vector<UINT8>	saveAlpha(INT32 x, INT32 y, INT32 width, INT32 height);
		std::vector<UINT8>	saveAlpha();

		UINT32 getImageWidth();
		UINT32 getImageHeight();

	private:
		D3D11_TEXTURE2D_DESC mDesc;
#ifdef WIN32
		ID3D11ShaderResourceView* m_ShaderView = nullptr;
		ID3D11Texture2D* mpTexture2D = nullptr;
		ID3D11SamplerState*	m_sampleState;
#endif
	};


}
