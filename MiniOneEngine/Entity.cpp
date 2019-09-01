#include "pch.h"
#include "Entity.h"
#include <stdlib.h>
namespace mini
{
	static const std::string EntityName[MaxType] =
	{
		"Level_",
		"Actor_",
		"Charactor_",
		"SkyLight_",
		"DirectionLight_",
		"PlayStart_",
		"Camera_",
		"Script_",
		"Light_",
		"Particle_",
		"Model_",
		"CollisionOBBBox_",
		"CollisionAABBBox_",
		"EmptyEntity_",
	};

	CEntity::CEntity()
	{
		static UINT32 uuid = 0;
		mUUID = uuid++;
		mStrName = "";
		mType = EmptyEntity;
		mbUpdate = true;
		mbActive = true;
		mbShow = true;
	}


	CEntity::~CEntity()
	{
	}
	void CEntity::updateFrame()
	{

	}

	UINT32 CEntity::getUUID()
	{
		return mUUID;
	}

	const std::string& CEntity::getName() const
	{
		if (mStrName.length() == 0)
		{
			char temp[32];
			_ultoa_s(mUUID, temp, 10);
			mStrName = EntityName[mType];
			mStrName.append(temp);
		}
		return mStrName;
	}

	void CEntity::setName(const std::string& strName)
	{
		mStrName = strName;
	}


	void CEntity::update()
	{

	}

	std::list<CUnit*>& CEntity::getUnits()
	{
		return mpUnits;
	}

	EntityType CEntity::getEntityType() const
	{
		return mType;
	}

	void CEntity::enable()
	{
		mbActive = true;
	}

	void CEntity::disable()
	{
		mbActive = false;
	}

	bool CEntity::isActive()
	{
		return mbActive;
	}

	bool CEntity::isVisual()
	{
		return mbShow;
	}

	void CEntity::updateUnitProperty(Property* pProperty)
	{

	}
#ifdef  EDITOR
	void CEntity::dirtyProperty()
	{
		mbUpdate = true;
	}
#endif


	void CEntity::show()
	{
		mbShow = true;
	}

	void CEntity::hide()
	{
		mbShow = false;
	}

}
