#pragma once
#include "Wnd.h"
namespace mini
{
	namespace ui
	{
		class CMenu : public CWnd
		{
			friend class CGuiParse;
			struct SubItem
			{
				bool visible = false;
				bool active = true;
				bool fold = true;
				std::string name;
				ItemFunction callback = nullptr;
				CWnd*	pWnd = nullptr;
				CWnd*	pContentWnd = nullptr;
				UINT32	processMsg = 0;
				std::list<SubItem>	childrenItem;
			};
		public:
			CMenu();
			explicit CMenu(const std::string text);
			virtual ~CMenu();
			void bindItemFunction(MSG_TYPE msgType, const std::string& name, Function function);

		private:
			virtual bool insideMsgProcess(event& e) override;
			void addItem(const std::string& name, CWnd* pSubItem, const std::string& parentName);
			void parseFinished();

		private:
			std::map<std::string, SubItem>	mItems;
		};
	}
}