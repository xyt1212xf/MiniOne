#include "pch.h"
#include "CClass.h"
namespace mini
{
	void updata(lua_State* L, UINT32 offset, ETypeEnum Type)
	{
		switch (Type)
		{
		case eTypeBool:
		{
			bool value = bool(1 == lua_toboolean(L, -3));
			memcpy((void*)offset, &value, sizeof(bool));
			break;
		}
		case eTypeChar:
		{
			INT8 value = (INT8)lua_tonumber(L, -3);
			memcpy((void*)offset, &value, sizeof(INT8));
			break;
		}
		case eTypeUChar:
		{
			UINT8 value = (UINT8)lua_tonumber(L, -2);
			memcpy((void*)offset, &value, sizeof(UINT8));
			break;
		}
		case eTypeShort:
		{
			INT16 value = (UINT16)lua_tonumber(L, -3);
			memcpy((void*)offset, &value, sizeof(INT16));
			break;
		}
		case eTypeUShort:
		{
			UINT16 value = (UINT16)lua_tonumber(L, -3);
			memcpy((void*)offset, &value, sizeof(UINT16));
			break;
		}
		case eTypeInt:
		case eTypeLong:
		{
			INT32 value = (INT32)lua_tonumber(L, -3);
			memcpy((void*)offset, &value, sizeof(INT32));
			break;
		}
		case eTypeUInt:
		case eTypeULong:
		{
			UINT32 value = (UINT32)lua_tonumber(L, -3);
			memcpy((void*)offset, &value, sizeof(UINT32));
			break;
		}
		case eTypeDouble:
		{
			double value = (double)lua_tonumber(L, -3);
			memcpy((void*)offset, &value, sizeof(double));
			break;
		}
		case eTypeFloat:
		{
			float value = (float)lua_tonumber(L, -2);
			memcpy((void*)offset, &value, sizeof(float));
			break;
		}
		case eTypeString:
		{
			const char* value = lua_tostring(L, -2);
			memcpy((void*)offset, value, strlen(value) + 1);
			break;
		}
		}
	}
}