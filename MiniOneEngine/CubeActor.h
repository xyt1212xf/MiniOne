#pragma once
#include "Actor.h"
namespace mini
{
	class CCubeActor : public CActor
	{
	public:
		CCubeActor();
		virtual ~CCubeActor();

	private:
		virtual void updateFrame();
		virtual bool initlalise(const std::string& file, const std::string& materilaName) override;
	};
}
