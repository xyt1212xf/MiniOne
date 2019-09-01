#pragma once
namespace mini
{
	class CToScriptClass
	{
	public:
		CToScriptClass();
		virtual ~CToScriptClass();
		virtual void RegisterMember();
		virtual void ScriptClass();
		void		 RegisterHandle();
		unsigned int GetRegisterHandle();

	protected:
		unsigned int mLuaObjectHandle;
	};
}
