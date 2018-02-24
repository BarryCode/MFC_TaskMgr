// MyDlgD.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgD.h"
#include "afxdialogex.h"
#include <vector>
using std::vector;

//������Ϣ����;
vector<MYWINDOWINFO> vecWindowInfo;

// CMyDlgD �Ի���

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


// CMyDlgD ��Ϣ�������


BOOL CMyDlgD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//������;
	m_WindowList.AddColumn(3, L"���ھ��", 100,L"���ڱ���", 200,L"��������",210);

	//���봰��;
	EnumWindow();

	//�����˵�;
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//****************************************************************
// Brief 	: ö�ٴ���;
// Method	: EnumWindowProc
// FullName	: CMyDlgD::EnumWindowProc
// Access	: public 
// Returns	: BOOL CALLBACK
// Parameter: HWND hWnd
// Parameter: LPARAM lParam
//****************************************************************

BOOL CALLBACK CMyDlgD::EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	//�����ṹ��;
	MYWINDOWINFO* WindowInfo = new MYWINDOWINFO;	
	//��ʽ�����;
	_stprintf_s(WindowInfo->hWnd, L"%p",hWnd);
	//��ȡ���ڱ���;����;
	::GetWindowTextW(hWnd, WindowInfo->WindowName, MAX_PATH);
	::GetClassNameW(hWnd, WindowInfo->ClassName, MAX_PATH);
	
	//����ɼ���������;	�Ҵ����б���;
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE && WindowInfo->WindowName[0])
	{
		vecWindowInfo.push_back(*WindowInfo);
	}	
	return TRUE;
}


//****************************************************************
// Brief 	: �����˵�;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU2);

	CMenu* pSub = pMenu->GetSubMenu(2);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);


	*pResult = 0;
}

//****************************************************************
// Brief 	: ��Ӧ�˵�;
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
		MessageBox(L"��ѡ������");
	}

	DWORD dwTemp;
	swscanf_s(m_WindowList.GetItemText(nSel, 0),L"%x", &dwTemp);

	//�жϲ˵�ID;
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
// Brief 	: ö�ٴ�����Ϣ;
// Method	: EnumWindow
// FullName	: CMyDlgD::EnumWindow
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgD::EnumWindow()
{
	//ö�ٴ���;
	::EnumWindows(EnumWindowProc, 0);
	//���봰��;
	for (size_t i = 0; i < vecWindowInfo.size(); i++)
	{
		m_WindowList.AddItem(3,
			vecWindowInfo[i].hWnd,
			vecWindowInfo[i].WindowName,
			vecWindowInfo[i].ClassName);
	}
}
