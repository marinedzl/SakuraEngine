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
	mCamera = nullptr;
	mCameraCtrl = nullptr;
	m_pRenderTarget = nullptr;
	mSelected = nullptr;
}

CEditorDlg::~CEditorDlg()
{
	theApp.pSceneView = nullptr;
	theApp.RemoveProcesser(this);
	DestroyWindow();
	SAFE_DELETE(mCameraCtrl);
	SAFE_RELEASE(mCamera);
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
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pRenderTarget = theApp.core->CreateRenderTarget(GetSafeHwnd());
	theApp.AddProcesser(this);

	mCamera = theApp.core->CreateCamera();

	mCameraCtrl = new CameraCtrl(mCamera);

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
	theApp.scene->Draw(mCamera, m_pRenderTarget);
	m_pRenderTarget->End();

	theApp.scene->GetConfig()->EnableGizmo(false);
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


void CEditorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (mSelected)
		mSelected->SetGizmoColor(Color(0.5f, 0.5f, 0.5f, 1));

	if (theApp.scene)
	{
		SECore::Ray ray;
		SECore::RaycastHit hit;

		mCamera->ScreenPointToRay(ray, Vector3((float)point.x, (float)point.y, 0));

		if (theApp.scene->RaycastBound(ray, hit, 10000))
		{
			mSelected = hit.entity;
		}

		if (mSelected)
			mSelected->SetGizmoColor(Color(0, 0, 0.5f, 1));
	}
	__super::OnLButtonDown(nFlags, point);
}
