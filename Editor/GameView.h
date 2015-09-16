#pragma once
#include "IGame.h"

class CGameView : public CDockablePane, public IIdleProcesser
{
	DECLARE_DYNAMIC(CGameView)
private:
	IGame* m_pGame;
	bool mChangingSize;
public:
	CGameView();
	virtual ~CGameView();
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
};
