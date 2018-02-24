// MyTab.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyTab.h"


// CMyTab

IMPLEMENT_DYNAMIC(CMyTab, CTabCtrl)

CMyTab::CMyTab()
{

}

CMyTab::~CMyTab()
{
}


BEGIN_MESSAGE_MAP(CMyTab, CTabCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &CMyTab::OnNMClick)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTab::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTab 消息处理程序




void CMyTab::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMyTab::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	CTabCtrl::PreSubclassWindow();
}


//****************************************************************
// Brief 	: 选项卡切换;
// Method	: OnTcnSelchange
// FullName	: CMyTab::OnTcnSelchange
// Access	: public 
// Returns	: void
// Parameter: NMHDR * pNMHDR
// Parameter: LRESULT * pResult
//****************************************************************

void CMyTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//检索当前选项卡选择项;
	DWORD nSel = GetCurSel();
	//隐藏所有;
	for (size_t i=0;i<m_dwCount;i++)
	{
		m_pWnd[i]->ShowWindow(SW_HIDE);
	}
	//出错结束;
	if (nSel>=m_dwCount)
	{
		return;
	}

	//移动窗口和TAB客户区一样大;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 25, 0, 0);		//TOP值减小;
	m_pWnd[nSel]->MoveWindow(&rt, 1);
	m_pWnd[nSel]->ShowWindow(SW_SHOW);
}

//****************************************************************
// Brief 	: 添加选项卡;
// Method	: InserItem
// FullName	: CMyTab::InserItem
// Access	: public 
// Returns	: void
// Parameter: DWORD dwCount
// Parameter: ...选项卡名,选项卡名,...
//****************************************************************

void CMyTab::MyInserItem(DWORD dwCount, ...)
{
	//变参添加;
	va_list va;
	va_start(va, dwCount);
	for (DWORD i=0;i<dwCount;i++)
	{
		TCHAR *p = va_arg(va, TCHAR*);
		InsertItem(i, p);
	}
	va_end(va);
}

//****************************************************************
// Brief 	: 插入子窗口;
// Method	: MyInsertChild
// FullName	: CMyTab::MyInsertChild
// Access	: public 
// Returns	: void
// Parameter: DWORD dwCount
// Parameter: ...窗口类,Dialog控件,窗口类,Dialog控件...
//****************************************************************

void CMyTab::MyInsertChild(DWORD dwCount, ...)
{
	m_dwCount = dwCount;
	va_list va;
	va_start(va, dwCount);
	for (DWORD i=0;i<dwCount;i++)
	{
		m_pWnd[i] = va_arg(va, CDialogEx*);
		UINT uID = va_arg(va, UINT);
		m_pWnd[i]->Create(uID, this);
	}
	va_end(va);

	//移动窗口和TAB客户区一样大;
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 25, 0, 0);
	m_pWnd[0]->MoveWindow(&rt, 1);
	m_pWnd[0]->ShowWindow(SW_SHOW);
}
