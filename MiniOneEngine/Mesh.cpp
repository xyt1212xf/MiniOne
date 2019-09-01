#include "pch.h"
#include "Mesh.h"
#include "RendererPlugin.h"
namespace mini
{
	CMesh::CMesh()
	{
		mVertexCount = 0;
		mIndexCount = 0;
		mpVertexBuffer = nullptr;
		mpIndexBuffer = nullptr;
		mResourceType = Mesh;
	}


	CMesh::~CMesh()
	{
		SafeRelease(mpVertexBuffer);
		SafeRelease(mpIndexBuffer);
	}

	bool CMesh::initialise()
	{
		return true;
	}

	const CGPUBuffer* CMesh::getVertexBuffer() const
	{
		return mpVertexBuffer;
	}

	const CGPUBuffer* CMesh::getIndexBuffer() const
	{
		return mpIndexBuffer;
	}

	void CMesh::fillVertexBuffer(void* pData, size_t length, UINT8 typeSize)
	{
		try
		{
			if (nullptr == pData || 0 == length)
			{
				throw 0;
			}

			D3D11_BUFFER_DESC vertexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData;
			mVertexSize = typeSize;

			mVertexCount = length / mVertexSize;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = mVertexSize * mVertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			vertexData.pSysMem = pData;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;
			CDevice* pDevice = CRendererPlugin::getSinglePtr()->getDevice();
			HRESULT result = pDevice->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &mpVertexBuffer);
			if (FAILED(result))
			{
				throw 1;
			}
			mVertices.resize(mVertexCount);
			memcpy(&mVertices[0], pData, length);
		}
		catch (...)
		{

		}
	}

	void CMesh::fillIndexBuffer(void* pData, size_t length, UINT8 typeSize)
	{
		try
		{
			if (nullptr == pData || 0 == length)
			{
				throw 0;
			}
			mIndexCount = length / typeSize;
			D3D11_BUFFER_DESC indexBufferDesc;
			D3D11_SUBRESOURCE_DATA indexData;

			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = typeSize * mIndexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			indexData.pSysMem = pData;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			CDevice* pDevice = CRendererPlugin::getSinglePtr()->getDevice();
			HRESULT result = pDevice->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &mpIndexBuffer);
			if (FAILED(result))
			{
				throw 1;
			}
			mIndices.resize(mIndexCount);
			memcpy(&mIndices[0], pData, length);
		}
		catch (...)
		{
		}
	}

#ifdef WIN32
	VertexFormatVTNTB* CMesh::getVertices()
	{
		if (mVertexSize == sizeof(VertexFormatVTNTB))
		{
			return &mVertices[0];
		}
		else
		{
			return nullptr;
		}
	}

	VertexFormatVTNTBSkin * CMesh::getVerticesSkin()
	{
		if (mVertexSize == sizeof(VertexFormatVTNTBSkin))
		{
			return &mVerticesSkin[0];
		}
		else
		{
			return nullptr;
		}
	}
#else
	VertexFormatVTN* CMesh::getVertices()
	{
		return &mpVertices[0];
	}
#endif
	UINT32* CMesh::getIndices()
	{
		return &mIndices[0];
	}

	UINT32 CMesh::getFaceNumber()
	{
		return mIndices.size() / 3;
	}

}

