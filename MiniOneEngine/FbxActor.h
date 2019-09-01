#pragma once
#include "Actor.h"
namespace mini
{
	class CFbxActor : public CActor
	{
	public:
		CFbxActor();
		virtual ~CFbxActor();

	private:
		virtual void updateFrame();
		virtual bool initlalise(const std::string& file, const std::string& materilaName) override;
	};
}
