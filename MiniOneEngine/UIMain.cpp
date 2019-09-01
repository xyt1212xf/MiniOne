#include "pch.h"
#include "UIMain.h"
#include "GuiPlugin.h"
namespace mini
{
	namespace ui
	{
		bool SetTopWindow(CWnd* pWnd)
		{
			if (pWnd)
			{
				CWnd* pParentWnd = pWnd->getParentUIWnd();
				if (NULL != pParentWnd)
				{
					CWnd* pTestWnd = pWnd;
					pTestWnd->AddRef();
					pParentWnd->removeChildWnd(pWnd);
					pParentWnd->addChildWnd(pWnd);
					SafeRelease(pTestWnd);
				}
				else
				{
					CWnd* pTestWnd = pWnd;
					pTestWnd->AddRef();
					CGuiPlugin::getSinglePtr()->removeWnd(pWnd);
					CGuiPlugin::getSinglePtr()->addWnd(pWnd);
					SafeRelease(pTestWnd);
				}
			}
			return false;
		}

		CUIMain* CUIMain::pMySelf = nullptr;
		CWnd*	CUIMain::mpfocusWnd = nullptr;

		CUIMain::CUIMain()
		{
			pMySelf = this;

		}

		CUIMain::~CUIMain()
		{
			for (auto i : mWindows)
			{
				SafeRelease(i);
			}
			pMySelf = nullptr;
		}

		void CUIMain::addWnd(CWnd* pWnd)
		{
			for (auto i : mWindows)
			{
				if (pWnd == i)
				{
					return;
				}
			}
			mWindows.push_front(pWnd);
			pWnd->AddRef();
		}

		void CUIMain::removeWnd(CWnd* pWnd)
		{
			auto item = mWindows.begin();
			auto end = mWindows.end();
			while (item != end)
			{
				if ((*item) == pWnd)
				{
					mWindows.erase(item);
					pWnd->mpParentWnd = nullptr;
					break;
				}
				++item;
			}
		}

		ui::CWnd* CUIMain::findWnd(const std::string& strName)
		{
			for (auto i : mWindows)
			{
				if (strName == i->getWndName())
				{
					return i;
				}
			}
			return nullptr;
		}

		ui::CWnd* CUIMain::findWnd(CWnd* pWnd)
		{
			for (auto i : mWindows)
			{
				if (pWnd == i)
				{
					return i;
				}
			}
			return nullptr;
		}

		bool CUIMain::processMessage(event& msg)
		{
			//mCheckWnd = NULL;
			//ÏûÏ¢ÅÉ·¢
			if (!mbProcess)
			{
				return false;
			}
			switch (msg.Messtype)
			{
			case LEFTDOWN:
				Mousedown(msg);
				//	mMouseWnd->LMousedown(msg);
				break;
			case LEFTUP:
				Mouseup(msg);
				//		mMouseWnd->LMouseup(msg);
				break;
			case RIGHTDOWN:
				Mousedown(msg);
				//	mMouseWnd->RMousedown(msg);
				break;
			case RIGHTUP:
				Mouseup(msg);
				//	mMouseWnd->RMouseup(msg);
				break;
			case MOUSEMOVE:
				MouseMove(msg);
				//	mMouseWnd->MouseMove(msg);
				break;
			case MOUSEWHEEL:
				Mousewheel(msg);
				//	mMouseWnd->Mousewheel(msg);
				break;
			case LEFTDBLCLK:
				DoubleClick(msg);
				break;
			case RIGHTDBLCLK:
				DoubleClick(msg);
				break;
			case WMCHAR:
				KeyboardProcess(msg);
				break;
			case WMKEYDOWN:
				KeyboardProcess(msg);
				break;
			case MBUTTONDOWN:
				break;
			case MBUTTONUP:
				break;
			}
			return true;
		}


		void CUIMain::Mouseup(event& msg)
		{
			if (nullptr != mCheckWnd)
			{
				CheckMousePickUI(msg, false);
				ProcessCtrlUp(msg);
				msg.bProcess = true;
			}
		}

		void CUIMain::Mousedown(event& msg)
		{
			CheckMousePickUI(msg, true);
			ProcessCtrlDown(msg);
			if (!msg.bProcess && mModeWnd)
			{
				/*
				mModeWnd->KillTimer(0);
				mModeWnd->SetTimer(0, 150);
				*/
			}
		}

		void CUIMain::Mousewheel(event& msg)
		{
			CheckMousePickUI(msg, false);
			if (mCheckCtrl)
			{
				msg.point.x = GET_WHEEL_DELTA_WPARAM(msg.wParam);
				//	mCheckCtrl->Mousewheel(msg);
				AddCallFunction(msg);
				//		mCheckCtrl = NULL;
				msg.bProcess = true;
			}
			else if (mCheckWnd)
			{
				msg.point.x = GET_WHEEL_DELTA_WPARAM(msg.wParam);
				// mCheckWnd->wndProcess(msg.Messtype, msg.point.x, msg.point.y, selfwnd);
				msg.bProcess = true;
			}

			else
			{
				CallModeWndFun(msg);
			}
		}

		void CUIMain::MouseMove(event& msg)
		{
			MouseLeave(msg);
			if (MOUSEMOVE != msg.Messtype)
			{
				return;
			}
			CheckMousePickUI(msg, false);
			if (mCheckCtrl)
			{
				if (!(mCheckCtrl->mUnProcessMsg & msg.Messtype))
				{
					if (UI_Normal == mCheckCtrl->mState)
					{
						mCheckCtrl->setState(UI_Hover);// UI_Push UI_Hover UI_Disable UI_Normal
					}
					executeCallback(msg);
					//	mCheckCtrl->MouseMove(msg);
						/*
						if (ui_wnd != mCheckCtrl->getWndType())
						{
							AddCallFunction(msg);
						}
						else
						{
							AddCallWndFunction(msg, mCheckCtrl);
						}*/
				}
				else
				{
					CallModeWndFun(msg);
				}
				msg.bProcess = true;
			}
			else if (mCheckWnd && mCheckWnd->getTextureAlpha(msg.point) > 0)
			{
				AddCallWndFunction(msg, mCheckWnd);
				msg.bProcess = true;
			}
			else
			{
				CallModeWndFun(msg);
			}
		}

		bool CUIMain::MouseLeave(event& msg)
		{
			bool bvalue = false;
			if (mCheckCtrl)
			{
				if (IsSpace(msg.point, mCheckCtrl))
				{
					if (mCheckCtrl->getTextureAlpha(msg.point) <= 0)
					{
						bvalue = true;
					}
				}
				else
				{
					bvalue = true;
				}
				if (bvalue)
				{
					if (!(mCheckCtrl->mUnProcessMsg & MOUSEOUT))
					{
						msg.Messtype = MOUSEOUT;
						mCheckCtrl->setState(UI_Normal);
						executeCallback(msg);
						//	mCheckCtrl->MouseLeave(msg);
						//	AddCallFunction(msg);
					}
					mCheckCtrl = NULL;
					return bvalue;
				}
			}
			else
			{
				CallModeWndFun(msg);
			}
			return bvalue;
		}

		void CUIMain::DoubleClick(event& msg)
		{
			CheckMousePickUI(msg, true);
			ProcessCtrlDown(msg);
		}

		void CUIMain::KeyboardProcess(event& msg)
		{
			if (CUIMain::mpfocusWnd &&
				CUIMain::mpfocusWnd->isFocus())
			{
				std::string buf;
#if defined(_UNICODE)||defined(UNICODE)
				strtowstr(UIWnd::mpfocusWnd->mContent, buf);
#else
				buf = CUIMain::mpfocusWnd->mContent;
#endif
				//	CUIMain::mpfocusWnd->KeyboardProcess(msg);
				const CheckGroundResult& result = _CheckKeyboardFocusUI(CUIMain::mpfocusWnd);
				if (result.first)
				{
					float value = static_cast<float>(atof(CUIMain::mpfocusWnd->mContent.c_str()));
					if (value < result.second.x || value > result.second.y)
					{
						CUIMain::mpfocusWnd->SetTextContent(buf);
					}
				}
				CWnd* pTempWnd = CUIMain::mpfocusWnd->getParentUIWnd();
				while (pTempWnd)
				{
					if (ui_wnd == pTempWnd->getWndType())
					{
						//pTempWnd->wndProcess(msg.Messtype, msg.wParam, msg.lParam, CUIMain::mpfocusWnd->getId());
						break;
					}
					else
					{
						pTempWnd = pTempWnd->getParentUIWnd();
					}
				}
			}
		}

		void CUIMain::AddCallFunction(event& msg)
		{

		}

		void CUIMain::AddCallWndFunction(event& msg, CWnd* pWnd)
		{

		}

		void CUIMain::CheckCtrlWnd(event& msg)
		{
			if (mCheckCtrl && ui_wnd == mCheckCtrl->getWndType())
			{
				Childlist::reverse_iterator b, e;
				b = mCheckCtrl->mpChildren.rbegin();
				e = mCheckCtrl->mpChildren.rend();
				for (b; b != e; ++b)
				{
					if (IsProcess((*b), msg.point, msg.Messtype))
					{
						CWnd* pOldWnd = mCheckCtrl;
						mCheckCtrl = (*b);
						CheckCtrlWnd(msg);
						if (mCheckCtrl->getTextureAlpha(msg.point) > 0)
						{
							break;
						}
						mCheckCtrl = pOldWnd;
					}
				}
			}
		}

		void CUIMain::CheckMousePickUI(event& msg, bool IsTop)
		{
			mCheckCtrl = NULL;
			ShowWndList::iterator iter, end;
			iter = mWindows.begin();
			end = mWindows.end();
			for (; iter != end; ++iter)
			{
				if (IsProcess((*iter), msg.point, msg.Messtype))
				{
					mCheckWnd = (*iter);
					_CheckMousePickUI((*iter), msg, IsTop);
					return;
				}
			}
			mCheckCtrl = nullptr;
			mCheckWnd = nullptr;
		}

		CWnd* CUIMain::ProcessCtrlDown(event& msg)
		{
			if (nullptr != mCheckCtrl)
			{
				mCheckCtrl->setState(UI_Push);
				executeCallback(msg);
				
			}
			return nullptr;
		}

		CWnd* CUIMain::ProcessCtrlUp(event& msg)
		{
			if (nullptr != mCheckCtrl)
			{
				mCheckCtrl->setState(UI_Normal);
				executeCallback(msg);
			}
			return nullptr;
		}

		void CUIMain::executeCallback(event& msg)
		{
			if (nullptr != mCheckWnd)
			{
				if (!callBack(msg, mCheckWnd) )
				{
					if (nullptr != mCheckCtrl)
					{
						CWnd* pParentWnd = mCheckCtrl->getParentUIWnd();
						while (pParentWnd->getWndType() != WndType::ui_wnd)
						{
							pParentWnd = pParentWnd->getParentUIWnd();
						}
						callBack(msg, pParentWnd);
					}
				}
			}

		}

		bool CUIMain::callBack(event& msg, CWnd* pWnd)
		{
			auto item = pWnd->mItemCallback.find(mCheckCtrl);
			if (item != pWnd->mItemCallback.end())
			{
				for (auto& callback : item->second)
				{
					auto message = std::get<0>(callback);
					if (message == msg.Messtype)
					{
						std::get<1>(callback)();
						return true;
					}
				}
			}
			return false;
		}

		void CUIMain::ProcessMsgFunction(event& msg)
		{

		}

		bool CUIMain::CallModeWndFun(event& msg)
		{
			return false;
		}

		bool CUIMain::Find(CWnd* pWnd)
		{
			return false;
		}

		bool CUIMain::IsProcess(CWnd* pWnd, const POINT& pt, UINT32 MsgType)
		{
			return	(pWnd->mbVisible &&
				pWnd->mActive &&
				IsSpace(pt, pWnd) &&
				IsMessagePro(MsgType, pWnd));
		}

		bool CUIMain::IsMessagePro(UINT32 MsgType, CWnd* pWnd)
		{
			if (pWnd->mflag & UI_ProcessMsg)
			{
				return (pWnd->mTransparencyMsg & MsgType) ? false : true;
			}
			return false;
		}

		bool CUIMain::_CheckMousePickUI(CWnd* pWnd, event& msg, bool IsTop)
		{
			Childlist::reverse_iterator b, e;
			b = pWnd->mpChildren.rbegin();
			e = pWnd->mpChildren.rend();
			for (; b != e; ++b)
			{
				if (IsProcess((*b), msg.point, msg.Messtype))
				{
					mCheckCtrl = (*b);
					(*b)->insideMsgProcess(msg);
					if (!msg.bProcess)
					{
						CheckCtrlWnd(msg);
						if (mCheckCtrl->getTextureAlpha(msg.point) > 0)
						{
							if (IsTop && (mCheckCtrl->mflag & UI_ISTOP))
							{
								SetTopWindow(mCheckCtrl);
							}
							for (auto& item : mCheckCtrl->mpChildren)
							{
								if (IsProcess( item, msg.point, msg.Messtype) &&
									_CheckMousePickUI( item, msg, false))
								{
									break;
								}
							}
							break;
						}
						else
						{
							mCheckCtrl = NULL;
						}
					}	
				}
			}
		
			if (mCheckCtrl || mCheckWnd->getTextureAlpha(msg.point) > 0)
			{
				if (IsTop)
				{
					CWnd* _pWnd = pWnd;
					if (pWnd->mflag & UI_ISTOP)
					{
						mWindows.remove(pWnd);
						mWindows.push_back(_pWnd);
					}
				}
				msg.bProcess = TRUE;
			}
			return msg.bProcess == TRUE ? true : false;
		}

		ui::CheckGroundResult CUIMain::_CheckKeyboardFocusUI(CWnd* pWnd)
		{
			CheckGroundResult Result;
			Result.first = false;
			/*
			CWnd* pTempWnd = pWnd->getParentUIWnd();
			if (pTempWnd)
			{
				UINT32 ParentID = pTempWnd->getHandle();
				UINT32 SelfID = pWnd->getHandle();
				MinMaxValueMap::iterator iter = mValueGoundMap.find(ParentID);
				if (iter != mValueGoundMap.end())
				{
					map<UINT32, CPoint2F>::iterator _it = (iter->second).find(SelfID);
					if ((iter->second).end() != _it)
					{
						Result.first = true;
						Result.second = (_it->second);
					}
				}
			}
			*/
			return Result;
		}

	}
}
