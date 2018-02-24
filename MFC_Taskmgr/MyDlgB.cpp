// MyDlgB.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgB.h"
#include "MyDlgF.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <tchar.h>

// CMyDlgB �Ի���

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


// CMyDlgB ��Ϣ�������


BOOL CMyDlgB::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	//��ʼ���б�;
	m_List.AddColumn(5, L"����", 100, L"PID", 50,L"������PID",80 ,L"�߳�����", 80, L"·��", 180);
	//��ʾ����;
	ShowProcess();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


//****************************************************************
// Brief 	: ��ʾ����;
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
		MessageBox(L"HELP ��������ʧ�ܣ�");
		return;
	}

	//��ʼ��������Ϣ�ṹ��;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		MessageBox(L"��ȡ������Ϣʧ��");
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

		//��Ҫ��ȡ·���Ľ���
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);
		//��ȡ��������·��;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);
		}
		else
		{
			szBuf[4] = L"��Ȩ����";
		}
		m_List.AddItem(5, szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4]);
		CloseHandle(hPro);
	} while (Process32Next(hSnapShot, &pi));

	return;
}


//****************************************************************
// Brief 	: �����Ҽ��˵�;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);
	
	CMenu* pSub = pMenu->GetSubMenu(0);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}


//****************************************************************
// Brief 	: ��Ӧ�Ҽ��˵�;
// Method	: OnMenuClick
// FullName	: CMyDlgB::OnMenuClick
// Access	: public 
// Returns	: void
// Parameter: UINT id
//****************************************************************

void CMyDlgB::OnMenuClick(UINT id)
{
	// TODO: �ڴ���������������
	//��ȡѡ����;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
	}
	int PID = _wtoi(m_List.GetItemText(nSel, 1));
	
	CMyDlgA DlgA(this,id,PID);
	DlgA.DoModal();
}


//****************************************************************
// Brief 	: ˢ��
// Method	: On32784
// FullName	: CMyDlgB::On32784
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgB::On32784()
{
	// TODO: �ڴ���������������
	//ˢ�½���;
	m_List.DeleteAllItems();
	ShowProcess();
}


//****************************************************************
// Brief 	: �������̣�
// Method	: OnClose
// FullName	: CMyDlgB::OnClose
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgB::OnClose()
{
	// TODO: �ڴ���������������
	//��ȡPid;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
	}
	int PID = _wtoi(m_List.GetItemText(nSel, 1));
	//��ȡ���;
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
	if (TerminateProcess(hProcess,0))
	{
		MessageBox(L"�����ɹ�!");
	}
	else
	{
		MessageBox(L"����ʧ��!");
	}
	CloseHandle(hProcess);
	m_List.DeleteAllItems();
	ShowProcess();
}


//****************************************************************
// Brief 	: �ļ�����;
// Method	: OnAttribute
// FullName	: CMyDlgB::OnAttribute
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgB::OnAttribute()
{
	// TODO: �ڴ���������������
	//��ȡѡ����;
	int nSel = m_List.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
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
		MessageBox(L"��ѡ����Ȩ�޷��ʵ�·��");
	}	
}


//****************************************************************
// Brief 	: �����ͷ����;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�����;
	int Sort_Colum = pNMLV->iSubItem;

	//0��4���ַ�������;������������;
	if (Sort_Colum==0||Sort_Colum==4)
		m_List.SortItemsPlus(Sort_Colum, false);
	else
		m_List.SortItemsPlus(Sort_Colum);

	*pResult = 0;
}
