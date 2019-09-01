#include "stdafx.h"
#include "ResourceMenu.h"

bool CResourceMenu::wndProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	switch (wmId)
	{
	case ID_RES_CREATE:
		return true;
	case ID_RES_SHOW:
		return true;
	case ID_RES_OPEN:
		return true;
	case ID_RES_DELETE:
		return true;
	case ID_RES_RENAME:
		return true;
	case ID_RES_COPYPATH:
		return true;
	}
	return false;
}
