// MyDlgA.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgA.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <tchar.h>

// CMyDlgA 对话框

IMPLEMENT_DYNAMIC(CMyDlgA, CDialogEx)

CMyDlgA::CMyDlgA(CWnd* pParent, int nMID, int nPID)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	this->m_nMenuID = nMID;
	this->m_nPID = nPID;
	m_bFlag = false;
}


CMyDlgA::~CMyDlgA()
{
}

void CMyDlgA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POPLIST1, m_PopList);
}


//****************************************************************
// Brief 	: 显示模块;
// Method	: ShowModule
// FullName	: CMyDlgA::ShowModule
// Access	: public 
// Returns	: void
// Parameter: int nPid
//****************************************************************

void CMyDlgA::ShowModule(int nPid)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPid);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"打开进程信息失败！");
		return;
	}
	MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
	if (!Module32First(hSnapShot, &me))
	{
		MessageBox(L"获取模块信息失败！");
		return;
	}
	m_PopList.AddColumn(3, L"模块名称", 100, L"模块大小", 100, L"模块路径", 290);
	do
	{
		TCHAR* SizeBuf=new TCHAR[11];
		_stprintf_s(SizeBuf, 10, L"%d",me.modBaseSize);
		m_PopList.AddItem(3, me.szModule, SizeBuf,me.szExePath);
	} while (Module32Next(hSnapShot, &me));

	return;
}

//****************************************************************
// Brief 	: 遍历线程;
// Method	: ShowThread
// FullName	: CMyDlgA::ShowThread
// Access	: public 
// Returns	: void
// Parameter: int nPid
//****************************************************************

void CMyDlgA::ShowThread(int nPid)
{
	//创建快照;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);

	//保存进程信息结构体;
	THREADENTRY32 ThreadInfo = { sizeof(THREADENTRY32) };
	
	//遍历进程;
	if (Thread32First(hSnapShot,&ThreadInfo))
	{
		m_PopList.AddColumn(3, L"线程ID", 160, L"优先级", 160, L"状态", 170);
		do
		{
			//显示符合条件的线程;
			if (ThreadInfo.th32OwnerProcessID==m_nPID)
			{
				CString PID, Levele, State;
				PID.Format(L"%d", ThreadInfo.th32ThreadID);
				Levele.Format(L"%d", ThreadInfo.tpBasePri);

				//获取线程句柄;
				DWORD dwState = 0;
				HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION |
					THREAD_QUERY_LIMITED_INFORMATION, FALSE, ThreadInfo.th32ThreadID);

					//判断线程状态;
				GetExitCodeThread(hThread, &dwState);
				if (dwState ==STILL_ACTIVE)
				{
					State = L"运行中";
				}
				else
				{
					State = L"挂起";
				}
				CloseHandle(hThread);
				m_PopList.AddItem(3, PID, Levele, State);
			}

		} while (Thread32Next(hSnapShot, &ThreadInfo));
	}
	else
	{
		MessageBox(L"获取线程信息失败！");
	}
}


//****************************************************************
// Brief 	: 遍历堆;
// Method	: ShowHeap
// FullName	: CMyDlgA::ShowHeap
// Access	: public 
// Returns	: void
// Parameter: int nPid
//****************************************************************

void CMyDlgA::ShowHeap(int nPid)
{
	//获取快照;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, nPid);
	if (!hSnapShot)
	{
		MessageBox(L"获取信息失败");
		return;
	}

	//堆链信息;
	HEAPLIST32 HeapListInfo = { 0 };
	HeapListInfo.dwSize = sizeof(HEAPLIST32);

	//获取第一个堆链信息;
	if (Heap32ListFirst(hSnapShot,&HeapListInfo))
	{
		m_PopList.AddColumn(4, L"堆ID", 120, L"大小", 120, L"起始地址", 120, L"状态", 130);
		do 
		{
			//堆块信息;
			HEAPENTRY32 HeapInfo = { 0 };
			HeapInfo.dwSize = sizeof(HEAPENTRY32);

			//获取堆块信息;
			if (Heap32First(&HeapInfo, nPid, HeapListInfo.th32HeapID))
			{
				do 
				{
					CString HID, BlockSize, Address, Flags;
					HID.Format(L"%d", HeapInfo.th32HeapID);
					BlockSize.Format(L"%d", HeapInfo.dwBlockSize);
					Address.Format(L"0x%x", HeapInfo.dwAddress);
					if (HeapInfo.dwFlags==LF32_FIXED)
					{
						Flags = L"已使用";
					}
					else if (HeapInfo.dwFlags == LF32_MOVEABLE)
					{
						Flags = L"可移动";
					}
					else
					{
						Flags = L"空闲";
					}
					m_PopList.AddItem(4, HID, BlockSize, Address, Flags);
				} while (Heap32Next(&HeapInfo));
			}		
		} while (Heap32ListNext(hSnapShot,&HeapListInfo));
	}
	else
	{
		MessageBox(L"获取信息失败");
	}
}

BEGIN_MESSAGE_MAP(CMyDlgA, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_POPLIST1, &CMyDlgA::OnRclickPoplist1)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32791, ID_32793, &CMyDlgA::OnMenuThread)	//处理菜单组;
END_MESSAGE_MAP()


// CMyDlgA 消息处理程序


BOOL CMyDlgA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	switch (m_nMenuID)
	{
	case ID_32781:
		m_bFlag = true;
		ShowThread(m_nPID);
		break;
	case ID_32782:		
		ShowModule(m_nPID);
		break;
	case ID_32783:
		ShowHeap(m_nPID);
		break;
	default:
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//****************************************************************
// Brief 	: 线程列表弹出右键菜单;
// Method	: OnRclickPoplist1
// FullName	: CMyDlgA::OnRclickPoplist1
// Access	: public 
// Returns	: void
// Parameter: NMHDR * pNMHDR
// Parameter: LRESULT * pResult
//****************************************************************

void CMyDlgA::OnRclickPoplist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//非线程列表结束;
	if (!m_bFlag)
	{
		return;
	}

	//线程列表下显示菜单;

	//获取弹出菜单;
	CMenu* pMenuThread = new CMenu;
	pMenuThread->LoadMenuW(IDR_MENU2);

	CMenu* pSubThread = pMenuThread->GetSubMenu(1);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSubThread->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}

//****************************************************************
// Brief 	: 响应右键菜单;
// Method	: OnMenuThread
// FullName	: CMyDlgA::OnMenuThread
// Access	: public 
// Returns	: void
// Parameter: UINT id
//****************************************************************

void CMyDlgA::OnMenuThread(UINT id)
{
	int nSel = m_PopList.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	int PID = _wtoi(m_PopList.GetItemText(nSel, 0));

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, PID);
	switch (id)
	{
	case  ID_32791:
		if (SuspendThread(hThread)== -1)
		{
			MessageBox(L"挂起线程失败！");
		}
		else
		{
			MessageBox(L"挂起线程成功！");
		}
		break;
	case ID_32792:
		if (ResumeThread(hThread)== -1)
		{
			MessageBox(L"恢复线程失败！");
		}
		else
		{
			MessageBox(L"恢复线程成功！");
		}
		break;
	case ID_32793:
		if (!TerminateThread(hThread,0))
		{
			MessageBox(L"结束线程失败！");
		}
		else
		{
			MessageBox(L"结束线程成功！");
		}
		break;
	default:
		break;
	}


}
