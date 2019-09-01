#pragma once
#include "EngineHead.h"
#include "PlugInEnum.h"
#include "FoundationStruct.h"
namespace mini
{
	class CPlugin
	{
	public:
		CPlugin();
		virtual ~CPlugin();
		virtual bool initialize();
		virtual bool onStart();
		virtual void onRun();
		virtual void onShutDown();
		virtual void Process(event& msg);
		void active(bool bActive);
		bool isRun();
		void stop();
		void resume();

	private:
		std::atomic_bool	mbRun = false;

	protected:
		bool	mActive;
	};
}
