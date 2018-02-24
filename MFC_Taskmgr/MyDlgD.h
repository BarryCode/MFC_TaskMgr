#pragma once
#include "MyList.h"


// CMyDlgD 对话框

typedef struct _MYWINDOWINFO
{
	TCHAR hWnd[MAX_PATH+1];
	TCHAR WindowName[MAX_PATH+1];
	TCHAR ClassName[MAX_PATH+1];
}MYWINDOWINFO, *PMYWINDOWINFO;


class CMyDlgD : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgD)

public:
	CMyDlgD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDlgD();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_WindowList;
	void EnumWindow();			//添加窗口;

	static BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam);			//枚举窗口回调函数;
	afx_msg void OnRclickWindowList1(NMHDR *pNMHDR, LRESULT *pResult);		//右键菜单;
	afx_msg void OnPopMenu(UINT id);										//响应右键菜单;
};
