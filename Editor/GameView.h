#pragma once
#include "IGame.h"

class CameraCtrl;
class CGameView : public CDialogEx, public IIdleProcesser
{
	DECLARE_DYNAMIC(CGameView)
private:
	IGame* m_pGame;

public:
	CGameView(CWnd* pParent = NULL);
	virtual ~CGameView();
	void Update(float deltaTime);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMEVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
