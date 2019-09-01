#pragma once
#include "EngineHead.h"
#include "Wnd.h"
#include "UIControlBase.h"
namespace mini
{
	namespace ui
	{
		typedef std::list<CWnd*>	ShowWndList;
		typedef std::pair<bool, CPoint2F> CheckGroundResult;

		class CUIMain : public UIControlBase
		{
			friend class CGuiPlugin;
			friend class CGuiRenderer;

		public:
			CUIMain();
			virtual ~CUIMain();
			CWnd* findWnd(const std::string& strName);
			CWnd* findWnd(CWnd* pWnd);

		private:
			void addWnd(CWnd* pWnd);
			void removeWnd(CWnd* pWnd);
			bool processMessage(event& msg);
			void Mouseup(event& msg);
			void Mousedown(event& msg);
			void Mousewheel(event& msg);
			void MouseMove(event& msg);
			bool MouseLeave(event& msg);
			void DoubleClick(event& msg);
			void KeyboardProcess(event& msg);
			void AddCallFunction(event& msg);
			void AddCallWndFunction(event& msg, CWnd* pWnd);
			void CheckCtrlWnd(event& msg);
			void CheckMousePickUI(event& msg, bool IsTop);
			CWnd* ProcessCtrlDown(event& msg);
			CWnd* ProcessCtrlUp(event& msg);

			void executeCallback(event& msg);
			bool callBack(event& msg, CWnd* pWnd);


			void ProcessMsgFunction(event& msg);
			bool CallModeWndFun(event& msg);
			bool Find(CWnd* pWnd);
			bool IsProcess(CWnd* pWnd, const POINT& pt, UINT32 MsgType);
			bool IsMessagePro(UINT32 MsgType, CWnd* pWnd);
			bool _CheckMousePickUI(CWnd* pWnd, event& mEvent, bool IsTop);
			CheckGroundResult _CheckKeyboardFocusUI(CWnd* pWnd);
		private:
			std::list<CWnd*> mWindows;
			static 	CWnd*	mpfocusWnd;
			CGuiRenderer*	mpRenderer = nullptr;
			CWnd*			mMouseWnd = nullptr;
			CWnd*			mModeWnd = nullptr;
			CWnd*			mCheckWnd = nullptr;
			CWnd*			mCheckCtrl = nullptr;
			bool			mbProcess = true;

		public:
			static CUIMain* pMySelf;
		};

		extern bool SetTopWindow(CWnd* pWnd);
	}
}
