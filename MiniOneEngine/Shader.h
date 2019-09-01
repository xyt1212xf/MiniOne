#pragma once
#include "Enginehead.h"
#include "ResourceBase.h"
#include "Device.h"
namespace mini
{
	class CShader : public CResourceBase
	{
		friend class CMaterial;
	public:
		CShader();
		virtual ~CShader();
		void setDevice(CDevice* pDevice);
		bool createVextexShader(const std::string& fileName, const std::string& strMain, const std::string& strVersion, D3D11_INPUT_ELEMENT_DESC* pInputElement = nullptr, UINT32 uNum = 0);
		bool createPixelShader(const std::string& fileName, const std::string& strMain, const std::string& strVersion);
		bool createVertexConstBuffer(INT32 nSize, UINT32 uStartSlot);
		bool createPixelConstBuffer(INT32 nSize);
		void commitToGpu();
		bool updateVertexConstBuffer(void* pData, UINT32 uStartSlot = 0);
		bool updatePixelConstBuffer(const std::string& name, void* pData);
		bool addPSParam( const std::string& name, INT32 size, INT32 nRegister);
		bool addVSParam( const std::string& name, INT32 size, INT32 nRegister);

	private:
		UINT32	mPixelConstBufferStride = 0;
		UINT32	mVertexConstBufferStride = 0;
		CDevice*						mpDevice;
		size_t							mVertexConstBufferSize;
		size_t							mPixelConstBufferSize;
#ifdef WIN32
		ID3D11VertexShader*						m_vertexShader;
		ID3D11PixelShader*						m_pixelShader;
		ID3D11InputLayout*						m_layout;
		std::map<UINT32, ID3D11Buffer*>			mVertexConstBuffers;
		std::map<std::string, std::tuple<UINT32, UINT32, ID3D11Buffer*>>	mPixelParamMap;
		ID3D11Buffer*					mPixelConstBuffer;
#else

#endif
	};
}