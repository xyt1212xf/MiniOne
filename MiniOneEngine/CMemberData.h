#pragma once
#include "ScriptHead.h"
namespace mini
{
	template<typename T>
	class CCMemberData
	{
		template< typename T> friend class CClass;
	public:
		CCMemberData(const std::string& Name, UINT32 Type, UINT32 Offset)
		{
			mName = Name;
			mOffset = Offset;
			mType = (ETypeEnum)Type;
		}

	private:
		T* mpObject;
		std::string		mName;
		UINT32			mOffset;
		ETypeEnum		mType;
	};

}
