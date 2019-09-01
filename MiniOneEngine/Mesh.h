#pragma once
#include "EngineHead.h"
#include "ResourceBase.h"
#include "GPUBuffer.h"
#include "FoundationStruct.h"
namespace mini
{
	class CMesh : public CResourceBase
	{
		friend class CMeshUnit;
		friend class CResourceManager;
	public:
		CMesh();
		virtual ~CMesh();
		virtual bool initialise() override;

		const CGPUBuffer* getVertexBuffer() const;
		const CGPUBuffer* getIndexBuffer() const;
		void fillVertexBuffer(void* pData, size_t length, UINT8 typeSize);
		void fillIndexBuffer(void* pData, size_t length, UINT8 typeSize);

	private:
#ifdef WIN32
		VertexFormatVTNTB* getVertices();
		VertexFormatVTNTBSkin* getVerticesSkin();
#else
		VertexFormatVTN* getVertices();
#endif // WIN32
		UINT32* getIndices();
		UINT32 getFaceNumber();

	private:
		std::vector<std::list<UINT32> >		mSkinInfo;

#ifdef WIN32
		std::vector<VertexFormatVTNTB>		mVertices;
		std::vector<VertexFormatVTNTBSkin>	mVerticesSkin;
#else
		std::vector<VertexFormatVTN>		mVertices;
		std::vector<VertexFormatVTNSkin>	mVerticesSkin;
#endif
		std::vector<UINT32> mIndices;
		CGPUBuffer*	mpVertexBuffer;
		CGPUBuffer*	mpIndexBuffer;
		UINT32 mVertexCount;
		UINT32 mIndexCount;
		UINT32 mVertexSize;
	};
}
