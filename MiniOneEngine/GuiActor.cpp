#include "pch.h"
#include "GuiActor.h"

namespace mini
{
	CGuiActor::CGuiActor()
		: mpWndUnit(nullptr)
	{
		mType = EntityType::Gui;
	}


	CGuiActor::~CGuiActor()
	{
	}

	void CGuiActor::updateFrame()
	{

	}

	bool CGuiActor::initlalise(const std::string& file, const std::string& materialName)
	{
		CActor::initlalise(file, materialName);
		mpWndUnit = new CWndUnit();
		if (file.length() > 0)
		{
			mpWndUnit->setUnitFile(file);
		}
		else
		{
			mpWndUnit->setUnitFile("resource/Gui/DefaultWnd.gui");
		}
		mpWndUnit->onInitialise();
		mpUnits.push_back(mpWndUnit);
		return true;
	}
}
