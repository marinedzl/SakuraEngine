#pragma once

class CameraCtrl;
class TransformCtrl;
class CChildView : public CWnd, public IIdleProcesser
{
private:
	bool mIsInited;
	RenderTarget* mRT;
	TransformCtrl* mTransformCtrl;
	enum State
	{
		eNormal,
		eTransform,
	};
	State mState;
	bool mChangingSize;
public:
	CChildView();
	void Update(float deltaTime);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitUpdate();
	void Resize();

public:
	virtual ~CChildView();

protected:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonCapture();
	afx_msg void OnExitSizeMove();
	afx_msg void OnEnterSizeMove();
};
