#include "stdafx.h"
#include "Editor.h"
#include "EditorDlg.h"
#include "afxdialogex.h"
#include "CameraCtrl.h"
#include "TransformCtrl.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEditorDlg::CEditorDlg(CWnd* pParent)
	: CDialogEx(IDD_EDITOR_DIALOG, pParent)
{
	mState = eNormal;
	mCameraCtrl = nullptr;
	mRT = nullptr;
	mSelected = nullptr;
	mTransformCtrl = nullptr;
}

CEditorDlg::~CEditorDlg()
{
	theApp.pSceneView = nullptr;
	theApp.RemoveProcesser(this);
	DestroyWindow();
	SAFE_DELETE(mCameraCtrl);
	SAFE_DELETE(mTransformCtrl);
	SAFE_RELEASE(mRT);
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
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL CEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mRT = theApp.core->CreateRenderTarget(GetSafeHwnd());
	theApp.AddProcesser(this);

	theApp.camera = theApp.core->CreateCamera();
	theApp.camera->SetView(mRT->GetWidth(), mRT->GetHeight());

	mCameraCtrl = new CameraCtrl(theApp.camera);
	mTransformCtrl = new TransformCtrl();

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

	mRT->Begin();
	theApp.scene->Draw(theApp.camera, mRT);
	mRT->End();

	theApp.scene->GetConfig()->EnableGizmo(false);
}


void CEditorDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mCameraCtrl)
	{
		mCameraCtrl->Move((float)point.x, (float)point.y);
	}
	if (mTransformCtrl)
	{
		mTransformCtrl->Move((float)point.x, (float)point.y);
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
	if (!theApp.scene)
		return;

	switch (mState)
	{
	case CEditorDlg::eNormal:
	{
		SECore::Ray ray;
		SECore::RaycastHit hit;

		theApp.camera->ScreenPointToRay(ray, Vector3((float)point.x, (float)point.y, 0));

		theApp.scene->RaycastBound(ray, hit, 10000);

		if (hit.entity)
		{
			if (mSelected)
			{
				if (hit.entity == mSelected)
				{
					mState = eTransform;
					mTransformCtrl->Begin((float)point.x, (float)point.y, TransformCtrl::eOpMove);
					SetCapture();
				}
				else
				{
					//unselect
					mSelected->SetGizmoColor(Color(0.5f, 0.5f, 0.5f, 1));
					mSelected = nullptr;
					mTransformCtrl->Attach(nullptr);

					//select
					mSelected = hit.entity;
					mSelected->SetGizmoColor(Color(0, 0, 0.5f, 1));
					mTransformCtrl->Attach(mSelected);
				}
			}
			else
			{
				//select
				mSelected = hit.entity;
				mSelected->SetGizmoColor(Color(0, 0, 0.5f, 1));
				mTransformCtrl->Attach(mSelected);
			}
		}
		else
		{
			//unselect
			mSelected->SetGizmoColor(Color(0.5f, 0.5f, 0.5f, 1));
			mSelected = nullptr;
			mTransformCtrl->Attach(nullptr);
		}
	}
		break;
	case CEditorDlg::eTransform:
	{
	}
		break;
	default:
		break;
	}

	__super::OnLButtonDown(nFlags, point);
}


void CEditorDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	mState = eNormal;
	if (mTransformCtrl)
	{
		mTransformCtrl->End();
	}
	ReleaseCapture();
	__super::OnLButtonUp(nFlags, point);
}
