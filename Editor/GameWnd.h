#pragma once
#include "IGame.h"

class CGameWnd : public CWnd, public IIdleProcesser
{
	DECLARE_DYNAMIC(CGameWnd)
private:
	IGame* m_pGame;
	bool mChangingSize;
public:
	CGameWnd();
	virtual ~CGameWnd();
	void Update(float deltaTime);
	BOOL OnInitUpdate();
	void Resize();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnExitSizeMove();
	afx_msg void OnEnterSizeMove();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
};
