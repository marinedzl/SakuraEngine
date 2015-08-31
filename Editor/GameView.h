#pragma once

class CameraCtrl;
class CGameView : public CDialogEx, public IIdleProcesser
{
	DECLARE_DYNAMIC(CGameView)
private:
	RenderTarget* m_pRenderTarget;
	CameraCtrl* mCameraCtrl;

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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
