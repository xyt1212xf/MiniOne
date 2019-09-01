#pragma once
#include "Actor.h"
namespace mini
{
	class CDiffuseActor : public CActor
	{
	public:
		CDiffuseActor();
		virtual ~CDiffuseActor();

	private:
		virtual void updateFrame();
		virtual bool initlalise(const std::string& file, const std::string& materilaName) override;
	};
}
