
// MFC_TaskmgrDlg.h : 头文件
//

#pragma once
#include "MyTab.h"


// CMFC_TaskmgrDlg 对话框
class CMFC_TaskmgrDlg : public CDialogEx
{
// 构造
public:
	CMFC_TaskmgrDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_TASKMGR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMenuTop(UINT id);

	DECLARE_MESSAGE_MAP()
public:
	CMyTab m_Tab;	//TAB控件;
	
};
