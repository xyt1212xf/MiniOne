#pragma once
#include "Wnd.h"
namespace mini
{
	namespace ui
	{
		class CEdit : public CWnd
		{
		public:
			CEdit();
			explicit CEdit(const std::string text);
			virtual ~CEdit();

		private:
		};
	}
}