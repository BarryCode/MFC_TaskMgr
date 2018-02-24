// MyDlgD.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgD.h"
#include "afxdialogex.h"
#include <vector>
using std::vector;

//窗口信息容器;
vector<MYWINDOWINFO> vecWindowInfo;

// CMyDlgD 对话框

IMPLEMENT_DYNAMIC(CMyDlgD, CDialogEx)

CMyDlgD::CMyDlgD(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CMyDlgD::~CMyDlgD()
{
}

void CMyDlgD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINDOW_LIST1, m_WindowList);
}


BEGIN_MESSAGE_MAP(CMyDlgD, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_WINDOW_LIST1, &CMyDlgD::OnRclickWindowList1)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32811, ID_32813, &CMyDlgD::OnPopMenu)
END_MESSAGE_MAP()


// CMyDlgD 消息处理程序


BOOL CMyDlgD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//插入列;
	m_WindowList.AddColumn(3, L"窗口句柄", 100,L"窗口标题", 200,L"窗口类名",210);

	//插入窗口;
	EnumWindow();

	//弹出菜单;
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//****************************************************************
// Brief 	: 枚举窗口;
// Method	: EnumWindowProc
// FullName	: CMyDlgD::EnumWindowProc
// Access	: public 
// Returns	: BOOL CALLBACK
// Parameter: HWND hWnd
// Parameter: LPARAM lParam
//****************************************************************

BOOL CALLBACK CMyDlgD::EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	//建立结构体;
	MYWINDOWINFO* WindowInfo = new MYWINDOWINFO;	
	//格式化句柄;
	_stprintf_s(WindowInfo->hWnd, L"%p",hWnd);
	//获取窗口标题;类名;
	::GetWindowTextW(hWnd, WindowInfo->WindowName, MAX_PATH);
	::GetClassNameW(hWnd, WindowInfo->ClassName, MAX_PATH);
	
	//窗体可见存入容器;	且窗口有标题;
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE && WindowInfo->WindowName[0])
	{
		vecWindowInfo.push_back(*WindowInfo);
	}	
	return TRUE;
}


//****************************************************************
// Brief 	: 弹出菜单;
// Method	: OnRclickWindowList1
// FullName	: CMyDlgD::OnRclickWindowList1
// Access	: public 
// Returns	: void
// Parameter: NMHDR * pNMHDR
// Parameter: LRESULT * pResult
//****************************************************************

void CMyDlgD::OnRclickWindowList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);

	CMenu* pSub = pMenu->GetSubMenu(2);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);


	*pResult = 0;
}

//****************************************************************
// Brief 	: 响应菜单;
// Method	: OnPopMenu
// FullName	: CMyDlgD::OnPopMenu
// Access	: public 
// Returns	: void
// Parameter: UINT id
//****************************************************************

void CMyDlgD::OnPopMenu(UINT id)
{
	int nSel = m_WindowList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}

	DWORD dwTemp;
	swscanf_s(m_WindowList.GetItemText(nSel, 0),L"%x", &dwTemp);

	//判断菜单ID;
	switch (id)
	{
	case ID_32811:
		::SetWindowPos((HWND)dwTemp, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		::PostMessage((HWND)dwTemp, WM_SYSCOMMAND, SC_RESTORE, 0);
		break;
	case ID_32812:
		m_WindowList.DeleteAllItems();
		vecWindowInfo.clear();
		EnumWindow();
		break;
	case ID_32813:
		::PostMessage((HWND)dwTemp, WM_SYSCOMMAND, SC_CLOSE, 0);
		break;
	default:
		break;
	}


}

//****************************************************************
// Brief 	: 枚举窗口信息;
// Method	: EnumWindow
// FullName	: CMyDlgD::EnumWindow
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgD::EnumWindow()
{
	//枚举窗口;
	::EnumWindows(EnumWindowProc, 0);
	//插入窗口;
	for (size_t i = 0; i < vecWindowInfo.size(); i++)
	{
		m_WindowList.AddItem(3,
			vecWindowInfo[i].hWnd,
			vecWindowInfo[i].WindowName,
			vecWindowInfo[i].ClassName);
	}
}
