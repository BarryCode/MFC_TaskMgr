#pragma once
#include "MyList.h"


// CMyDlgA �Ի���

class CMyDlgA : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgA)

public:
	CMyDlgA(CWnd* pParent, int nMID, int nPID);   // ��׼���캯��
	
	virtual ~CMyDlgA();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int m_nMenuID;	//�Ҽ��˵�ID;
	int m_nPID;		//����PID;
	bool m_bFlag;	//�Ƿ���ʾ�Ҽ��˵�;

	CMyList m_PopList;			//List;
	void ShowModule(int nPid);	//ģ��;
	void ShowThread(int nPid);	//�߳�;
	void ShowHeap(int nPid);	//��;
	
	afx_msg void OnRclickPoplist1(NMHDR *pNMHDR, LRESULT *pResult);	//�Ҽ��˵�;
	afx_msg void OnMenuThread(UINT id);								//��Ӧ�Ҽ��˵�;
};
