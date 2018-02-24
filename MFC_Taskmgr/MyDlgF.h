#pragma once


// CMyDlgF 对话框

class CMyDlgF : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgF)

public:
	CMyDlgF(CString  Path, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDlgF();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
protected:

	CString m_StrFileName;		//文件名;
	CString m_StrType;			//类型;
	CString m_StrPath;			//路径;
	CString m_StrSize;			//大小;
	CString m_StrCreatTime;		//创建时间;
	CString m_StrViewTime;		//访问时间;
	CString m_StrChangeTime;	//修改时间;
	BOOL m_bOnlyRead;			//只读;
	BOOL m_bHide;				//隐藏;
	
public:
	afx_msg void OnBnClickedOk();	//点击OK;
};
