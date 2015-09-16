#pragma once
#include "GameWnd.h"

class CGameView : public CDockablePane
{
	DECLARE_DYNAMIC(CGameView)
private:
	CGameWnd mGameWnd;
public:
	CGameView();
	virtual ~CGameView();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
