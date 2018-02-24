// CMyDlgF.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgF.h"
#include "afxdialogex.h"


// CMyDlgF �Ի���

IMPLEMENT_DYNAMIC(CMyDlgF, CDialogEx)

CMyDlgF::CMyDlgF(CString  Path, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
	, m_StrFileName(_T(""))
	, m_StrType(_T(""))
	, m_StrPath(Path)
	, m_StrCreatTime(_T(""))
	, m_StrViewTime(_T(""))
	, m_StrChangeTime(_T(""))
	, m_bOnlyRead(FALSE)
	, m_bHide(FALSE)
	, m_StrSize(_T(""))
{
	
}

CMyDlgF::~CMyDlgF()
{
}

void CMyDlgF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StrFileName);
	DDX_Text(pDX, IDC_EDIT2, m_StrType);
	DDX_Text(pDX, IDC_EDIT3, m_StrPath);
	DDX_Text(pDX, IDC_EDIT4, m_StrCreatTime);
	DDX_Text(pDX, IDC_EDIT5, m_StrViewTime);
	DDX_Text(pDX, IDC_EDIT6, m_StrChangeTime);
	DDX_Check(pDX, IDC_CHECK1, m_bOnlyRead);
	DDX_Check(pDX, IDC_CHECK2, m_bHide);
	DDX_Text(pDX, IDC_EDIT7, m_StrSize);
}


BEGIN_MESSAGE_MAP(CMyDlgF, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyDlgF::OnBnClickedOk)
END_MESSAGE_MAP()


// CMyDlgF ��Ϣ�������

//��ʼ��;
BOOL CMyDlgF::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ȡ�ļ���Ϣ;	
	CFileStatus FileStatus;
	if (CFile::GetStatus(m_StrPath,FileStatus))
	{
		m_StrFileName = FileStatus.m_szFullName;		
		m_StrCreatTime.Format(FileStatus.m_ctime.Format("%Y��%m��%d��  %H:%M:%S"));
		m_StrChangeTime.Format(FileStatus.m_mtime.Format("%Y��%m��%d��  %H:%M:%S"));
		m_StrViewTime.Format(FileStatus.m_atime.Format("%Y��%m��%d��  %H:%M:%S"));

		//�ļ��ߴ�;
		if (FileStatus.m_size/1024 <1)	
			m_StrSize.Format(L"%d Byte", FileStatus.m_size);
		else if (FileStatus.m_size / 1024/1024 < 1)
			m_StrSize.Format(L"%d KB", FileStatus.m_size/1024);
		else if (FileStatus.m_size / 1024/1024/1024 < 1)
			m_StrSize.Format(L"%d MB", FileStatus.m_size/1024/1024);
		else if (FileStatus.m_size / 1024 /1024/1024/1024< 1)
			m_StrSize.Format(L"%d GB", FileStatus.m_size / 1024 / 1024 / 1024);		

		//�ļ�����;
		if ((FileStatus.m_attribute & CFile::readOnly) == CFile::readOnly)
		{
			m_bOnlyRead = TRUE;
		}
		if ((FileStatus.m_attribute & CFile::hidden) == CFile::hidden)
		{
			m_bHide = TRUE;
		}

		//��ȡ�ļ�����;
		SHFILEINFO FileInfo = {};
		SHGetFileInfo(m_StrPath, 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_TYPENAME);
		m_StrType = FileInfo.szTypeName;

		UpdateData(FALSE);
	}

	else
	{
		MessageBox(L"��Ȩ�޷���");
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


//****************************************************************
// Brief 	: ���ȷ��;
// Method	: OnBnClickedOk
// FullName	: CMyDlgF::OnBnClickedOk
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgF::OnBnClickedOk()
{
	UpdateData(TRUE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD Attr = GetFileAttributes(m_StrPath);
	Attr = Attr &~FILE_ATTRIBUTE_READONLY &~FILE_ATTRIBUTE_HIDDEN;

	//��ѡ��״̬;
	if (m_bOnlyRead)
	{
		Attr |= FILE_ATTRIBUTE_READONLY;
	}
	if (m_bHide)
	{
		Attr |= FILE_ATTRIBUTE_HIDDEN;
	}
	//�����ļ�����;
	SetFileAttributes(m_StrPath, Attr);

	CDialogEx::OnOK();
}
