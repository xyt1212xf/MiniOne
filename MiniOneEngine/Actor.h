#pragma once

#include "Entity.h"
#include "Vector.h"
#include "UnitDerive.h"
#include "BoundBox.h"
namespace mini
{
	class CActor : public CEntity
	{
		friend class CScene;
	public:
		CActor();
		virtual ~CActor();
		virtual bool initlalise(const std::string& fileName, const std::string& materilaName);
		virtual void updateUnitProperty(Property* pProperty);
		virtual std::pair<bool, float> isPick(const CVector& v, bool isPickBound = true);
		
		void setPosition(CVector pt);
		void setScale(CVector pt);
		void setRotation(CVector pt);
		void bindMeshUnit( CMeshUnit* pMeshUnit );

		CVector& getPosition();
		CVector& getScale();
		CVector& getRotation();
		CBoundBox&	getBoundBox();

	private:
		void update();
		void createMaterial(const std::string& materialName)noexcept;

	protected:
		virtual void updateFrame();
		virtual void initialiseFinished();

	protected:
		CVector					mPosition;
		CVector					mScale;
		CVector					mRotation;
		CMeshUnit*				mpMeshUnit = nullptr;
	};
}



