// MyDlgE.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgE.h"
#include "afxdialogex.h"


// CMyDlgE 对话框

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


// CMyDlgE 消息处理程序


//****************************************************************
// Brief 	: 点击按钮;
// Method	: OnBnClickedButton1
// FullName	: CMyDlgE::OnBnClickedButton1
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgE::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取路径;
	UpdateData(TRUE);

	//清理;
	if (DeleFile(m_DirName))
	{
		MessageBox(L"清理完成");
	}
	else
	{
		MessageBox(L"清理失败");
	}
}

//****************************************************************
// Brief 	: 清理文件;
// Method	: DeleFile
// FullName	: CMyDlgE::DeleFile
// Access	: public 
// Returns	: bool
// Parameter: CString DirName
//****************************************************************

bool CMyDlgE::DeleFile(CString DirName)
{
	//空路径退出;
	if (DirName.IsEmpty())
	{
		MessageBox(L"路径有误");
		return	false;
	}

	//判断是否文件夹;
	WIN32_FIND_DATA wsd = {};
	HANDLE hFind = FindFirstFile(DirName, &wsd);
	if (!(wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		MessageBox(L"不是文件夹");
		return false;
	}

	DirName += L"\\*";
	hFind = FindFirstFile(DirName, &wsd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"打开路径失败");
	}

	do
	{
		//过滤本级和上级;
		if (!wcscmp(wsd.cFileName, L".")
			|| !wcscmp(wsd.cFileName, L".."))
		{
			continue;
		}
		//递归遍历文件夹;
		else if (wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			CString NextDir = DirName.Left(DirName.GetLength() - 1);
			NextDir += wsd.cFileName;
			DeleFile(NextDir);
		}
		//转换文件名格式;
		TCHAR* szBuff = wsd.cFileName;
		//获取后缀名;
		TCHAR* szExName = PathFindExtension(szBuff);
		//对比后缀名;
		if (m_DeleFileName.Find(szExName) >= 0)
		{
			//删除指定文件;
			CString DeleFileName = DirName.Left(DirName.GetLength() - 1);
			DeleFileName += wsd.cFileName;
			DeleteFile(DeleFileName);
		}
	} while (FindNextFile(hFind, &wsd));

	FindClose(hFind);

	return true;
}





//****************************************************************
// Brief 	: 拖动文件：
// Method	: OnDropFiles
// FullName	: CMyDlgE::OnDropFiles
// Access	: public 
// Returns	: void
// Parameter: HDROP hDropInfo
//****************************************************************

void CMyDlgE::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TCHAR szPatch[MAX_PATH] = { 0 };

	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT idx = 0; idx < nCount; idx++)
	{
		DragQueryFile(hDropInfo, idx, szPatch, MAX_PATH);
		SetDlgItemText(IDC_VSC_EDIT2, szPatch);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}

//初始化;
BOOL CMyDlgE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//允许拖曳文件;
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);		// 0x0049 == WM_COPYGLOBALDATA

	m_DeleFileName = L"\\\.obj \\\.tlog \\\.lastbuildstate \\\.idb \\\.pdb \\\.pch \\\.res \\\.ilk \\\.exe \\\.sdf \\\.ipch \\\.log \\\.db";

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
