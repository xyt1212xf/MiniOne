#pragma once
#include "EngineHead.h"
namespace mini
{
	class CRefcount
	{
	public:
		CRefcount();
		virtual ~CRefcount();
		virtual	void AddRef();
		virtual void Release();
		unsigned short getRef();

	protected:
		unsigned short mCount;
	};
}
