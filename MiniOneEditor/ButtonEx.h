#pragma once
#include <afxwin.h>
class CButtonEx : public CButton
{
public:
	CButtonEx();
	virtual ~CButtonEx();
	
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//���º͵����¼�
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

};

