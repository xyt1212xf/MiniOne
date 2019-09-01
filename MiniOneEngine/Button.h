#pragma once
#include "Wnd.h"
namespace mini
{
	namespace ui
	{
		class CButton : public CWnd
		{
		public:
			CButton();
			explicit CButton(const std::string text);
			virtual ~CButton();

		private:
		};
	}
}