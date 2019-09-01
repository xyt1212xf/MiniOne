#pragma once
#include "luaBind.h"
namespace mini
{
	extern void updata(lua_State* L, UINT32 offset, ETypeEnum Type);

	/*
	template<typename T>
	static int Script_Init( lua_State* L )
	{
		lua_newtable( L );
		lua_pushstring( L, "__index" );
		luaL_getmetatable( L, T::MetaName );
		lua_rawset( L, -3 );

		luaL_getmetatable( L, T::MetaName );
		lua_setmetatable( L, -2 );

		return 1;
	}


	template<typename T>
	inline int CtoScript_Init()
	{
		lua_State* L = CScriptPlugin::getSinglePtr()->getLuaState();
		CreateMetatable( T::MetaName, L );
		lua_pushnumber( L, 0 );
		T**p = (T**)lua_newuserdata( L, sizeof( T ) );
		*p = T::getSinglePtr();
		luaL_getmetatable( L, T::MetaName );
		lua_setmetatable( L, -2 );
		lua_rawset( L, -3 );

		T::RegistertoScript();
		std::string ClassName = T::ClassName;
		ClassName.append( "_Inst" );
		lua_register( L, ClassName.c_str(), Script_Init<T> );
		return 1;
	}
	*/


	template< class T >
	class CClass
	{
		typedef std::map<std::string, CCMemberData<T>* > RegMemMap;
	public:
		CClass(const std::string& SelfName)
		{
			mpObject = new T();
			mClassName = SelfName;
			static CLuaBind* pLuaBind = CLuaBind::initialize();
			pLuaBind->bindClass(mpObject);
		}

		virtual ~CClass()
		{
			auto b = mRegMemData.begin();
			auto e = mRegMemData.end();
			for (; b != e; ++b)
			{
				SafeDelete(b->second);
			}
			mRegMemData.clear();
			SafeDelete(mpObject);
		}
		void UpRegMemData(T* pObject, const char* key)
		{
			lua_State* L = CScriptPlugin::getSinglePtr()->getLuaState();
			auto iter = mRegMemData.find(key);
			if (iter != mRegMemData.end())
			{
				UINT32 offset = (UINT32)(&*pObject) + (iter->second)->mOffset;
				updata(L, offset, (iter->second)->mType);
			}
		}

		void addClassMemData(CCMemberData<T>* pMemData)
		{
			mRegMemData.insert(make_pair(pMemData->mName, pMemData));
		}



	private:
		T* mpObject;
		std::string mClassName;
		RegMemMap mRegMemData;
	};

#define DeclareScriptClass( Class ) \
public:\
	static const char ClassName[]; \
	static const char MetaName[]; \
	static CClass<Class> mCClass; \
	virtual void RegisterMember();	\
	virtual void ScriptClass();	

#define DefineScriptClass( Class ) \
	Class registerInitstance;\
	const char Class::ClassName[] = #Class; \
	const char Class::MetaName[] = #Class##"_Meta"; \
	CClass<Class> Class::mCClass( #Class );


#define REGISTER_CLASS_MEMDATA( Class, Var, Type ) \
	luna<Class>::Reg_MemData( CScriptPlugin::getSinglePtr()->getLuaState(), #Var, Type, OFFSET( Class, Var ) );

#define DeclareLightClass( Class ) \
	public: \
	static const char ClassName[]; \
	static const char MetaName[]; 

#define DefineLightClass( Class ) \
	const char Class::ClassName[] = #Class; \
	const char Class::MetaName[] = #Class##"_Meta"; 



#define BeginRegisterMember( Class )\
void Class::ScriptClass()\
{\
	luna<Class>::Register_Class( CScriptPlugin::getSinglePtr()->getLuaState()); \
}\
void Class::RegisterMember()\
{

#define EndRegisterMember() }

}
