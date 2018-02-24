
// MFC_TaskmgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MFC_TaskmgrDlg.h"
#include "afxdialogex.h"
#include "MyTab.h"
#include "MyDlgA.h"
#include "MyDlgB.h"
#include "MyDlgC.h"
#include "MyDlgD.h"
#include "MyDlgE.h"

//导入电源管理库;
#include <powrprof.h>
#pragma  comment(lib,"powrprof.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_TaskmgrDlg 对话框


CMFC_TaskmgrDlg::CMFC_TaskmgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_TASKMGR_DIALOG, pParent)
{
	//更改图标;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFC_TaskmgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CMFC_TaskmgrDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL_RANGE(BN_CLICKED, ID_32801, ID_32806, &CMFC_TaskmgrDlg::OnMenuTop)	//主菜单消息处理;
END_MESSAGE_MAP()


// CMFC_TaskmgrDlg 消息处理程序

BOOL CMFC_TaskmgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//加载皮肤;
	SkinH_AttachEx(L"skins\\black.she", NULL);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化选项卡;
	m_Tab.MyInserItem(4, L"进程",  L"性能", L"窗口", L"VS清理");
	//插入窗口;
	m_Tab.MyInsertChild(4,		
		new CMyDlgB, IDD_DIALOG2,
		new CMyDlgC, IDD_DIALOG3,
		new CMyDlgD, IDD_DIALOG4,
		new CMyDlgE, IDD_DIALOG5
	);

	//注册快捷键;
	::RegisterHotKey(GetSafeHwnd(), 0x1234, MOD_CONTROL, 'H');	//必须大写字母;
	::RegisterHotKey(GetSafeHwnd(), ID_32801, MOD_ALT, '1');
	::RegisterHotKey(GetSafeHwnd(), ID_32802, MOD_ALT, '2');
	::RegisterHotKey(GetSafeHwnd(), ID_32803, MOD_ALT, '3');
	::RegisterHotKey(GetSafeHwnd(), ID_32804, MOD_ALT, '4');
	::RegisterHotKey(GetSafeHwnd(), ID_32805, MOD_ALT, '5');
	::RegisterHotKey(GetSafeHwnd(), ID_32806, MOD_ALT, '6');

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_TaskmgrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_TaskmgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//****************************************************************
// Brief 	: 主菜单消息处理;
// Method	: OnMenuTop
// FullName	: CMFC_TaskmgrDlg::OnMenuTop
// Access	: protected 
// Returns	: void
// Parameter: UINT id
//****************************************************************

void CMFC_TaskmgrDlg::OnMenuTop(UINT id)
{
	//获取关机重启权限;
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

	int Flag = 0;

	switch (id)
	{
	case ID_32801:
		LockWorkStation();
			break;
	case ID_32802:
		SetSuspendState(TRUE, FALSE, FALSE);
		break;
	case ID_32803:
		SetSuspendState(FALSE, FALSE, FALSE);
		break;
	case ID_32804:
		ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		break;
	case ID_32805:
		//关机提示;
		Flag = MessageBox(L"确定要关机吗？\r\n点击【确定】关机\r\n点击【取消】继续", L"是否关机?", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;
	case ID_32806:
		//重启提示;
		Flag = MessageBox(L"确定要重启吗？\r\n点击【确定】重启\r\n点击【取消】继续", L"是否重启?", MB_OKCANCEL);
		if (Flag == IDCANCEL)
		{
			break;
		}
		else
		{
			ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
		}
		break;
	}
}

//****************************************************************
// Brief 	: 重写消息转换虚函数;
// Method	: PreTranslateMessage
// FullName	: CMFC_TaskmgrDlg::PreTranslateMessage
// Access	: virtual protected 
// Returns	: BOOL
// Parameter: MSG * pMsg
//****************************************************************

BOOL CMFC_TaskmgrDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//响应快捷键;
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0x1234))
	{
		if (IsWindowVisible()==TRUE)
		{
			ShowWindow(SW_HIDE);
		}
		else
		{
			ShowWindow(SW_SHOW);
		}
	}
	else if ((pMsg->message == WM_HOTKEY) &&
			((pMsg->wParam == ID_32801)||
			(pMsg->wParam == ID_32802) ||
			(pMsg->wParam == ID_32803) ||
			(pMsg->wParam == ID_32804) ||
			(pMsg->wParam == ID_32805) ||
			(pMsg->wParam == ID_32806)))
	{
		OnMenuTop(pMsg->wParam);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}




