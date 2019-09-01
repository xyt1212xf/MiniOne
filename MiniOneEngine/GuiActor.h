#pragma once
#include "Actor.h"
#include "UnitDerive.h"
namespace mini
{
	class CGuiActor : public CActor
	{
	public:
		CGuiActor();
		virtual ~CGuiActor();

	private:
		virtual void updateFrame();
		virtual bool initlalise(const std::string& file, const std::string& materialName)override;

	private:
		CWndUnit* mpWndUnit;
	};
}

