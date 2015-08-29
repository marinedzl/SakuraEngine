#include "stdafx.h"
#include "Editor.h"
#include "SceneLoader.h"
#include "EditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CEditorApp, CWinApp)
END_MESSAGE_MAP()


CEditorApp::CEditorApp()
{
	scene = nullptr;
	mLastTime = 0;
}

CEditorApp theApp;

BOOL CEditorApp::InitInstance()
{
	BOOL ret = FALSE;

	//_CrtSetBreakAlloc(148);

	CWinApp::InitInstance();

	SetRegistryKey(_T("Sakura Engine Editor"));

	CEditorDlg* dlg = new CEditorDlg();
	CHECK(dlg);

	if (!dlg->Create(IDD_EDITOR_DIALOG))
	{
		CHECK(!"Create Main Dialog Failed!");
		SAFE_DELETE(dlg);
		goto Exit0;
	}

	m_pMainWnd = dlg;
	m_pMainWnd->ShowWindow(SW_SHOW);
	
	CHECK(SECore::InitCore(""));

	scene = SECore::CreateScene();
	CHECK(scene);

	CHECK(SceneLoader::Load(scene, _T("scene.json")));

	ret = TRUE;
Exit0:
	return ret;
}

int CEditorApp::ExitInstance()
{
	SAFE_DELETE(m_pMainWnd);
	SAFE_RELEASE(scene);
	SECore::ReleaseCore();
	return CWinApp::ExitInstance();
}

BOOL CEditorApp::OnIdle(LONG lCount)
{
	DWORD currentTime = timeGetTime();
	DWORD dwDelta = currentTime - mLastTime;

	if (dwDelta > 0)
	{
		mLastTime = currentTime;

		float deltaTime = dwDelta / 1000.0f;

		//deltaTime *= 0.1f;

		if (scene)
			scene->Update(deltaTime);
	}

	return CWinApp::OnIdle(lCount);
}
