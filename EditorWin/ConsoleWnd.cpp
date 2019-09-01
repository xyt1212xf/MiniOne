#include "stdafx.h"
#include "ConsoleWnd.h"

CConsoleWnd::CConsoleWnd()
{

}

CConsoleWnd::~CConsoleWnd()
{

}

void CConsoleWnd::bindMsg()
{
	BindMessage(mini::LEFTUP, "Demo", &CConsoleWnd::play);
}

bool CConsoleWnd::play()
{
	return true;
}
