#pragma once

class CGameView : public CDialogEx
{
	DECLARE_DYNAMIC(CGameView)

public:
	CGameView(CWnd* pParent = NULL);
	virtual ~CGameView();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMEVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
