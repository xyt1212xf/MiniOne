#include "pch.h"
#include "ScriptPlugin.h"
#include "Engine.h"
namespace mini
{
	CScriptPlugin::CScriptPlugin()
		: mpLua(nullptr)
		, mpLuaBind(nullptr)
	{

	}

	CScriptPlugin::~CScriptPlugin()
	{
	}

	bool CScriptPlugin::onStart()
	{
		return true;
	}

	void CScriptPlugin::onRun()
	{

	}

	void CScriptPlugin::onShutDown()
	{
		SafeDelete(mpLuaBind);
		if (mpLua)
		{
			lua_close(mpLua);
			mpLua = nullptr;
		}
	}

	lua_State* CScriptPlugin::getLuaState()
	{
		return mpLua;
	}

	std::map<std::string, int>& CScriptPlugin::getGraphicDeviceConfig()
	{
		return mGraphicDeviceConfig;
	}

#include "test.h"

	bool CScriptPlugin::initialize()
	{
		mpLuaBind = CLuaBind::initialize();
		if (nullptr == mpLuaBind)
		{
			return false;
		}
		mpLua = luaL_newstate();
		luaL_openlibs(mpLua);
		mpLuaBind->setLusState(mpLua);
		CLuaBind::init();

		CLuaBind::doFile("config.lua");

		graphicConfig();
		plugInConfig();
#if 0
		//test c++ of class register to lua
		std::string content = "test = CTest:new()\n "
			"test:setUser(10)";
		CLuaBind::doString(content);
#endif
		return true;
	}

	void CScriptPlugin::plugInConfig()
	{
#define Plugin(type) \
	lua_getglobal(mpLua, "Plugin"); \
	lua_pushstring(mpLua, #type); \
	lua_gettable(mpLua, -2); \
	iResult = (int)lua_tonumber(mpLua, -1); \
	if (iResult != 0) \
	{\
		mNeedInitialisePlugin.push_back(PluginType::type);\
		GEngine->addPlugin(PluginType::type); \
	}

		int iTop = lua_gettop(mpLua);
		int iResult = 0;
		Plugin(RENDERER);
		Plugin(NETWORK);
		Plugin(GUI);

		lua_settop(mpLua, iTop);

	}

	void CScriptPlugin::graphicConfig()
	{
#ifdef WIN32 
#define GRAPHIC(type) \
		lua_getglobal(mpLua, "DX11"); \
		lua_pushstring(mpLua, #type); \
		lua_gettable(mpLua, -2); \
		iResult = (int)lua_tonumber(mpLua, -1); \
		mGraphicDeviceConfig[#type] = iResult; 
#endif 

		int iTop = lua_gettop(mpLua);
		int iResult = 0;

		GRAPHIC(fullScreen);
		GRAPHIC(vsync);
		GRAPHIC(deviceWidth);
		GRAPHIC(deviceHeight);
		GRAPHIC(cameraNear);
		GRAPHIC(cameraFar);
		GRAPHIC(fps);

		lua_settop(mpLua, iTop);
	}

}
