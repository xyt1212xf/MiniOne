#include "pch.h"
#include "Skeleton.h"
namespace mini
{
	CSkeleton::CSkeleton()
	{

	}

	CSkeleton::~CSkeleton()
	{

	}

	void CSkeleton::addBone(Bone& bone)
	{
		INT64 parentId = bone.nParentIndex;
		bone.nIndex = mBones.size();
		if (parentId != bone.nIndex)
		{
			mBonesLayer[parentId].push_back(bone.nIndex);
		}
		mBones.push_back(bone);
	}

	void CSkeleton::initSkin()
	{

	}

	void CSkeleton::update(float time)
	{

	}
}
