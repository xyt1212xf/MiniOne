#include "stdafx.h"
#include "ButtonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CButtonEx, CButton)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()

END_MESSAGE_MAP()

CButtonEx::CButtonEx()
{
}


CButtonEx::~CButtonEx()
{
}

void CButtonEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	//添加你的代码
	GetParent()->SetTimer(1, 50, nullptr);
	CButton::OnLButtonDown(nFlags, point);
}

void CButtonEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	//添加你的代码 
	GetParent()->KillTimer(1);
	CButton::OnLButtonUp(nFlags, point);
}
