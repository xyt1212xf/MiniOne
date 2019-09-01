#pragma once
#include "Actor.h"
namespace mini
{
	class CTriangleActor : public CActor
	{
	public:
		CTriangleActor();
		virtual ~CTriangleActor();

	private:
		virtual bool initlalise(const std::string& file, const std::string& materilaName)override;

	};

}

