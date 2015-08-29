#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"

class CEditorApp : public CWinApp
{
public:
	DWORD mLastTime;
	SECore::Scene* scene;
public:
	CEditorApp();
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
};

extern CEditorApp theApp;
