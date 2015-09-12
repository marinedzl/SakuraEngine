// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// Editor.h : Editor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CEditorApp:
// �йش����ʵ�֣������ Editor.cpp
//

class CameraCtrl;
class CEditorApp : public CWinAppEx
{
private:
	DWORD mLastTime;
	std::list<IIdleProcesser*> m_processers;
public:
	SECore::Core* core;
	SECore::Scene* scene;
	SECore::Camera* camera;
	CameraCtrl* cameraCtrl;
	GameObject* selected;
public:
	CEditorApp();
	void AddProcesser(IIdleProcesser* v) { m_processers.push_back(v); }
	void RemoveProcesser(IIdleProcesser* v) { m_processers.remove(v); }

private:
	BOOL InitCore();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

// ʵ��

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEditorApp theApp;
