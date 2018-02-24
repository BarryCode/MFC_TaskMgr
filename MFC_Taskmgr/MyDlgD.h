#pragma once
#include "MyList.h"


// CMyDlgD �Ի���

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
	CMyDlgD(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyDlgD();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_WindowList;
	void EnumWindow();			//��Ӵ���;

	static BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam);			//ö�ٴ��ڻص�����;
	afx_msg void OnRclickWindowList1(NMHDR *pNMHDR, LRESULT *pResult);		//�Ҽ��˵�;
	afx_msg void OnPopMenu(UINT id);										//��Ӧ�Ҽ��˵�;
};
