#pragma once
#include "MyList.h"


// CMyDlgA 对话框

class CMyDlgA : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgA)

public:
	CMyDlgA(CWnd* pParent, int nMID, int nPID);   // 标准构造函数
	
	virtual ~CMyDlgA();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int m_nMenuID;	//右键菜单ID;
	int m_nPID;		//进程PID;
	bool m_bFlag;	//是否显示右键菜单;

	CMyList m_PopList;			//List;
	void ShowModule(int nPid);	//模块;
	void ShowThread(int nPid);	//线程;
	void ShowHeap(int nPid);	//堆;
	
	afx_msg void OnRclickPoplist1(NMHDR *pNMHDR, LRESULT *pResult);	//右键菜单;
	afx_msg void OnMenuThread(UINT id);								//响应右键菜单;
};
