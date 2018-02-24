// MyDlgC.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Taskmgr.h"
#include "MyDlgC.h"
#include "afxdialogex.h"
#include <windows.h>
#include <Pdh.h>
#include <PdhMsg.h>
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartTitle.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartString.h"


#pragma  comment(lib,"pdh.lib")

// CMyDlgC 对话框

IMPLEMENT_DYNAMIC(CMyDlgC, CDialogEx)

CMyDlgC::CMyDlgC(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CMyDlgC::~CMyDlgC()
{
}

void CMyDlgC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_CPU, m_TextCPU);
	DDX_Control(pDX, IDC_TEXT_MEM, m_TextMem);
	DDX_Control(pDX, IDC_TEXT_DISK, m_TextDisk);
	DDX_Control(pDX, IDC_TEXT_NET, m_TextNet);
	DDX_Control(pDX, IDC_NUM_CPU, m_NumCPU);
	DDX_Control(pDX, IDC_NUM_MEM, m_NumMem);
	DDX_Control(pDX, IDC_NUM_DISK, m_NumDiskWirte);
	DDX_Control(pDX, IDC_NUM_DISK2, m_NumDiskRead);
	DDX_Control(pDX, IDC_NUM_NET, m_NumNetDown);
	DDX_Control(pDX, IDC_NUM_NET2, m_NumNetUP);
	DDX_Control(pDX, IDC_CPU, m_ChartCPU1);
}


//****************************************************************
// Brief 	: 字体设置;
// Method	: InitFont
// FullName	: CMyDlgC::InitFont
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgC::InitFont()
{
	//初始化字体;
	m_Font.CreatePointFont(100, L"微软雅黑", NULL);

	//设置字体;
	m_TextCPU.SetFont(&m_Font, true);
	m_TextMem.SetFont(&m_Font, true);
	m_TextDisk.SetFont(&m_Font, true);
	m_TextNet.SetFont(&m_Font, true);

	m_NumCPU.SetFont(&m_Font, true);
	m_NumMem.SetFont(&m_Font, true);
	m_NumDiskRead.SetFont(&m_Font, true);
	m_NumDiskWirte.SetFont(&m_Font, true);
	m_NumNetDown.SetFont(&m_Font, true);
	m_NumNetUP.SetFont(&m_Font, true);
}

//****************************************************************
// Brief 	: 初始化图表;
// Method	: CPUChart
// FullName	: CMyDlgC::CPUChart
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgC::InitChart()
{
	//清空数据;
	for (size_t i = 0; i < 100; i++)
	{
		m_X[i] = i;
		m_CPUArr[i] = 0;
		m_MEMArr[i] = 0;
	}

	//建立坐标;
	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCPU1.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetVisible(false);	//设置坐标轴隐藏;	
	pAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pAxis->SetMinMax(0, 100);
	pAxis = m_ChartCPU1.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pAxis->SetVisible(true);
	pAxis->SetMinMax(0, 100);

	//设置标题;
	TChartString str1;
	str1 = L"";
	m_ChartCPU1.GetTitle()->AddString(str1);

	//设置背景色;
	m_ChartCPU1.SetBackColor(RGB(253, 255, 223));
	//设置表格色;
	//m_ChartCPU1.SetBorderColor(RGB(221, 240, 237));	

	//设置标签字体;
	labelFont.SetFont(L"微软雅黑", 60, false, true, false);
	labelFont2.SetFont(L"微软雅黑", 80, false, true, false);
}


//****************************************************************
// Brief 	: 线程获取性能数据;
// Method	: MyThreadFunction
// FullName	: CMyDlgC::MyThreadFunction
// Access	: public static 
// Returns	: UINT
// Parameter: LPVOID lpParam
//****************************************************************

UINT CMyDlgC::MyThreadFunction(LPVOID lpParam)
{

	CMyDlgC* DlgC = (CMyDlgC*)lpParam;

	HQUERY query;
	PDH_STATUS status = PdhOpenQuery(NULL, NULL, &query);
	if (status != ERROR_SUCCESS)
	{
		DlgC->MessageBox(L"初始化查询失败！");
		return 0;
	}

	//定义查询项;
	HCOUNTER cntCPU, cntMemory;
	HCOUNTER cntDiskRead, cntDiskWrite;

	//加入计数器;
	status = PdhAddCounter(query, L"\\Processor Information(_Total)\\% Processor Time", NULL, &cntCPU);
	status = PdhAddCounter(query, L"\\Memory\\Available MBytes", NULL, &cntMemory);

	//硬盘和网络根据电脑类型不同，查询语句附加信息不同；可读取配置文件拼接查询语句;
	status = PdhAddCounter(query, L"\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", NULL, &cntDiskRead);
	status = PdhAddCounter(query, L"\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", NULL, &cntDiskWrite);

	//定义容器存储网卡名称与路径;
	vector<HCOUNTER> vecNetSent, vecNetRece;
	vector<CString>  vecNetSentPath, vecNetRecePath;


	//枚举获得网卡名称;

	//清零缓冲区;
	DWORD dwCountListLen = NULL;
	DWORD dwInstandeListLen = NULL;
	LPTSTR lpCounterList = NULL;
	LPTSTR lpInstanceList = NULL;
	LPTSTR lpString = NULL;

	//获得缓冲区大小;
	PdhEnumObjectItems(NULL, NULL, L"Network Interface", lpCounterList, &dwCountListLen,
		lpInstanceList, &dwInstandeListLen, PERF_DETAIL_WIZARD, 0);

	//分配缓冲区内存;
	lpCounterList = new TCHAR[dwCountListLen];
	lpInstanceList = new TCHAR[dwInstandeListLen];

	//枚举网卡名称;
	status = PdhEnumObjectItems(NULL, NULL, L"Network Interface", lpCounterList, &dwCountListLen,
		lpInstanceList, &dwInstandeListLen, PERF_DETAIL_WIZARD, 0);

	//判断是否正确;
	if (status != ERROR_SUCCESS)
	{
		DlgC->MessageBox(L"添加计数器失败!");
		return 0;
	}

	//for循环取出网卡名称;字符串中保存形式为[网卡名 0 网卡名 0 ...];每个网卡名称中以0隔开;
	for (lpString = lpInstanceList; *lpString != 0; lpString += lstrlen(lpString) + 1)
	{
		//计数器;
		HCOUNTER hNetSent, hNetRece;
		//数据路径;
		TCHAR SentPath[MAX_PATH], RecePath[MAX_PATH];
		//拼接路径;
		_stprintf_s(SentPath, MAX_PATH, L"\\Network Interface(%s)\\Bytes Sent/sec", lpString);
		_stprintf_s(RecePath, MAX_PATH, L"\\Network Interface(%s)\\Bytes Received/sec", lpString);
		//存入容器;
		vecNetSent.push_back(hNetSent);
		vecNetRece.push_back(hNetRece);
		vecNetSentPath.push_back(SentPath);
		vecNetRecePath.push_back(RecePath);
	}

	//添加计数器;
	for (size_t i = 0; i < vecNetSent.size(); i++)
	{
		status = PdhAddCounter(query, vecNetRecePath[i], NULL, &vecNetRece[i]);
		status = PdhAddCounter(query, vecNetSentPath[i], NULL, &vecNetSent[i]);
	}


	//循环刷新数据;//m_bClose判断程序是否结束;
	while (DlgC->m_bClose)
	{

		//查询数据;
		PdhCollectQueryData(query);
		//需要收集两个样本;至少间隔500ms;
		Sleep(500);
		PdhCollectQueryData(query);

		//临时数据;
		double dCPUData, dMEMData;

		//CPU使用率;
		PDH_FMT_COUNTERVALUE pdhValue;
		DWORD dwValue;
		CString Buff;
		status = PdhGetFormattedCounterValue(cntCPU, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
		if (status != ERROR_SUCCESS)
		{
			DlgC->m_NumCPU.SetWindowText(L"CPU数据查询失败!");
		}
		else
		{
			dCPUData = pdhValue.doubleValue;
			Buff.Format(L"利用率 %d %%", (int)pdhValue.doubleValue);
			DlgC->m_NumCPU.SetWindowText(Buff);
		}

		//内存可用数量;
		status = PdhGetFormattedCounterValue(cntMemory, PDH_FMT_LONG, &dwValue, &pdhValue);
		if (status != ERROR_SUCCESS)
		{
			DlgC->m_NumMem.SetWindowText(L"内存数据查询失败!");
		}
		else
		{
			dMEMData = (100 - pdhValue.longValue / DlgC->m_dwMem * 100);
			Buff.Format(L"利用率 %.0f %%", (100 - pdhValue.longValue / DlgC->m_dwMem * 100));
			DlgC->m_NumMem.SetWindowText(Buff);
		}


		//刷新CPU/MEM数据;
		DlgC->InsertCharData(dCPUData, dMEMData);


		//硬盘上传下载;
		status = PdhGetFormattedCounterValue(cntDiskRead, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
		if (status != ERROR_SUCCESS)
		{
			DlgC->m_NumDiskRead.SetWindowText(L"硬盘读取数据查询失败!");
		}
		else
		{
			Buff.Format(L"读取: %.2f MB /秒", pdhValue.doubleValue / 1024 / 1024);
			DlgC->m_NumDiskRead.SetWindowText(Buff);
		}

		status = PdhGetFormattedCounterValue(cntDiskWrite, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
		if (status != ERROR_SUCCESS)
		{
			DlgC->m_NumDiskWirte.SetWindowText(L"硬盘写入数据查询失败!");
		}
		else
		{
			Buff.Format(L"写入: %.2f MB /秒", pdhValue.doubleValue / 1024 / 1024);
			DlgC->m_NumDiskWirte.SetWindowText(Buff);
		}

		
		//for循环获取网络数据;
		double  dSend = 0; 
		double  dRece = 0;
		for (size_t i=0;i<vecNetRece.size();i++)
		{
			status = PdhGetFormattedCounterValue(vecNetRece[i], PDH_FMT_DOUBLE, &dwValue, &pdhValue);
			if (status != ERROR_SUCCESS) ;
			else  dRece += pdhValue.doubleValue;

			status = PdhGetFormattedCounterValue(vecNetSent[i], PDH_FMT_DOUBLE, &dwValue, &pdhValue);
			if (status != ERROR_SUCCESS);
			else dSend += pdhValue.doubleValue;
		}

		//输出;
		Buff.Format(L"下载: %.2f KB /秒", dRece / 1024);
		DlgC->m_NumNetDown.SetWindowText(Buff);

		Buff.Format(L"上传: %.2f KB /秒", dSend / 1024);
		DlgC->m_NumNetUP.SetWindowText(Buff);
	}

	PdhCloseQuery(query);
	return 0;
}

//****************************************************************
// Brief 	: 获取内存大小;
// Method	: GetMem
// FullName	: CMyDlgC::GetMem
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgC::GetMem()
{
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	m_dwMem = memStatus.ullTotalPhys / 1024.0 / 1024;
}

//****************************************************************
// Brief 	: 绘制图表;
// Method	: Paint
// FullName	: CMyDlgC::Paint
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgC::Paint()
{
	//关闭刷新;
	m_ChartCPU1.EnableRefresh(false);

	m_ChartCPU1.RemoveAllSeries();	//清空;	


	//绘制CPU;
	CChartLineSerie *pLineSerie1;
	pLineSerie1 = m_ChartCPU1.CreateLineSerie();
	pLineSerie1->AddPoints(m_X, m_CPUArr, 100);
	pLineSerie1->SetName(_T("CPU"));
	pLineSerie1->SetColor(RGB(94, 213, 209));

	//绘制MEM;
	CChartLineSerie *pLineSerie2;
	pLineSerie2 = m_ChartCPU1.CreateLineSerie();
	pLineSerie2->AddPoints(m_X, m_MEMArr, 100);
	pLineSerie2->SetName(L"内存");
	pLineSerie2->SetColor(RGB(241, 124, 103));


	//添加标签;
	CChartBalloonLabel<SChartXYPoint>* pLable =
		pLineSerie1->CreateBalloonLabel(96, _T("CPU"));
	pLable->SetFont(labelFont);


	//添加标签;
	CChartBalloonLabel<SChartXYPoint>* pLable2 =
		pLineSerie2->CreateBalloonLabel(96, _T("内存"));
	pLable2->SetFont(labelFont2);


	//启用刷新;
	m_ChartCPU1.EnableRefresh(true);
	int n = 0;
}

//****************************************************************
// Brief 	: 更新数据;
// Method	: InsertCharData
// FullName	: CMyDlgC::InsertCharData
// Access	: public 
// Returns	: void
// Parameter: double dCPU
// Parameter: double dMEM
//****************************************************************

void CMyDlgC::InsertCharData(double dCPU, double dMEM)
{
	for (size_t i = 1; i < 100; i++)
	{
		m_CPUArr[i - 1] = m_CPUArr[i];
		m_MEMArr[i - 1] = m_MEMArr[i];
	}
	m_CPUArr[99] = dCPU;
	m_MEMArr[99] = dMEM;
}

BEGIN_MESSAGE_MAP(CMyDlgC, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMyDlgC 消息处理程序


//****************************************************************
// Brief 	: 初始化;
// Method	: OnInitDialog
// FullName	: CMyDlgC::OnInitDialog
// Access	: virtual protected 
// Returns	: BOOL
//****************************************************************

BOOL CMyDlgC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//获取内存大小;
	GetMem();

	//初始化字体;
	InitFont();

	m_bClose = true;

	//创建线程刷新使用率;
	hThread=AfxBeginThread(MyThreadFunction, this);

	//初始化图表;
	InitChart();

	//设置定时器;
	SetTimer(0, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//****************************************************************
// Brief 	: 定时器绘制图表;
// Method	: OnTimer
// FullName	: CMyDlgC::OnTimer
// Access	: protected 
// Returns	: void
// Parameter: UINT_PTR nIDEvent
//****************************************************************

void CMyDlgC::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 0)
	{
		//绘图;
		Paint();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CMyDlgC::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//关闭线程;
	m_bClose = false;

	CDialogEx::OnClose();
}
