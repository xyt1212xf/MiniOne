#include "pch.h"
#include "test.h"
namespace mini
{
	DefineScriptClass(CTest)
		BeginRegisterMember(CTest)
		REGISTER_CLASS_MEMFUN1(CTest, void, int, "setUser", setUser);
	REGISTER_CLASS_MEMFUN0(CTest, int, "getUser", getUser);
	EndRegisterMember()
		CTest::CTest()
	{

	}

	CTest::~CTest()
	{

	}

	void CTest::setUser(int a)
	{
		m = a;
	}

	int CTest::getUser()
	{
		return m;
	}
}
