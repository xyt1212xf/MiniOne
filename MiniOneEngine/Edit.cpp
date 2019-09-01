#include "pch.h"
#include "edit.h"
namespace mini
{
	namespace ui
	{
		CEdit::CEdit()
		{
			mWndType = ui_button;
		}
		CEdit::CEdit(const std::string text) : CEdit()
		{
			setWndName(text);
		}

		CEdit::~CEdit()
		{

		}

	}
}