#pragma once


// CMyDlgF �Ի���

class CMyDlgF : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgF)

public:
	CMyDlgF(CString  Path, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyDlgF();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
protected:

	CString m_StrFileName;		//�ļ���;
	CString m_StrType;			//����;
	CString m_StrPath;			//·��;
	CString m_StrSize;			//��С;
	CString m_StrCreatTime;		//����ʱ��;
	CString m_StrViewTime;		//����ʱ��;
	CString m_StrChangeTime;	//�޸�ʱ��;
	BOOL m_bOnlyRead;			//ֻ��;
	BOOL m_bHide;				//����;
	
public:
	afx_msg void OnBnClickedOk();	//���OK;
};
