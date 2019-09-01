#include "pch.h"
#include "Menu.h"
namespace mini
{
	namespace ui
	{
		CMenu::CMenu()
		{
			mWndType = ui_menu;
		}

		CMenu::CMenu(const std::string text) : CMenu()
		{
			setWndName(text);
		}

		CMenu::~CMenu()
		{
			for (auto& item : mItems)
			{
				if (nullptr != (item.second).pContentWnd)
				{
					SafeRelease((item.second).pWnd);
					SafeRelease((item.second).pContentWnd);
					for (auto& list : (item.second).childrenItem)
					{
						SafeRelease(list.pWnd);
						SafeRelease(list.pContentWnd);
					}
				}
			}
		}

		bool CMenu::insideMsgProcess(event& e)
		{
			for (auto& item : mItems)
			{
				if (IsSpace(e.point, (item.second).pWnd))
				{
					if ((item.second).processMsg & e.Messtype)
					{
						if (nullptr != (item.second).callback)
						{
							e.bProcess = (item.second).callback();
							return e.bProcess == TRUE ? true : false;
						}
					}
					for (auto& children : (item.second).childrenItem)
					{
						children.visible = !children.visible;
						children.pWnd->setVisual(children.visible);
					}
				}
			}
			return false;
		}

		void CMenu::addItem(const std::string& name, CWnd* pSubItem, const std::string& parentName)
		{
			if (parentName.length() != 0)
			{
				auto item = mItems.find(parentName);
				if (item != mItems.end())
				{
					(item->second).childrenItem.push_back(SubItem());
					auto& subItem = (item->second).childrenItem.back();
					subItem.pWnd = pSubItem;
					subItem.visible = false;
					subItem.name = name;
					pSubItem->AddRef();
				}
			}
			else
			{
				mItems[name].pWnd = pSubItem;
				mItems[name].visible = true;
				mItems[name].name = name;
				pSubItem->AddRef();
			}
		}

		void CMenu::parseFinished()
		{
			for (auto& item : mItems)
			{
				SubItem& subitem = (item.second);
				if (subitem.childrenItem.size() > 0)
				{
					subitem.pContentWnd = new CWnd();
					subitem.pContentWnd->setVisual(false);
					for (auto& list : subitem.childrenItem)
					{
						subitem.pContentWnd->addChildWnd(list.pWnd);
					}
				}
			}
		}

		void CMenu::bindItemFunction(MSG_TYPE msgType, const std::string& name, Function function)
		{
			for (auto& item : mItems)
			{
				if (name == (item.second).name)
				{
					(item.second).callback = std::bind(function, this);
					(item.second).processMsg |= msgType;
				}
				else
				{
					for (auto& list : (item.second).childrenItem)
					{
						if (name == list.name)
						{
							list.callback = std::bind(function, this);
							list.processMsg |= msgType;
							return;
						}
					}
				}
			}
		}
	}
}