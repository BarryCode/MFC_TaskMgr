
// MFC_Taskmgr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_TaskmgrApp: 
// �йش����ʵ�֣������ MFC_Taskmgr.cpp
//

class CMFC_TaskmgrApp : public CWinApp
{
public:
	CMFC_TaskmgrApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_TaskmgrApp theApp;