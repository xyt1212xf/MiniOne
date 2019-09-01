#include "pch.h"
#include "Refcount.h"
namespace mini
{
	CRefcount::CRefcount()
		: mCount(1)
	{

	}

	CRefcount::~CRefcount()
	{

	}

	void CRefcount::AddRef()
	{
		++mCount;
	}

	void CRefcount::Release()
	{
		if (mCount == 0)
		{
		}
		if (--mCount == 0)
		{
			delete this;
		}
	}

	unsigned short CRefcount::getRef()
	{
		return mCount;
	}
}

