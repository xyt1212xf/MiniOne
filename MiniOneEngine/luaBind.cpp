#include "pch.h"
#include "luaBind.h"
#include "ScriptPlugin.h"
namespace mini
{
	CLuaBind::CLuaBind()
	{

	}

	CLuaBind::~CLuaBind()
	{

	}

	void CLuaBind::initCommonFunction()
	{
		static bool isRun = true;
		if (isRun)
		{
			//这个函数只会在注册类的时候被调用，所以不会影响运行效率，但是对启动稍微有一点点的影响
			const char* content = "function newindex( t, k, v )\n"
				"	 local a = 3\n"
				"  print(a)\n"
				"end\n"
				""
				"function SetValue( t, k, v )\n"
				"  t[k] = v\n"
				"end\n"
				"_LUAC = {}\n"
				"function GlobalMessageMap(wndId)\n"
				"	_LUAC.messageMap = _LUAC.messageMap or {}\n"
				"	_LUAC.messageMap[wndId] = _LUAC.messageMap[wndId] or {}\n"
				"	return 1\n"
				"end\n";
			doString(content);
			isRun = false;
		}
	}


	bool CLuaBind::doString(const std::string& content)
	{
		if (!content.empty())
		{
			luaL_dostring(CLuaBind::getSinglePtr()->mpLua, content.c_str());
			return true;
		}
		return false;
	}

	bool CLuaBind::doFile(const std::string& filename, const std::string& entryMain)
	{
		lua_State* plua = CLuaBind::getSinglePtr()->mpLua;
		int bRet = luaL_loadfile(plua, filename.c_str());
		if (bRet)
		{
			return false;
		}
		bRet = lua_pcall(plua, 0, 0, 0);
		if (entryMain.length() > 0)
		{
			lua_getglobal(plua, entryMain.c_str());
			bRet = lua_pcall(plua, 0, 0, 0);
		}

		if (bRet)
		{
			int t = lua_type(plua, -1);
			const char* err = lua_tostring(plua, -1);
			lua_pop(plua, 1);
			return false;
		}
		return true;
	}

	void CLuaBind::setLusState(lua_State* plua)
	{
		mpLua = plua;
	}

	void CLuaBind::bindClass(CToScriptClass* pScriptClass)
	{
		mScriptClass.push_back(pScriptClass);
	}

	void CLuaBind::registerScriptClass()
	{
		for (auto item : mScriptClass)
		{
			item->ScriptClass();
			item->RegisterMember();
		}
	}

	void CLuaBind::init()
	{
		getSinglePtr()->initCommonFunction();
		getSinglePtr()->registerScriptClass();
	}

}
