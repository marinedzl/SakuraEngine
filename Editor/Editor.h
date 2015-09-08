#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"

class CEditorDlg;
class CGameView;
class CEditorApp : public CWinApp
{
private:
	DWORD mLastTime;
	std::list<IIdleProcesser*> m_processers;
public:
	SECore::Core* core;
	SECore::Scene* scene;
	SECore::Camera* camera;
	CEditorDlg* pSceneView;
	CGameView* pGameView;
public:
	CEditorApp();
	void AddProcesser(IIdleProcesser* v) { m_processers.push_back(v); }
	void RemoveProcesser(IIdleProcesser* v) { m_processers.remove(v); }
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
};

extern CEditorApp theApp;
