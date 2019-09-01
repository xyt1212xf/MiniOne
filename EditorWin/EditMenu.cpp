#include "stdafx.h"
#include "EditMenu.h"

bool CEditMenu::wndProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	switch (wmId)
	{
	case ID_EDIT_COPY:
		return true;
	case ID_EDIT_DELETE:
		return true;
	case ID_EDIT_CUT:
		return true;
	case ID_EDIT_PASTE:
		return true;
	}
	return false;
}
