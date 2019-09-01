#include "pch.h"
#include "button.h"
namespace mini
{
	namespace ui
	{
		CButton::CButton()
		{
			mWndType = ui_button;
		}
		CButton::CButton(const std::string text) : CButton()
		{
			setWndName(text);
		}

		CButton::~CButton()
		{

		}

	}
}