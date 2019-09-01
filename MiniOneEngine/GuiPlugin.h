#pragma once
#include "plugin.h"
#include "ScriptPlugin.h"
#include "UIMain.h"
#include "GuiParse.h"
#include "GuiRenderer.h"
namespace mini
{
	class CCamera;
	namespace ui
	{
		class CGuiPlugin : public CPlugin, public TSingle<CGuiPlugin>
		{
			friend class CPluginFactory;
		public:
			virtual ~CGuiPlugin();
			void addWnd(CWnd* pWnd);
			void removeWnd(CWnd* pWnd);
			CWnd* findWnd(const std::string& strName);
			CWnd* findWnd(CWnd* pWnd);
			bool initWnd(CWnd* pWnd, std::string guiFile);
			virtual void Process(event& msg);

		private:
			CGuiPlugin(CScriptPlugin* pPlugin);
			virtual bool onStart();
			virtual void onRun();
			virtual void onShutDown();
			bool callScript(UINT32 scriptHandle, UINT32 id);
			bool callCPlusPlus(CWnd* pWnd, CWnd* pchildWnd);
		private:
			CGuiParse			mGuiParse;
			CUIMain*			mpUIMain;
			CCamera*			mpUICamear;
			CGuiRenderer*		mpGuiRenderer = nullptr;
			CScriptPlugin*		mpPlugin;
			static CGuiPlugin*	gMySelf;
		};
	}
}