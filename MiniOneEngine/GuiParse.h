#pragma once
#include "GUIStyle.h"
#include "Wnd.h"
#include "Button.h"
#include "TableWnd.h"
#include "Menu.h"
#include "Radio.h"

namespace mini
{
	namespace ui
	{
		class CGuiParse
		{
			friend class CGuiPlugin;
		private:
			CGuiParse();
			~CGuiParse();
			CWnd* createControl(const std::string& wndtype);
			std::map<std::string, std::string> parseAttribute(TiXmlElement* pElement);
			std::map<std::string, std::string>  parseWnd(CWnd* pWnd, TiXmlElement* pElement);
			std::map<std::string, std::string> parseFont(CWnd* pWnd, TiXmlElement* pElement);
			std::map<std::string, std::string>  parseMenu(CWnd* pWnd, TiXmlElement* pElement);
			std::map<std::string, std::string>  parseButton(CWnd* pWnd, TiXmlElement* pElement);
			std::map<std::string, std::string>  parseTablwWnd(CWnd* pWnd, TiXmlElement* pElement);
			std::map<std::string, std::string>  parseRadio(CWnd* pWnd, TiXmlElement* pElement);
			void bindCommonAttribute(CWnd* pWnd, std::map<std::string, std::string>& attributeMap);

		private:
			CGuiStyleManager*	mpGuiStyleManager;
		};
	}
}