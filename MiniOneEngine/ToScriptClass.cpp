#include "pch.h"
#include "ToScriptClass.h"
namespace mini
{
	CToScriptClass::CToScriptClass()
	{
		mLuaObjectHandle = 0;
	}

	CToScriptClass::~CToScriptClass()
	{

	}

	void CToScriptClass::RegisterMember()
	{

	}

	void CToScriptClass::ScriptClass()
	{

	}

	void CToScriptClass::RegisterHandle()
	{
		static unsigned int handle = 0;
		mLuaObjectHandle = ++handle;
	}

	unsigned int CToScriptClass::GetRegisterHandle()
	{
		return mLuaObjectHandle;
	}
}
