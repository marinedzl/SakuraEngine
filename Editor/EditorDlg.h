#pragma once

class CameraCtrl;
class CEditorDlg : public CDialogEx, public IIdleProcesser
{
private:
	RenderTarget* m_pRenderTarget;
	Camera* mCamera;
	CameraCtrl* mCameraCtrl;
	GameObject* mSelected;
public:
	CEditorDlg(CWnd* pParent = NULL);
	virtual ~CEditorDlg();
	void Update(float deltaTime);
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
