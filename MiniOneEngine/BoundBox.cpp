#include "pch.h"
#include "BoundBox.h"

namespace mini
{
	CBoundBox::CBoundBox()
	{
		mbDirty = true;
		mPt = CVector(0, 0, 0);
		mRadius = CVector(0, 0, 0);
	}


	CBoundBox::~CBoundBox()
	{
	}

	void CBoundBox::setPosition(const CVector& pt)
	{
		mPt = mOriginalPt + pt;
		for (auto& i : mBoundBoxPit)
		{
			i.x += pt.x;
			i.y += pt.y;
			i.z += pt.z;
		}
		mbDirty = true;
	}

	void CBoundBox::setScale(const CVector& scale)
	{
		mRadius.x = mOriginalRadius.x * scale.x;
		mRadius.y = mOriginalRadius.y * scale.y;
		mRadius.z = mOriginalRadius.z * scale.z;
		update();
		mbDirty = true;
	}

	void CBoundBox::setRadius(const CVector& radiuse)
	{
		mRadius = radiuse;
		mbDirty = true;
	}

	CVector& CBoundBox::getCenterPt()
	{
		return mPt;
	}

	CVector& CBoundBox::getRadius()
	{
		return mRadius;
	}

	void CBoundBox::update()
	{
	//	if (mbDirty)
		{
			CVector minPt = mPt + mRadius;
			CVector maxPt = mPt - mRadius;
			mBoundBoxPit[0].x = minPt.x;
			mBoundBoxPit[0].y = maxPt.y;
			mBoundBoxPit[0].z = minPt.z;

			mBoundBoxPit[1].x = maxPt.x;
			mBoundBoxPit[1].y = maxPt.y;
			mBoundBoxPit[1].z = minPt.z;

			mBoundBoxPit[2].x = maxPt.x;
			mBoundBoxPit[2].y = minPt.y;
			mBoundBoxPit[2].z = minPt.z;

			mBoundBoxPit[3].x = minPt.x;
			mBoundBoxPit[3].y = minPt.y;
			mBoundBoxPit[3].z = minPt.z;

			mBoundBoxPit[4].x = minPt.x;
			mBoundBoxPit[4].y = maxPt.y;
			mBoundBoxPit[4].z = maxPt.z;

			mBoundBoxPit[5].x = maxPt.x;
			mBoundBoxPit[5].y = maxPt.y;
			mBoundBoxPit[5].z = maxPt.z;

			mBoundBoxPit[6].x = maxPt.x;
			mBoundBoxPit[6].y = minPt.y;
			mBoundBoxPit[6].z = maxPt.z;

			mBoundBoxPit[7].x = minPt.x;
			mBoundBoxPit[7].y = minPt.y;
			mBoundBoxPit[7].z = maxPt.z;
		}
	}
}
