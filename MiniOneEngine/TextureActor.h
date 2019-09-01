#pragma once
#include "Actor.h"
namespace mini
{
	class CTextureActor : public CActor
	{
	public:
		CTextureActor();
		virtual ~CTextureActor();

	private:
		virtual bool initlalise(const std::string& file, const std::string& materilaName)override;

	};
}


