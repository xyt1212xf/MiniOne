#include "stdafx.h"
#include "CreateEntityWnd.h"
#include "Engine.h"
#include "GuiPlugin.h"

CCreateEntityWnd::CCreateEntityWnd()
{

}

CCreateEntityWnd::~CCreateEntityWnd()
{

}

void CCreateEntityWnd::bindMsg()
{
	BindMessage(mini::LEFTUP, "CreateCube", &CCreateEntityWnd::createCube);
}

bool CCreateEntityWnd::createCube()
{
	return true;
}


