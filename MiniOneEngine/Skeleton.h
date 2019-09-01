#pragma once
#include "EngineHead.h"
#include "Device.h"
#include "Skin.h"
namespace mini
{
	struct Bone
	{
		std::string boneName;
		INT64		uniqueID;
		INT64		nParentIndex;
		INT64		nIndex;

		//VertexAtTimeT = VertexFromControlPoint * kMatFromBoneSpaceToWorldSpace * KeyFrameMatrixAtTimeT
		CMatrix neSpaceToModelSpace;
	};

	class CSkeleton
	{
	public:
		CSkeleton();
		virtual ~CSkeleton();
		void addBone(Bone& bone);
		void initSkin();
		void update(float time);

	private:
		std::vector<Bone>	mBones;
		std::map<INT64, std::list<INT64> >	mBonesLayer;
		CSkin	mSkin;
	};

}


