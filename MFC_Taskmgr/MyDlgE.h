#pragma once


// CMyDlgE 对话框

class CMyDlgE : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgE)

public:
	CMyDlgE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDlgE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();				//点击按钮;	
	afx_msg void OnDropFiles(HDROP hDropInfo);		//拖动文件处理;
	CString m_DirName;								//文件夹路径;
	CString m_DeleFileName;							//删除文件类型;
	bool DeleFile(CString DirName);					//清理程序;
};
