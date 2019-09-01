#pragma once
#include "Actor.h"
namespace mini
{
	class CCharacter : public CActor
	{
	public:
		CCharacter();
		virtual ~CCharacter();

	private:
		virtual void updateFrame();
		virtual bool initlalise(const std::string& file, const std::string& materilaName)override;
	};
}
