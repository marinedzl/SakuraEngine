#pragma once

class CGameView;
class CEditorDlg : public CDialogEx
{
private:
	CGameView* m_pGameView;
public:
	CEditorDlg(CWnd* pParent = NULL);
	virtual ~CEditorDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITOR_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnOpenGameView();
};
