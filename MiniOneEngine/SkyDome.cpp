#include "pch.h"
#include "skyDome.h"
namespace mini
{
	CSkyDome::CSkyDome()
	{
		// Set the color at the top of the sky dome.
		mColor.apexColor = CPoint4F(0.0f, 0.15f, 0.66f, 1.0f);

		// Set the color at the center of the sky dome.
		mColor.centerColor = CPoint4F(0.81f, 0.38f, 0.66f, 1.0f);
	}

	CSkyDome::~CSkyDome()
	{

	}

	std::pair<bool, float> CSkyDome::isPick(const CVector& v, bool isPickBound /*= true*/)
	{
		return std::pair<bool, float>(false, 0.0f);
	}

	void CSkyDome::setColor(CPoint4F apex, CPoint4F center)
	{
		setApexColor(apex);
		setCenterColor(center);
		if (nullptr != mpMeshUnit)
		{
			auto materialUnit = mpMeshUnit->getMaterialUnit();
			materialUnit->updatePSConstBuffer("GradientBuffer", &mColor);
		}
	}

	void CSkyDome::setApexColor(CPoint4F apex)
	{
		mColor.apexColor = apex;
	}

	void CSkyDome::setCenterColor(CPoint4F center)
	{
		mColor.centerColor = center;
	}

	void CSkyDome::initialiseFinished()
	{
		setColor(mColor.apexColor, mColor.centerColor);
	}
}