#pragma once
#include "Engine.h"
#include "ScriptHead.h"
#include "ToScriptClass.h"
#include "ParseParam.h"
#include "CMemberData.h"
#include "Foundation.h"

#define OFFSET( Class, var ) (UINT32)&(((Class*)0)->var)

namespace mini
{
	class CLuaBind : public TSingle<CLuaBind>
	{
	public:
		CLuaBind();
		~CLuaBind();
		static void	initCommonFunction();
		static bool	doString(const std::string& content);
		static bool doFile(const std::string& filename, const std::string& entryMain = "");
		void setLusState(lua_State* plua);
		void bindClass(CToScriptClass* pScriptClass);
		void registerScriptClass();
		static void init();

	public:
		lua_State*	mpLua;

	private:
		std::list<CToScriptClass*>	mScriptClass;

	};

	static void CreateMessageMap(lua_State* L, UINT32 handle)
	{
		char index[32];
		_ultoa_s(handle, index, 10);
		std::string runString = "local tempWndTable = _LUAC[";
		runString.append(index);
		runString += "]\n";

		runString += ""
			"function tempWndTable:binding(id, msgType, func)\n"
			"	self:setSize(500, 500);	\n"
			"end";

		//	CLuaBind::doString(runString.c_str());
	}

	//建立元表
	static int CreateMetatable(const char* Name, lua_State* L)
	{
		luaL_getmetatable(L, Name);
		if (lua_isnil(L, -1))
		{
			luaL_newmetatable(L, Name);
			lua_pushstring(L, "__index");
			lua_pushvalue(L, -2);
			lua_settable(L, -3);
			return 1;
		}
		return 0;
	}

	static void RegisterLuaBuildObject(lua_State* L, UINT32 handle)
	{
		lua_getglobal(L, "_LUAC");
		lua_pushnumber(L, handle);
		lua_pushvalue(L, -3);
		lua_settable(L, -3);
		lua_pop(L, 1);
		int t = lua_gettop(L);
		lua_getglobal(L, "GlobalMessageMap");     //函数名
		lua_pushnumber(L, handle);
		int bRet = lua_pcall(L, 1, 1, 0);
		if (bRet)
		{
			int t = lua_type(L, -1);
			const char* err = lua_tostring(L, -1);
			lua_pop(L, 1);
			return;
		}
		lua_settop(L, t);
		CreateMessageMap(L, handle);
	}


#include "ScriptPlugin.h"

	//类成员函数
	template<typename Ret>
	class CMemCall
	{
	public:
		template< typename Cla, Ret(Cla::*fun)() >
		static int doCall(lua_State* L)
		{
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			Ret ret = ((*ppobjct)->*(fun))();
			ParseRet(L, ret);
			return 1;
		}

		template< typename P1, typename Cla, Ret(Cla::*fun)(P1) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			Ret ret = ((*ppobjct)->*(fun))(p1);
			ParseRet(L, ret);
			return 1;
		}

		template< typename P1, typename P2, typename Cla, Ret(Cla::*fun)(P1, P2) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -2);
			P2 p2 = CParseParam<P2>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			Ret ret = ((*ppobjct)->*(fun))(p1, p2);
			ParseRet(L, ret);
			return 1;
		}

		template< typename P1, typename P2, typename P3, typename Cla, Ret(Cla::*fun)(P1, P2, P3) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -3);
			P2 p2 = CParseParam<P2>::ParseParam(L, -2);
			P3 p3 = CParseParam<P3>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			Ret ret = ((*ppobjct)->*(fun))(p1, p2, p3);
			ParseRet(L, ret);
			return 1;
		}
		template< typename P1, typename P2, typename P3, typename P4, typename Cla, Ret(Cla::*fun)(P1, P2, P3, P4) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -4);
			P2 p2 = CParseParam<P2>::ParseParam(L, -3);
			P3 p3 = CParseParam<P3>::ParseParam(L, -2);
			P4 p4 = CParseParam<P4>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			Ret ret = ((*ppobjct)->*(fun))(p1, p2, p3, p4);
			ParseRet(L, ret);
			return 1;
		}
		template< typename P1, typename P2, typename P3, typename P4, typename P5, typename Cla, Ret(Cla::*fun)(P1, P2, P3, P4, P5) >
		static int doCall(lua_State* L)
		{

			P1 p1 = CParseParam<P1>::ParseParam(L, -5);
			P2 p2 = CParseParam<P2>::ParseParam(L, -4);
			P3 p3 = CParseParam<P3>::ParseParam(L, -3);
			P4 p4 = CParseParam<P4>::ParseParam(L, -2);
			P5 p5 = CParseParam<P5>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			Ret ret = ((*ppobjct)->*(fun))(p1, p2, p3, p4, p5);
			ParseRet(L, ret);
			return 1;
		}
		template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename Cla, Ret(Cla::*fun)(P1, P2, P3, P4, P5, P6) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -6);
			P2 p2 = CParseParam<P2>::ParseParam(L, -5);
			P3 p3 = CParseParam<P3>::ParseParam(L, -4);
			P4 p4 = CParseParam<P4>::ParseParam(L, -3);
			P5 p5 = CParseParam<P5>::ParseParam(L, -2);
			P6 p6 = CParseParam<P6>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			Ret ret = ((*ppobjct)->*(fun))(p1, p2, p3, p4, p5, p6);
			ParseRet(L, ret);
			return 1;
		}
	};

	template<>
	class CMemCall< void >
	{
	public:
		template< typename Cla, void (Cla::*fun)() >
		static int doCall(lua_State* L)
		{
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			((*ppobjct)->*(fun))();
			return 1;
		}
		template< typename P1, typename Cla, void (Cla::*fun)(P1) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			((*ppobjct)->*(fun))(p1);
			return 1;
		}

		template< typename P1, typename P2, typename Cla, void (Cla::*fun)(P1, P2) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -2);
			P2 p2 = CParseParam<P2>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			((*ppobjct)->*(fun))(p1, p2);
			return 1;
		}

		template< typename P1, typename P2, typename P3, typename Cla, void (Cla::*fun)(P1, P2, P3) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -3);
			P2 p2 = CParseParam<P2>::ParseParam(L, -2);
			P3 p3 = CParseParam<P3>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			((*ppobjct)->*(fun))(p1, p2, p3);
			return 1;
		}
		template< typename P1, typename P2, typename P3, typename P4, typename Cla, void (Cla::*fun)(P1, P2, P3, P4) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -4);
			P2 p2 = CParseParam<P2>::ParseParam(L, -3);
			P3 p3 = CParseParam<P3>::ParseParam(L, -2);
			P4 p4 = CParseParam<P4>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			((*ppobjct)->*(fun))(p1, p2, p3, p4);
			return 1;
		}
		template< typename P1, typename P2, typename P3, typename P4, typename P5, typename Cla, void (Cla::*fun)(P1, P2, P3, P4, P5) >
		static int doCall(lua_State* L)
		{

			P1 p1 = CParseParam<P1>::ParseParam(L, -5);
			P2 p2 = CParseParam<P2>::ParseParam(L, -4);
			P3 p3 = CParseParam<P3>::ParseParam(L, -3);
			P4 p4 = CParseParam<P4>::ParseParam(L, -2);
			P5 p5 = CParseParam<P5>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			((*ppobjct)->*(fun))(p1, p2, p3, p4, p5);
			return 1;
		}
		template< typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename Cla, void (Cla::*fun)(P1, P2, P3, P4, P5, P6) >
		static int doCall(lua_State* L)
		{
			P1 p1 = CParseParam<P1>::ParseParam(L, -6);
			P2 p2 = CParseParam<P2>::ParseParam(L, -5);
			P3 p3 = CParseParam<P3>::ParseParam(L, -4);
			P4 p4 = CParseParam<P4>::ParseParam(L, -3);
			P5 p5 = CParseParam<P5>::ParseParam(L, -2);
			P6 p6 = CParseParam<P6>::ParseParam(L, -1);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			Cla** ppobjct = (Cla**)lua_touserdata(L, -1);
			((*ppobjct)->*(fun))(p1, p2, p3, p4, p5, p6);
			return 1;
		}
	};


	template<typename T>
	class luna
	{
	public:
		//注册数据成员
		static void Reg_MemData(lua_State* L, const std::string& reg_name, UINT8 type, UINT32 offset)
		{
			_CreateMetatable(L);
			int t = lua_gettop(L);
			luaL_getmetatable(L, T::MetaName);
			std::string aa = T::MetaName;
			t = lua_gettop(L);
			lua_pushstring(L, reg_name.c_str());
			lua_pushnumber(L, offset);
			lua_settable(L, -3);
			t = lua_gettop(L);
			CCMemberData<T>* pTemp = new CCMemberData<T>(reg_name, type, offset);
			T::mCClass.addClassMemData(pTemp);
		}

		template< typename Ret, Ret(T::*fun)() >
		static void Build_CMemCall(lua_State* L, const char* funName)
		{
			int top = lua_gettop(L);
			luaL_getmetatable(L, T::MetaName);
			if (!lua_isnil(L, -1))
			{
				lua_pushstring(L, funName);
#if 0
				lua_CFunction pcall = &CMemCall<Ret>::doCall<T, fun>;
				lua_pushcfunction(L, pcall);
#else
				lua_pushcfunction(L, nullptr);
#endif
				lua_rawset(L, -3);
			}
			lua_settop(L, top);
		}

		template< typename Ret, typename P1, Ret(T::*fun)(P1) >
		static void Build_CMemCall(lua_State* L, const char* funName)
		{
			int top = lua_gettop(L);
			luaL_getmetatable(L, T::MetaName);
			if (!lua_isnil(L, -1))
			{
				lua_pushstring(L, funName);
#if 0
				lua_CFunction pcall = &CMemCall<Ret>::doCall<P1, T, fun>;
				lua_pushcfunction(L, pcall);
#else
				lua_pushcfunction(L, nullptr);
#endif
				lua_rawset(L, -3);
			}
			lua_settop(L, top);
		}

		template< typename Ret, typename P1, typename P2, Ret(T::*fun)(P1, P2) >
		static void Build_CMemCall(lua_State* L, const char* funName)
		{
			int top = lua_gettop(L);
			luaL_getmetatable(L, T::MetaName);
			if (!lua_isnil(L, -1))
			{
				lua_pushstring(L, funName);
#if 0
				lua_CFunction pcall = &CMemCall<Ret>::doCall<P1, P2, T, fun>;
				lua_pushcfunction(L, pcall);
#else
				lua_pushcfunction(L, nullptr);
#endif
				lua_rawset(L, -3);
			}
			lua_settop(L, top);
		}


		template< typename Ret, typename P1, typename P2, typename P3,
			Ret(T::*fun)(P1, P2, P3) >
			static void Build_CMemCall(lua_State* L, const char* funName)
		{
			int top = lua_gettop(L);
			luaL_getmetatable(L, T::MetaName);
			if (!lua_isnil(L, -1))
			{
				lua_pushstring(L, funName);
#if 0
				lua_CFunction pcall = &CMemCall<Ret>::doCall<P1, P2, P3, T, fun>;
				lua_pushcfunction(L, pcall);
#else
				lua_pushcfunction(L, nullptr);
#endif
				lua_rawset(L, -3);
			}
			lua_settop(L, top);
		}

		template< typename Ret, typename P1, typename P2, typename P3, typename P4,
			Ret(T::*fun)(P1, P2, P3, P4) >
			static void Build_CMemCall(lua_State* L, const char* funName)
		{
			int top = lua_gettop(L);
			luaL_getmetatable(L, T::MetaName);
			if (!lua_isnil(L, -1))
			{
				lua_pushstring(L, funName);
#if 0
				lua_CFunction pcall = &CMemCall<Ret>::doCall<P1, P2, P3, P4, T, fun>;
				lua_pushcfunction(L, pcall);
#else
				lua_pushcfunction(L, nullptr);
#endif
				lua_rawset(L, -3);
			}
			lua_settop(L, top);
		}

		template< typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5,
			Ret(T::*fun)(P1, P2, P3, P4, P5) >
			static void Build_CMemCall(lua_State* L, const char* funName)
		{
			int top = lua_gettop(L);
			luaL_getmetatable(L, T::MetaName);
			if (!lua_isnil(L, -1))
			{
				lua_pushstring(L, funName);
#if 0
				lua_CFunction pcall = &CMemCall<Ret>::doCall<P1, P2, P3, P4, P5, T, fun>;
				lua_pushcfunction(L, pcall);
#else
				lua_pushcfunction(L, nullptr);
#endif
				lua_rawset(L, -3);
			}
			lua_settop(L, top);
		}

		template< typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
			Ret(T::*fun)(P1, P2, P3, P4, P5, P6) >
			static void Build_CMemCall(lua_State* L, const char* funName)
		{
			int top = lua_gettop(L);
			luaL_getmetatable(L, T::MetaName);
			if (!lua_isnil(L, -1))
			{
				lua_pushstring(L, funName);
#if 0
				lua_CFunction pcall = &CMemCall<Ret>::doCall<P1, P2, P3, P4, P5, P6, T, fun>;
				lua_pushcfunction(L, pcall);
#else
				lua_pushcfunction(L, nullptr);
#endif
				lua_rawset(L, -3);
			}
			lua_settop(L, top);
		}

		static int Register_Class(lua_State* L)
		{
			CLuaBind::initCommonFunction();
			_CreateMetatable(L);
			lua_newtable(L);
			lua_pushstring(L, "new");
			lua_pushcfunction(L, _CreateClassObject);
			lua_settable(L, -3);

			/*
			new 和new object都调用同一个函数的原因那个是,
			在脚本如果有table覆盖了new,可以继续用newobject来建立类对象,
			当不存在lua的继承table的时候用new也可以建立出来类对象。
			*/
			lua_pushstring(L, "newobject");
			lua_pushcfunction(L, _CreateClassObject);
			lua_settable(L, -3);

			lua_setglobal(L, T::ClassName);
			return 1;
		}
	private:
		static int _CreateClassObject(lua_State* L)
		{
			lua_newtable(L);

			lua_pushstring(L, "__index");
			luaL_getmetatable(L, T::MetaName);
			lua_rawset(L, -3);

			lua_pushnumber(L, 0);
			T** pUserData = (T**)lua_newuserdata(L, sizeof(T*));
			*pUserData = new T();
			luaL_getmetatable(L, T::MetaName);
			lua_setmetatable(L, -2);
			lua_rawset(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);

			luaL_getmetatable(L, T::MetaName);
			lua_setmetatable(L, -2);

			(*pUserData)->RegisterHandle();
			RegisterLuaBuildObject(L, (*pUserData)->GetRegisterHandle());
			return 1;
		}

		static int _CreateMetatable(lua_State* L)
		{
			if (1 == CreateMetatable(T::MetaName, L))
			{
				lua_pushstring(L, "__gc");
				lua_pushcfunction(L, gc_obj);
				lua_settable(L, -3);

				lua_pushstring(L, "__tostring");
				lua_pushcfunction(L, tostring);
				lua_settable(L, -3);
			}
			return 1;
		}

		static int gc_obj(lua_State *L)
		{
			T** obj = static_cast<T**>(luaL_checkudata(L, -1, T::MetaName));
			if (foundation::CheckClass<T>())
			{
				SafeRelease((*obj));
			}
			return 1;
		}

		static int newindex(lua_State* L)
		{
			if (lua_isstring(L, -2))
			{
				UpdataCObject(L, lua_tostring(L, -2));
			}

			lua_getglobal(L, "SetValue");
			lua_pushvalue(L, -4);
			lua_pushvalue(L, -4);
			lua_pushvalue(L, -4);
			lua_pcall(L, 3, 0, 0);

			return 1;
		}

		static int tostring(lua_State* L)
		{
			lua_pushstring(L, T::ClassName);
			return 1;
		}

		static bool UpdataCObject(lua_State* L, const char* reg_name)
		{
			luaL_getmetatable(L, T::MetaName);
			lua_pushnumber(L, 0);
			lua_gettable(L, 1);
			T** ppobjct = (T**)lua_touserdata(L, -1);
			T::mCClass.UpRegMemData(*ppobjct, reg_name);
			lua_pop(L, 2);
			return true;
		}
	};

	template<typename Ret, typename Cla, Ret(Cla::*fun)() >
	void RegMemFun(const char* szFunName)
	{
		lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//	luna<Cla>::Build_CMemCall<Ret, fun>(pState, szFunName);
	}

	template<typename Ret, typename P1, typename Cla, Ret(Cla::*fun)(P1) >
	void RegMemFun(const char* szFunName)
	{
		lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//	luna<Cla>::Build_CMemCall<Ret, P1, fun>(pState, szFunName);
	}

	template<typename Ret, typename P1, typename P2, typename Cla, Ret(Cla::*fun)(P1, P2) >
	void RegMemFun(const char* szFunName)
	{
		lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//	luna<Cla>::Build_CMemCall<Ret, P1, P2, fun>(pState, szFunName);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename Cla, Ret(Cla::*fun)(P1, P2, P3) >
	void RegMemFun(const char* szFunName)
	{
		lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//	luna<Cla>::Build_CMemCall<Ret, P1, P2, P3, fun>(pState, szFunName);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4,
		typename Cla, Ret(Cla::*fun)(P1, P2, P3, P4) >
		void RegMemFun(const char* szFunName)
	{
		lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//	luna<Cla>::Build_CMemCall<Ret, P1, P2, P3, P4, fun>(pState, szFunName);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5,
		typename Cla, Ret(Cla::*fun)(P1, P2, P3, P4, P5) >
		void RegMemFun(const char* szFunName)
	{
		lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//	luna<Cla>::Build_CMemCall<Ret, P1, P2, P3, P4, P5, fun>(pState, szFunName);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
		typename Cla, Ret(Cla::*fun)(P1, P2, P3, P4, P5, P6) >
		void RegMemFun(const char* szFunName)
	{
		lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//	luna<Cla>::Build_CMemCall<Ret, P1, P2, P3, P4, P5, P6, fun>(pState, szFunName);
	}

	//注册普通的函数
	template<typename Ret>
	class CCallFun
	{
	public:
		template<typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			Ret ret;
			ret = func();

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			Ret ret;

			ret = func(p1);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);

			Ret ret = func(p1, p2);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename P3, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);

			Ret ret = func(p1, p2, p3);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);

			Ret ret = func(p1, p2, p3, p4);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);

			Ret ret = func(p1, p2, p3, p4, p5);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);

			Ret ret = func(p1, p2, p3, p4, p5, p6);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
			typename Func, Func func>
			static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);
			P7 p7 = CParseParam<P7>::ParseParam(pState, 7);

			Ret ret = func(p1, p2, p3, p4, p5, p6, p7);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
			typename P7, typename P8,
			typename Func, Func func>
			static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);
			P7 p7 = CParseParam<P7>::ParseParam(pState, 7);
			P8 p8 = CParseParam<P8>::ParseParam(pState, 8);

			Ret ret = func(p1, p2, p3, p4, p5, p6, p7, p8);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
			typename P7, typename P8, typename P9,
			typename Func, Func func>
			static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);
			P7 p7 = CParseParam<P7>::ParseParam(pState, 7);
			P8 p8 = CParseParam<P8>::ParseParam(pState, 8);
			P9 p9 = CParseParam<P9>::ParseParam(pState, 9);

			Ret ret = func(p1, p2, p3, p4, p5, p6, p7, p8, p9);

			ParseRet(pState, ret);

			return 1;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
			typename P7, typename P8, typename P9, typename P10,
			typename Func, Func func>
			static int CallFun(lua_State* pState)
		{
			P1  p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2  p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3  p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4  p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5  p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6  p6 = CParseParam<P6>::ParseParam(pState, 6);
			P7  p7 = CParseParam<P7>::ParseParam(pState, 7);
			P8  p8 = CParseParam<P8>::ParseParam(pState, 8);
			P9  p9 = CParseParam<P9>::ParseParam(pState, 9);
			P10 p10 = CParseParam<P10>::ParseParam(pState, 10);

			Ret ret = func(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);

			ParseRet(pState, ret);

			return 1;
		}
	};


	template<>
	class CCallFun<void>
	{
	public:
		template<typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			func();
			return 0;
		}

		template<typename P1, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			//P1 p1 = CParseParam<P1>ParseParam(pState, 1);

			//func(p1);

			return 0;
		}

		template<typename P1, typename P2, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);

			func(p1, p2);

			return 0;
		}

		template<typename P1, typename P2, typename P3, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);

			func(p1, p2, p3);

			return 0;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);

			func(p1, p2, p3, p4);

			return 0;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);

			func(p1, p2, p3, p4, p5);

			return 0;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename Func, Func func>
		static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);

			func(p1, p2, p3, p4, p5, p6);

			return 0;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
			typename P7,
			typename Func, Func func>
			static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);
			P7 p7 = CParseParam<P7>::ParseParam(pState, 7);

			func(p1, p2, p3, p4, p5, p6, p7);

			return 0;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
			typename P7, typename P8,
			typename Func, Func func>
			static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);
			P7 p7 = CParseParam<P7>::ParseParam(pState, 7);
			P8 p8 = CParseParam<P8>::ParseParam(pState, 8);

			func(p1, p2, p3, p4, p5, p6, p7, p8);

			return 0;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
			typename P7, typename P8, typename P9,
			typename Func, Func func>
			static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);
			P7 p7 = CParseParam<P7>::ParseParam(pState, 7);
			P8 p8 = CParseParam<P8>::ParseParam(pState, 8);
			P9 p9 = CParseParam<P9>::ParseParam(pState, 9);

			func(p1, p2, p3, p4, p5, p6, p7, p8, p9);

			return 0;
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
			typename P7, typename P8, typename P9, typename P10,
			typename Func, Func func>
			static int CallFun(lua_State* pState)
		{
			P1 p1 = CParseParam<P1>::ParseParam(pState, 1);
			P2 p2 = CParseParam<P2>::ParseParam(pState, 2);
			P3 p3 = CParseParam<P3>::ParseParam(pState, 3);
			P4 p4 = CParseParam<P4>::ParseParam(pState, 4);
			P5 p5 = CParseParam<P5>::ParseParam(pState, 5);
			P6 p6 = CParseParam<P6>::ParseParam(pState, 6);
			P7 p7 = CParseParam<P7>::ParseParam(pState, 7);
			P8 p8 = CParseParam<P8>::ParseParam(pState, 8);
			P9 p9 = CParseParam<P9>::ParseParam(pState, 9);
			P10 p10 = CParseParam<P10>::ParseParam(pState, 10);

			func(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);

			return 0;
		}
	};


	template<typename Ret, typename Func, Func func>
	void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename Func, Func func>
	void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename Func, Func func>
	void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename Func, Func func>
	void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, P3, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename Func, Func func>
	void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, P3, P4, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5, typename Func, Func func>
	void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, P3, P4, P5, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename Func, Func func>
	void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, P3, P4, P5, P6, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
		typename Func, Func func>
		void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, P3, P4, P5, P6, P7, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename Func, Func func>
		void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, P3, P4, P5, P6, P7, P8, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9,
		typename Func, Func func>
		void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, P3, P4, P5, P6, P7, P8, P9, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	template<typename Ret, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10,
		typename Func, Func func>
		void RegFun(const char* szFunName)
	{
		//lua_State* pState = CScriptPlugin::getSinglePtr()->getLuaState();
		//lua_CFunction pFun = &CCallFun<Ret>::CallFun<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, Func, func>;
		//lua_register(pState, szFunName, pFun);
	}

	// template<typename Ret, typename Cla, Ret (Cla::*fun)() >
	// void RegMemFun(const char* szFunName)
	// {
	// 	lua_State* pState = Script::getScript()->getLuaState();
	// 	luna<Cla>::Build_CMemCall<Ret, Cla, fun>( pState, szFunName );
	// }
}

//类成员函数
#define REGISTER_BASECLASS_MEMFUN0( C, B, R, N, F ) \
	RegMemFun<R, C, &B::F>( N );

#define REGISTER_CLASS_MEMFUN0( C, R, N, F ) \
	RegMemFun<R, C, &C::F>( N );

#define REGISTER_CLASS_MEMFUN1( C, R, P1, N, F ) \
	RegMemFun<R, P1, C, &C::F>( N );

#define REGISTER_CLASS_MEMFUN2( C, R, P1, P2, N, F ) \
	RegMemFun<R, P1, P2, C, &C::F>( N );

#define REGISTER_CLASS_MEMFUN3( C, R, P1, P2, P3, N, F ) \
	RegMemFun<R, P1, P2, P3, C, &C::F>( N );

#define REGISTER_CLASS_MEMFUN4( C, R, P1, P2, P3, P4, N, F ) \
	RegMemFun<R, P1, P2, P3, P4, C, &C::F>( N );

#define REGISTER_CLASS_MEMFUN5( C, R, P1, P2, P3, P4, P5, N, F ) \
	RegMemFun<R, P1, P2, P3, P4, P5, C, &C::F>( N );

#define REGISTER_CLASS_MEMFUN6( C, R, P1, P2, P3, P4, P5, P6, N, F ) \
	RegMemFun<R, P1, P2, P3, P4, P5, P6, C, &C::F>( N );


//类静态成员函数
#define REGISTER_CLASS_STATIC_MEMFUN0( R, F, N ) \
	RegFun< R, R(*)(), &F>( #N );

#define REGISTER_CLASS_STATIC_MEMFUN1( R, P1, F, N ) \
	RegFun< R, C0, R(*)(C0), &F>( #N );

#define REGISTER_CLASS_STATIC_MEMFUN2( R, P1, P2, F, N ) \
	RegFun< R, C0, C1, R(*)(C0, C1), &F>( #N );

#define REGISTER_CLASS_STATIC_MEMFUN3( R, P1, P2, P3, F, N ) \
	RegFun< R, C0, C1, C2, R(*)(C0, C1, C2), &F>( #N );

#define REGISTER_CLASS_STATIC_MEMFUN4( R, P1, P2, P3, P4, F, N ) \
	RegFun< R, C0, C1, C2, C3, R(*)(C0, C1, C2, C3), &F>( #N );

#define REGISTER_CLASS_STATIC_MEMFUN5( R, P1, P2, P3, P4, P5, F, N ) \
	RegFun< R, C0, C1, C2, C3, C4, R(*)(C0, C1, C2, C3, C4), &F>( #N );

#define REGISTER_CLASS_STATIC_MEMFUN6( R, P1, P2, P3, P4, P5, P6, F, N ) \
	RegFun< R, C0, C1, C2, C3, C4, C5, R(*)(C0, C1, C2, C3, C4, C5), &F>( #N );


//普通函数
#define REGIST_GLOBALFUNCTION_0( R, F ) \
	RegFun< R, R(*)(), &F>(#F);

#define REGIST_GLOBALFUNCTION_1( R, F, C0 ) \
	RegFun< R, C0, R(*)(C0), &F>(#F);

#define REGIST_GLOBALFUNCTION_2( R, F, C0, C1 ) \
	RegFun< R, C0, C1, R(*)(C0, C1), &F>(#F);

#define REGIST_GLOBALFUNCTION_3( R, F, C0, C1, C2 ) \
	RegFun< R, C0, C1, C2, R(*)(C0, C1, C2), &F>(#F);

#define REGIST_GLOBALFUNCTION_4( R, F, C0, C1, C2, C3 ) \
	RegFun< R, C0, C1, C2, C3, R(*)(C0, C1, C2, C3), &F>(#F);

#define REGIST_GLOBALFUNCTION_5( R, F, C0, C1, C2, C3, C4 ) \
	RegFun< R, C0, C1, C2, C3, C4, R(*)(C0, C1, C2, C3, C4), &F>(#F);

#define REGIST_GLOBALFUNCTION_6( R, F, C0, C1, C2, C3, C4, C5 ) \
	RegFun< R, C0, C1, C2, C3, C4, C5, R(*)(C0, C1, C2, C3, C4, C5), &F>(#F);

#define REGIST_GLOBALFUNCTION_7( R, F, C0, C1, C2, C3, C4, C5, C6 ) \
	RegFun< R, C0, C1, C2, C3, C4, C5, C6, R(*)(C0, C1, C2, C3, C4, C5, C6), &F>(#F);

#define REGIST_GLOBALFUNCTION_8( R, F, C0, C1, C2, C3, C4, C5, C6, C7 ) \
	RegFun< R, C0, C1, C2, C3, C4, C5, C6, C7, R(*)(C0, C1, C2, C3, C4, C5, C6, C7), &F>(#F);

#define REGIST_GLOBALFUNCTION_9( R, F, C0, C1, C2, C3, C4, C5, C6, C7, C8 ) \
	RegFun< R, C0, C1, C2, C3, C4, C5, C6, C7, C8, R(*)(C0, C1, C2, C3, C4, C5, C6, C7, C8), &F>(#F);

#define REGIST_GLOBALFUNCTION_10( R, F, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9 ) \
	RegFun< R, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, R(*)(C0, C1, C2, C3, C4, C5, C6, C7, C8, C9), &F>(#F);


#include "CClass.h"

