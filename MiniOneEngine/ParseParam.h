#pragma once
#include "ScriptHead.h"
namespace mini
{
	template<typename T>
	class CType2Enum
	{
	public:
		enum {
			Value = eTypeNone,
		};
	};

	template<>
	class CType2Enum<bool>
	{
	public:
		enum {
			Value = eTypeBool,
		};
	};

	template<>
	class CType2Enum<char>
	{
	public:
		enum {
			Value = eTypeChar,
		};
	};

	template<>
	class CType2Enum<unsigned char>
	{
	public:
		enum {
			Value = eTypeUChar,
		};
	};

	template<>
	class CType2Enum<short>
	{
	public:
		enum {
			Value = eTypeShort,
		};
	};

	template<>
	class CType2Enum<unsigned short>
	{
	public:
		enum {
			Value = eTypeUShort,
		};
	};

	template<>
	class CType2Enum<int>
	{
	public:
		enum {
			Value = eTypeInt,
		};
	};

	template<>
	class CType2Enum<unsigned int>
	{
	public:
		enum {
			Value = eTypeUInt,
		};
	};

	template<>
	class CType2Enum<INT64>
	{
	public:
		enum {
			Value = eTypeLong,
		};
	};

	template<>
	class CType2Enum<UINT64>
	{
	public:
		enum {
			Value = eTypeULong,
		};
	};

	template<>
	class CType2Enum<float>
	{
	public:
		enum {
			Value = eTypeFloat,
		};
	};

	template<>
	class CType2Enum<double>
	{
	public:
		enum {
			Value = eTypeDouble,
		};
	};

	template<>
	class CType2Enum<char*>
	{
	public:
		enum {
			Value = eTypeString,
		};
	};

	template<>
	class CType2Enum<const char*>
	{
	public:
		enum {
			Value = eTypeString,
		};
	};

	//////////////////////////////////////////////////////////////////////////
	//数据类型 

	template<typename T>
	class CParseParam
	{
	public:
		static T ParseParam(lua_State* pState, int index)
		{
			int type = CType2Enum<T>::Value;
			switch (type)
			{
			case eTypeChar:
			case eTypeUChar:
			case eTypeShort:
			case eTypeUShort:
			case eTypeInt:
			case eTypeUInt:
			case eTypeFloat:
			case eTypeDouble:
				return static_cast<T>(lua_tonumber(pState, index));
			case eTypeLong:
			case eTypeULong:
				return static_cast<T>(lua_tonumber(pState, index));
			default:
				printf("不正确的参数类型");
				break;
			}

			return 0;
		}
	};

	template<>
	class CParseParam<char*>
	{
	public:
		static char* ParseParam(lua_State* pState, int index)
		{
			return const_cast<char*>(lua_tostring(pState, index));
		}
	};

	template<>
	class CParseParam<const char*>
	{
	public:
		static const char* ParseParam(lua_State* pState, int index)
		{
			return lua_tostring(pState, index);
		}
	};

	template<>
	class CParseParam<bool>
	{
	public:
		static bool ParseParam(lua_State* pState, int index)
		{
			return lua_toboolean(pState, index) != 0 ? true : false;
		}
	};


	template<typename T>
	static void ParseRet(lua_State* pState, T ret)
	{
		int type = CType2Enum<T>::Value;
		switch (type)
		{
		case eTypeChar:
		case eTypeUChar:
		case eTypeShort:
		case eTypeUShort:
		case eTypeInt:
		case eTypeUInt:
		case eTypeFloat:
		case eTypeDouble:
			lua_pushnumber(pState, static_cast<double>(ret));
			break;
		case eTypeLong:
		case eTypeULong:
			//notify It won't reach the upper limit of UINT64, it's just a double type of data. !!!!!
			lua_pushnumber(pState, static_cast<double>(ret));
			break;
		default:
			printf("不正确的参数类型");
			break;
		}
	}

	template<>
	static void ParseRet(lua_State* pState, char* ret)
	{
		lua_pushstring(pState, ret);
	}


	template<>
	static void ParseRet(lua_State* pState, const char* ret)
	{
		lua_pushstring(pState, ret);
	}

	template<>
	static void ParseRet(lua_State* pState, bool ret)
	{
		lua_pushboolean(pState, ret);
	}


	//template<>
	//static void ParseRet( lua_State* pState, ResourceBase* pWnd )
	//{
	//	
	//}

	//////////////////////////////////////////////////////////////////////////
	//
}