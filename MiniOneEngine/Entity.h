#pragma once
#include "EngineHead.h"
#include "Unit.h"

namespace mini
{
	//z这里的设计需要在仔细考虑,modifty at 2019.3.6
	//这个的顺序不要改变，对应的是Actor的图标
	enum EntityType
	{
		Level,
		Actor,
		Charactor,
		SkyLight,
		DirectionLight,
		PlayStart,
		Camera,
		Script,
		Light,
		Particle,
		Model,
		Gui,
		Terrain,
		SkyDome,
		RenderToTexture,
		CollisionOBBBox,
		CollisionAABBBox,
		PosetRender,
		EmptyEntity,
#ifdef EDITOR
		Triangle,
		TriangleTexture,
		Diffuse,
		CubeMesh,
#endif
		MaxType,
	};


	//////////////////////////////////////////////////////////////////////////
	class CEntity : public CRefcount
	{
	public:
		CEntity();
		virtual ~CEntity();
		virtual void updateFrame();
		virtual void updateUnitProperty(Property* pProperty);
#ifdef  EDITOR
		virtual void dirtyProperty();
#endif //  Editor
		UINT32				getUUID();
		const std::string&	getName() const;
		void				setName(const std::string& strName);
		std::list<CUnit*>&	getUnits();
		EntityType			getEntityType() const;
		void				enable();
		void				disable();
		void				show();
		void				hide();
		bool				isActive();
		bool				isVisual();

	private:
		void				update();

	protected:
		std::list<CUnit*>	mpUnits;
		EntityType			mType;
		bool				mbUpdate;
		bool				mbActive;
		bool				mbShow;

	private:
		UINT32		mUUID;
	//	UINT32		mFlag;
		mutable std::string	mStrName;
	};
}