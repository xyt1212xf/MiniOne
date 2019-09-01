#pragma once
#include "ScriptPlugin.h"
#include "Refcount.h"
#include "luaBind.h"
namespace mini
{
	class CTest : public CToScriptClass, public CRefcount
	{
	public:
		CTest();
		virtual ~CTest();
		void setUser(int a);
		int getUser();

	private:
		int m;
		DeclareScriptClass(CTest);
	};
}
