#pragma once


// CMyTab
//自定义扩展TAB;

class CMyTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTab)

public:
	CMyTab();
	virtual ~CMyTab();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);		
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);	//选项卡切换;
	
	void MyInserItem(DWORD dwCount, ...);							//插入选项卡标签;	
	void MyInsertChild(DWORD dwCount, ...);							//添加子对话框;	
	DWORD m_dwCount;												//保存子对话框信息;
	CDialogEx* m_pWnd[10];											//子窗口数组;
};


