// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "Base.h"
#include <tchar.h>
typedef std::wstring TString;
void log(const TCHAR* fmt, ...);

std::string WStr2MStr(const std::wstring& src);