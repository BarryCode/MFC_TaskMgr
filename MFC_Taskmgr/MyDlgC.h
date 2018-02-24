#pragma once
#include "afxwin.h"
#include "ChartCtrl/ChartCtrl.h"
#include <vector>
using std::vector;

// CMyDlgC 对话框


class CMyDlgC : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlgC)

public:
	CMyDlgC(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDlgC();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:	

	static UINT  MyThreadFunction(LPVOID lpParam);	//线程回调函数;
	void GetMem();		//获取内存大小;
	double m_dwMem;		//内存大小;
	void InitFont();	//初始化字体;
	CFont m_Font;		//字体;

	//界面文字;
	CStatic m_TextCPU;
	CStatic m_TextMem;
	CStatic m_TextDisk;
	CStatic m_TextNet;

	CStatic m_NumCPU;
	CStatic m_NumMem;
	CStatic m_NumDiskRead;
	CStatic m_NumDiskWirte;
	CStatic m_NumNetDown;	
	CStatic m_NumNetUP;

	//动态绘图;
	CChartCtrl m_ChartCPU1;	//图表控件;

	CChartFont labelFont;	//标签字体;
	CChartFont labelFont2;	//标签字体;字号大一点;

	double m_CPUArr[100];	//CPU数据;
	double m_MEMArr[100];	//MEM数据;
	double m_X[100];		//X轴坐标;

	void Paint();			//绘图主程序;
	void InitChart();		//初始化图标信息;
	void InsertCharData(double dCPU,double dMEM);	//更新数据;

	HANDLE hThread;
	bool m_bClose;
	afx_msg void OnClose();
};
