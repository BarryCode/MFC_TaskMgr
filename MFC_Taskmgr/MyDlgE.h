#pragma once


// CMyDlgE �Ի���

class CMyDlgE : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgE)

public:
	CMyDlgE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyDlgE();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();				//�����ť;	
	afx_msg void OnDropFiles(HDROP hDropInfo);		//�϶��ļ�����;
	CString m_DirName;								//�ļ���·��;
	CString m_DeleFileName;							//ɾ���ļ�����;
	bool DeleFile(CString DirName);					//�������;
};
