#pragma once
#include "EngineHead.h"
#include "Vector.h"
namespace mini
{
	class CBoundBox
	{
	public:
		CBoundBox();
		~CBoundBox();
		void setPosition(const CVector& pt);
		void setScale(const CVector& scale);
		void setRadius(const CVector& radius);
		template<typename T>
		void calculation(T* pData, size_t size);
		CVector& getCenterPt();
		CVector& getRadius();

	private:
		void update();

	private:
		CVector	mPt;
		CVector	mRadius;
		CVector mBoundBoxPit[8];

		CVector	mOriginalPt;
		CVector mOriginalRadius;
		CVector mOriginalBoundBoxPit[8];
		float	mRadiusLength = 0;
		bool mbDirty;
	};

	template<typename T>
	void CBoundBox::calculation(T* pData, size_t size)
	{
#define compare( index, value ) \
		if (pData[index].position.value < min.value)\
		{\
			min.value = pData[index].position.value; \
		}\
		if (pData[index].position.value > max.value)\
		{\
			max.value = pData[index].position.value; \
		}

		CVector min = { 0, 0, 0 };
		CVector max = { 0, 0, 0 };
		
		for (size_t i = 0; i < size; ++i)
		{
			compare(i, x);
			compare(i, y);
			compare(i, z);
		}
		mOriginalPt = (min + max) * 0.5f;
		mOriginalRadius = max;
		mRadius = mOriginalRadius;
		mPt = mOriginalPt;
		mRadiusLength = D3DXVec3Length(&max);
		update();
		memcpy(mOriginalBoundBoxPit, mBoundBoxPit, sizeof(mBoundBoxPit));
	}
}
