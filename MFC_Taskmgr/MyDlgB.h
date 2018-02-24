#pragma once
#include "MyList.h"
#include "MyDlgA.h"

// CMyDlgB 对话框

class CMyDlgB : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgB)

public:
	CMyDlgB(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDlgB();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:	
	CMyList m_List;		
	void ShowProcess();		//显示进程;
	
	
	afx_msg void On32784();					//刷新;
	afx_msg void OnClose();					//结束进程;
	afx_msg void OnAttribute();				//文件属性;
	afx_msg void OnColumnclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);	//点击列头排序;
	afx_msg void OnMenuClick(UINT id);											//响应右键菜单;
	afx_msg void OnRclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);			//弹出菜单;
};
