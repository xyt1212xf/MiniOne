#include "pch.h"
#include "ResourceBase.h"
#include "ResourceManager.h"

namespace mini
{
	CResourceBase::CResourceBase()
	{
		static UINT32 uuid = 0;
		mResourceUUID = uuid++;
		mResourceName = "";
		mResourceType = ResourceBase;
	}


	CResourceBase::~CResourceBase()
	{
		if (CResourceManager::getSinglePtr())
		{
			CResourceManager::getSinglePtr()->clearResource(this);
		}
	}

	UINT32 CResourceBase::getResourceID() const
	{
		return mResourceUUID;
	}

	std::string& CResourceBase::getResourceName()
	{
		return mResourceName;
	}

	void CResourceBase::setResourceName(std::string strName)
	{
		mResourceName = strName;
	}

	bool CResourceBase::initialise()
	{
		return false;
	}

	ResType CResourceBase::getResourceType()
	{
		return mResourceType;
	}
}
