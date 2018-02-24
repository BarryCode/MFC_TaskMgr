// MyDlgE.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgE.h"
#include "afxdialogex.h"


// CMyDlgE �Ի���

IMPLEMENT_DYNAMIC(CMyDlgE, CDialogEx)

CMyDlgE::CMyDlgE(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
	, m_DirName(_T(""))
	, m_DeleFileName(_T(""))
{

}

CMyDlgE::~CMyDlgE()
{
}

void CMyDlgE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VSC_EDIT2, m_DirName);
	DDX_Text(pDX, IDC_VSC_EDIT1, m_DeleFileName);
}


BEGIN_MESSAGE_MAP(CMyDlgE, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_VSC_BUTTON1, &CMyDlgE::OnBnClickedButton1)	
END_MESSAGE_MAP()


// CMyDlgE ��Ϣ�������


//****************************************************************
// Brief 	: �����ť;
// Method	: OnBnClickedButton1
// FullName	: CMyDlgE::OnBnClickedButton1
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgE::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ·��;
	UpdateData(TRUE);

	//����;
	if (DeleFile(m_DirName))
	{
		MessageBox(L"�������");
	}
	else
	{
		MessageBox(L"����ʧ��");
	}
}

//****************************************************************
// Brief 	: �����ļ�;
// Method	: DeleFile
// FullName	: CMyDlgE::DeleFile
// Access	: public 
// Returns	: bool
// Parameter: CString DirName
//****************************************************************

bool CMyDlgE::DeleFile(CString DirName)
{
	//��·���˳�;
	if (DirName.IsEmpty())
	{
		MessageBox(L"·������");
		return	false;
	}

	//�ж��Ƿ��ļ���;
	WIN32_FIND_DATA wsd = {};
	HANDLE hFind = FindFirstFile(DirName, &wsd);
	if (!(wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		MessageBox(L"�����ļ���");
		return false;
	}

	DirName += L"\\*";
	hFind = FindFirstFile(DirName, &wsd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"��·��ʧ��");
	}

	do
	{
		//���˱������ϼ�;
		if (!wcscmp(wsd.cFileName, L".")
			|| !wcscmp(wsd.cFileName, L".."))
		{
			continue;
		}
		//�ݹ�����ļ���;
		else if (wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			CString NextDir = DirName.Left(DirName.GetLength() - 1);
			NextDir += wsd.cFileName;
			DeleFile(NextDir);
		}
		//ת���ļ�����ʽ;
		TCHAR* szBuff = wsd.cFileName;
		//��ȡ��׺��;
		TCHAR* szExName = PathFindExtension(szBuff);
		//�ԱȺ�׺��;
		if (m_DeleFileName.Find(szExName) >= 0)
		{
			//ɾ��ָ���ļ�;
			CString DeleFileName = DirName.Left(DirName.GetLength() - 1);
			DeleFileName += wsd.cFileName;
			DeleteFile(DeleFileName);
		}
	} while (FindNextFile(hFind, &wsd));

	FindClose(hFind);

	return true;
}





//****************************************************************
// Brief 	: �϶��ļ���
// Method	: OnDropFiles
// FullName	: CMyDlgE::OnDropFiles
// Access	: public 
// Returns	: void
// Parameter: HDROP hDropInfo
//****************************************************************

void CMyDlgE::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	TCHAR szPatch[MAX_PATH] = { 0 };

	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT idx = 0; idx < nCount; idx++)
	{
		DragQueryFile(hDropInfo, idx, szPatch, MAX_PATH);
		SetDlgItemText(IDC_VSC_EDIT2, szPatch);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//��ʼ��;
BOOL CMyDlgE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//������ҷ�ļ�;
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);		// 0x0049 == WM_COPYGLOBALDATA

	m_DeleFileName = L"\\\.obj \\\.tlog \\\.lastbuildstate \\\.idb \\\.pdb \\\.pch \\\.res \\\.ilk \\\.exe \\\.sdf \\\.ipch \\\.log \\\.db";

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
