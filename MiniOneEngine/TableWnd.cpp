#include "pch.h"
#include "TableWnd.h"

namespace mini
{
	namespace ui
	{

		CTableWnd::CTableWnd()
		{
			mWndType = ui_tableWnd;
			mSubItemWnds.reserve(8);
		}

		CTableWnd::~CTableWnd()
		{
			for (auto item : mSubItemWnds)
			{
				item.pItemWnd->Release();
				item.pWnd->Release();
			}
			mSubItemWnds.clear();
		}

		void CTableWnd::addToTable(CWnd* pWnd, const std::string& itemName )
		{
			subWnd item;
			CRadio* pRadio = new CRadio();
			pRadio->setGuiStyle("default");
			pRadio->setGroupId(getId());
			addChildWnd(pRadio);
			pRadio->setFont(mpFontUnit);
			pRadio->setWindowText(itemName);
			pRadio->setPosition( mSubItemWnds.size() * 80.0f, 0);
			pRadio->setSize(80, 25);
			item.pItemWnd = pRadio;
			item.pWnd = pWnd;
			SafeRelease(pRadio);

			mCurrentIndex = mSubItemWnds.size();
			addChildWnd(pWnd);
			mSubItemWnds.push_back(item);
			pWnd->setPosition(0, 25);
			select(mSubItemWnds.size() - 1);
		}

		void CTableWnd::removeFromTable(INT32 index)
		{
			if (index < (INT32)mSubItemWnds.size())
			{
				auto item = mSubItemWnds.begin() + index;
				(*item).pWnd->Release();
				(*item).pItemWnd->Release();
				mSubItemWnds.erase(item);
			}
		}

		void CTableWnd::select(INT32 index)
		{
			if (index < (INT32)mSubItemWnds.size())
			{
				for (auto& item : mSubItemWnds)
				{
					(item).pWnd->setVisual(false);
				}
				mSubItemWnds[index].pWnd->setVisual(true);
				mSubItemWnds[index].pItemWnd->select();
				mCurrentIndex = index;
			}
		}

		bool CTableWnd::insideMsgProcess(event& e)
		{
			if (LEFTUP == e.Messtype)
			{
				INT32 idx = 0;
				for (auto& item : mSubItemWnds)
				{
					if (IsSpace(e.point, (item).pItemWnd))
					{
						select(idx);
					}
					else
					{
						++idx;
					}
				}
			}
			return true;
		}

	}
}