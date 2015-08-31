#include "stdafx.h"
#include "Editor.h"
#include "EditorDlg.h"
#include "afxdialogex.h"
#include "CameraCtrl.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEditorDlg::CEditorDlg(CWnd* pParent)
	: CDialogEx(IDD_EDITOR_DIALOG, pParent)
{
	mCameraCtrl = NULL;
	m_pRenderTarget = NULL;
}

CEditorDlg::~CEditorDlg()
{
	theApp.pSceneView = NULL;
	theApp.RemoveProcesser(this);
	DestroyWindow();
	SAFE_DELETE(mCameraCtrl);
	SAFE_RELEASE(m_pRenderTarget);
}

void CEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEditorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPEN_GAME_VIEW, &CEditorDlg::OnOpenGameView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()

BOOL CEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pRenderTarget = SECore::CreateRenderTarget(GetSafeHwnd());
	theApp.AddProcesser(this);

	mCameraCtrl = new CameraCtrl(theApp.scene->GetCamera());

	theApp.pSceneView = this;

	return TRUE;
}

void CEditorDlg::OnPaint()
{
	CDialogEx::OnPaint();
}

void CEditorDlg::OnClose()
{
	CDialogEx::OnClose();
	PostQuitMessage(0);
}

void CEditorDlg::OnOpenGameView()
{
	if (theApp.pGameView)
		return;
	CGameView* pGameView = new CGameView();
	CHECK(pGameView);
	if (pGameView->Create(IDD_GAMEVIEW))
		pGameView->ShowWindow(SW_SHOW);
	else
		SAFE_DELETE(pGameView);
Exit0:
	;
}

void CEditorDlg::Update(float deltaTime)
{
	if (!IsWindowVisible())
		return;

	if (mCameraCtrl)
		mCameraCtrl->Update(deltaTime);

	theApp.scene->GetConfig()->EnableGizmo(true);

	m_pRenderTarget->Begin();
	theApp.scene->Draw(m_pRenderTarget);
	m_pRenderTarget->End();
}


void CEditorDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Move((float)point.x, (float)point.y);
	}
	__super::OnMouseMove(nFlags, point);
}


BOOL CEditorDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Scroll(-(float)zDelta * 0.01f);
	}
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}


void CEditorDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Begin((float)point.x, (float)point.y, CameraCtrl::eOpRotate);
		SetCapture();
	}
	__super::OnRButtonDown(nFlags, point);
}


void CEditorDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->End();
		ReleaseCapture();
	}
	__super::OnRButtonUp(nFlags, point);
}


void CEditorDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Begin((float)point.x, (float)point.y, CameraCtrl::eOpMove);
		SetCapture();
	}
	__super::OnMButtonDown(nFlags, point);
}


void CEditorDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->End();
		ReleaseCapture();
	}
	__super::OnMButtonUp(nFlags, point);
}
