#pragma once
#include "MyList.h"
#include "MyDlgA.h"

// CMyDlgB �Ի���

class CMyDlgB : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgB)

public:
	CMyDlgB(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyDlgB();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:	
	CMyList m_List;		
	void ShowProcess();		//��ʾ����;
	
	
	afx_msg void On32784();					//ˢ��;
	afx_msg void OnClose();					//��������;
	afx_msg void OnAttribute();				//�ļ�����;
	afx_msg void OnColumnclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);	//�����ͷ����;
	afx_msg void OnMenuClick(UINT id);											//��Ӧ�Ҽ��˵�;
	afx_msg void OnRclickProcessList1(NMHDR *pNMHDR, LRESULT *pResult);			//�����˵�;
};
