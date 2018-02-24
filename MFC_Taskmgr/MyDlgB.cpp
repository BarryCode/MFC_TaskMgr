// MyDlgB.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgB.h"
#include "MyDlgF.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <tchar.h>

// CMyDlgB 对话框

IMPLEMENT_DYNAMIC(CMyDlgB, CDialogEx)

CMyDlgB::CMyDlgB(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CMyDlgB::~CMyDlgB()
{
}

void CMyDlgB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Process_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CMyDlgB, CDialogEx)
	ON_COMMAND(ID_32784, &CMyDlgB::On32784)
	ON_COMMAND(ID_32787, &CMyDlgB::OnClose)
	ON_COMMAND(ID_32786, &CMyDlgB::OnAttribute)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32781, ID_32783, &CMyDlgB::OnMenuClick)
	ON_NOTIFY(NM_RCLICK, IDC_Process_LIST1, &CMyDlgB::OnRclickProcessList1)	
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_Process_LIST1, &CMyDlgB::OnColumnclickProcessList1)
END_MESSAGE_MAP()


// CMyDlgB 消息处理程序


BOOL CMyDlgB::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	//初始化列表;
	m_List.AddColumn(5, L"名称", 100, L"PID", 50,L"父进程PID",80 ,L"线程数量", 80, L"路径", 180);
	//显示进程;
	ShowProcess();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//****************************************************************
// Brief 	: 显示进程;
// Method	: ShowProcess
// FullName	: CMyDlgB::ShowProcess
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgB::ShowProcess()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"HELP 函数调用失败！");
		return;
	}

	//初始化快照信息结构体;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		MessageBox(L"获取进程信息失败");
		return;
	}
	do
	{
		TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6],  new TCHAR[6],  new TCHAR[6], new TCHAR[MAX_PATH], };
		szBuf[0] = pi.szExeFile;
		if (pi.th32ProcessID > 0&& pi.th32ParentProcessID>0&& pi.cntThreads>=0)
		{
			_stprintf_s(szBuf[1], 6, L"%d", pi.th32ProcessID);
			_stprintf_s(szBuf[2], 6, L"%d", pi.th32ParentProcessID);
			_stprintf_s(szBuf[3], 6, L"%d", pi.cntThreads);
		}
		else
		{
			szBuf[1] = L"  ";
			szBuf[2] = L"  ";
			szBuf[3] = L"  ";
		}

		//打开要获取路径的进程
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);
		//获取进程完整路径;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);
		}
		else
		{
			szBuf[4] = L"无权访问";
		}
		m_List.AddItem(5, szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4]);
		CloseHandle(hPro);
	} while (Process32Next(hSnapShot, &pi));

	return;
}


//****************************************************************
// Brief 	: 弹出右键菜单;
// Method	: OnRclickProcessList1
// FullName	: CMyDlgB::OnRclickProcessList1
// Access	: public 
// Returns	: void
// Parameter: NMHDR * pNMHDR
// Parameter: LRESULT * pResult
//****************************************************************

void CMyDlgB::OnRclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);
	
	CMenu* pSub = pMenu->GetSubMenu(0);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}


//****************************************************************
// Brief 	: 响应右键菜单;
// Method	: OnMenuClick
// FullName	: CMyDlgB::OnMenuClick
// Access	: public 
// Returns	: void
// Parameter: UINT id
//****************************************************************

void CMyDlgB::OnMenuClick(UINT id)
{
	// TODO: 在此添加命令处理程序代码
	//获取选中项;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	int PID = _wtoi(m_List.GetItemText(nSel, 1));
	
	CMyDlgA DlgA(this,id,PID);
	DlgA.DoModal();
}


//****************************************************************
// Brief 	: 刷新
// Method	: On32784
// FullName	: CMyDlgB::On32784
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgB::On32784()
{
	// TODO: 在此添加命令处理程序代码
	//刷新进程;
	m_List.DeleteAllItems();
	ShowProcess();
}


//****************************************************************
// Brief 	: 结束进程；
// Method	: OnClose
// FullName	: CMyDlgB::OnClose
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgB::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	//获取Pid;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	int PID = _wtoi(m_List.GetItemText(nSel, 1));
	//获取句柄;
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
	if (TerminateProcess(hProcess,0))
	{
		MessageBox(L"结束成功!");
	}
	else
	{
		MessageBox(L"结束失败!");
	}
	CloseHandle(hProcess);
	m_List.DeleteAllItems();
	ShowProcess();
}


//****************************************************************
// Brief 	: 文件属性;
// Method	: OnAttribute
// FullName	: CMyDlgB::OnAttribute
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgB::OnAttribute()
{
	// TODO: 在此添加命令处理程序代码
	//获取选中项;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	CString Path=m_List.GetItemText(nSel, 4);
	if ((Path[0]>='a'&&Path[0]<='z')||
		(Path[0] >= 'A'&&Path[0] <= 'Z'))
	{
		CMyDlgF DlgF(Path);
		DlgF.DoModal();
	}
	else
	{
		MessageBox(L"请选择有权限访问的路径");
	}	
}


//****************************************************************
// Brief 	: 点击列头排序;
// Method	: OnColumnclickProcessList1
// FullName	: CMyDlgB::OnColumnclickProcessList1
// Access	: public 
// Returns	: void
// Parameter: NMHDR * pNMHDR
// Parameter: LRESULT * pResult
//****************************************************************

void CMyDlgB::OnColumnclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获取点击列;
	int Sort_Colum = pNMLV->iSubItem;

	//0列4列字符串排序;其他整型排序;
	if (Sort_Colum==0||Sort_Colum==4)
		m_List.SortItemsPlus(Sort_Colum, false);
	else
		m_List.SortItemsPlus(Sort_Colum);

	*pResult = 0;
}
