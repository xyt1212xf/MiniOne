#pragma once
#include "Actor.h"
namespace mini
{
	class CSkyDome : public CActor
	{
		struct GradientBufferType
		{
			CPoint4F	apexColor;
			CPoint4F	centerColor;
		};

	public:
		CSkyDome();
		virtual ~CSkyDome();
		virtual std::pair<bool, float> isPick(const CVector& v, bool isPickBound = true) override;

		void setColor(CPoint4F apex, CPoint4F center);
		void setApexColor(CPoint4F apex);
		void setCenterColor(CPoint4F center);

	private:
		virtual void initialiseFinished()override;

	private:
		std::vector<VertexFormatVTN>	mVertices;
		std::vector<UINT32>				mIndices;
		GradientBufferType				mColor;

		INT32 m_vertexCount = 0;
		INT32 m_indexCount = 0;
	};
}