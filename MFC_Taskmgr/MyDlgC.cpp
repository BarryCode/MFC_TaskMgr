// MyDlgC.cpp : ʵ���ļ�
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

// CMyDlgC �Ի���

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
// Brief 	: ��������;
// Method	: InitFont
// FullName	: CMyDlgC::InitFont
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgC::InitFont()
{
	//��ʼ������;
	m_Font.CreatePointFont(100, L"΢���ź�", NULL);

	//��������;
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
// Brief 	: ��ʼ��ͼ��;
// Method	: CPUChart
// FullName	: CMyDlgC::CPUChart
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgC::InitChart()
{
	//�������;
	for (size_t i = 0; i < 100; i++)
	{
		m_X[i] = i;
		m_CPUArr[i] = 0;
		m_MEMArr[i] = 0;
	}

	//��������;
	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCPU1.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetVisible(false);	//��������������;	
	pAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pAxis->SetMinMax(0, 100);
	pAxis = m_ChartCPU1.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pAxis->SetVisible(true);
	pAxis->SetMinMax(0, 100);

	//���ñ���;
	TChartString str1;
	str1 = L"";
	m_ChartCPU1.GetTitle()->AddString(str1);

	//���ñ���ɫ;
	m_ChartCPU1.SetBackColor(RGB(253, 255, 223));
	//���ñ��ɫ;
	//m_ChartCPU1.SetBorderColor(RGB(221, 240, 237));	

	//���ñ�ǩ����;
	labelFont.SetFont(L"΢���ź�", 60, false, true, false);
	labelFont2.SetFont(L"΢���ź�", 80, false, true, false);
}


//****************************************************************
// Brief 	: �̻߳�ȡ��������;
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
		DlgC->MessageBox(L"��ʼ����ѯʧ�ܣ�");
		return 0;
	}

	//�����ѯ��;
	HCOUNTER cntCPU, cntMemory;
	HCOUNTER cntDiskRead, cntDiskWrite;

	//���������;
	status = PdhAddCounter(query, L"\\Processor Information(_Total)\\% Processor Time", NULL, &cntCPU);
	status = PdhAddCounter(query, L"\\Memory\\Available MBytes", NULL, &cntMemory);

	//Ӳ�̺�������ݵ������Ͳ�ͬ����ѯ��丽����Ϣ��ͬ���ɶ�ȡ�����ļ�ƴ�Ӳ�ѯ���;
	status = PdhAddCounter(query, L"\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", NULL, &cntDiskRead);
	status = PdhAddCounter(query, L"\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", NULL, &cntDiskWrite);

	//���������洢����������·��;
	vector<HCOUNTER> vecNetSent, vecNetRece;
	vector<CString>  vecNetSentPath, vecNetRecePath;


	//ö�ٻ����������;

	//���㻺����;
	DWORD dwCountListLen = NULL;
	DWORD dwInstandeListLen = NULL;
	LPTSTR lpCounterList = NULL;
	LPTSTR lpInstanceList = NULL;
	LPTSTR lpString = NULL;

	//��û�������С;
	PdhEnumObjectItems(NULL, NULL, L"Network Interface", lpCounterList, &dwCountListLen,
		lpInstanceList, &dwInstandeListLen, PERF_DETAIL_WIZARD, 0);

	//���仺�����ڴ�;
	lpCounterList = new TCHAR[dwCountListLen];
	lpInstanceList = new TCHAR[dwInstandeListLen];

	//ö����������;
	status = PdhEnumObjectItems(NULL, NULL, L"Network Interface", lpCounterList, &dwCountListLen,
		lpInstanceList, &dwInstandeListLen, PERF_DETAIL_WIZARD, 0);

	//�ж��Ƿ���ȷ;
	if (status != ERROR_SUCCESS)
	{
		DlgC->MessageBox(L"��Ӽ�����ʧ��!");
		return 0;
	}

	//forѭ��ȡ����������;�ַ����б�����ʽΪ[������ 0 ������ 0 ...];ÿ��������������0����;
	for (lpString = lpInstanceList; *lpString != 0; lpString += lstrlen(lpString) + 1)
	{
		//������;
		HCOUNTER hNetSent, hNetRece;
		//����·��;
		TCHAR SentPath[MAX_PATH], RecePath[MAX_PATH];
		//ƴ��·��;
		_stprintf_s(SentPath, MAX_PATH, L"\\Network Interface(%s)\\Bytes Sent/sec", lpString);
		_stprintf_s(RecePath, MAX_PATH, L"\\Network Interface(%s)\\Bytes Received/sec", lpString);
		//��������;
		vecNetSent.push_back(hNetSent);
		vecNetRece.push_back(hNetRece);
		vecNetSentPath.push_back(SentPath);
		vecNetRecePath.push_back(RecePath);
	}

	//��Ӽ�����;
	for (size_t i = 0; i < vecNetSent.size(); i++)
	{
		status = PdhAddCounter(query, vecNetRecePath[i], NULL, &vecNetRece[i]);
		status = PdhAddCounter(query, vecNetSentPath[i], NULL, &vecNetSent[i]);
	}


	//ѭ��ˢ������;//m_bClose�жϳ����Ƿ����;
	while (DlgC->m_bClose)
	{

		//��ѯ����;
		PdhCollectQueryData(query);
		//��Ҫ�ռ���������;���ټ��500ms;
		Sleep(500);
		PdhCollectQueryData(query);

		//��ʱ����;
		double dCPUData, dMEMData;

		//CPUʹ����;
		PDH_FMT_COUNTERVALUE pdhValue;
		DWORD dwValue;
		CString Buff;
		status = PdhGetFormattedCounterValue(cntCPU, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
		if (status != ERROR_SUCCESS)
		{
			DlgC->m_NumCPU.SetWindowText(L"CPU���ݲ�ѯʧ��!");
		}
		else
		{
			dCPUData = pdhValue.doubleValue;
			Buff.Format(L"������ %d %%", (int)pdhValue.doubleValue);
			DlgC->m_NumCPU.SetWindowText(Buff);
		}

		//�ڴ��������;
		status = PdhGetFormattedCounterValue(cntMemory, PDH_FMT_LONG, &dwValue, &pdhValue);
		if (status != ERROR_SUCCESS)
		{
			DlgC->m_NumMem.SetWindowText(L"�ڴ����ݲ�ѯʧ��!");
		}
		else
		{
			dMEMData = (100 - pdhValue.longValue / DlgC->m_dwMem * 100);
			Buff.Format(L"������ %.0f %%", (100 - pdhValue.longValue / DlgC->m_dwMem * 100));
			DlgC->m_NumMem.SetWindowText(Buff);
		}


		//ˢ��CPU/MEM����;
		DlgC->InsertCharData(dCPUData, dMEMData);


		//Ӳ���ϴ�����;
		status = PdhGetFormattedCounterValue(cntDiskRead, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
		if (status != ERROR_SUCCESS)
		{
			DlgC->m_NumDiskRead.SetWindowText(L"Ӳ�̶�ȡ���ݲ�ѯʧ��!");
		}
		else
		{
			Buff.Format(L"��ȡ: %.2f MB /��", pdhValue.doubleValue / 1024 / 1024);
			DlgC->m_NumDiskRead.SetWindowText(Buff);
		}

		status = PdhGetFormattedCounterValue(cntDiskWrite, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
		if (status != ERROR_SUCCESS)
		{
			DlgC->m_NumDiskWirte.SetWindowText(L"Ӳ��д�����ݲ�ѯʧ��!");
		}
		else
		{
			Buff.Format(L"д��: %.2f MB /��", pdhValue.doubleValue / 1024 / 1024);
			DlgC->m_NumDiskWirte.SetWindowText(Buff);
		}

		
		//forѭ����ȡ��������;
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

		//���;
		Buff.Format(L"����: %.2f KB /��", dRece / 1024);
		DlgC->m_NumNetDown.SetWindowText(Buff);

		Buff.Format(L"�ϴ�: %.2f KB /��", dSend / 1024);
		DlgC->m_NumNetUP.SetWindowText(Buff);
	}

	PdhCloseQuery(query);
	return 0;
}

//****************************************************************
// Brief 	: ��ȡ�ڴ��С;
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
// Brief 	: ����ͼ��;
// Method	: Paint
// FullName	: CMyDlgC::Paint
// Access	: public 
// Returns	: void
//****************************************************************

void CMyDlgC::Paint()
{
	//�ر�ˢ��;
	m_ChartCPU1.EnableRefresh(false);

	m_ChartCPU1.RemoveAllSeries();	//���;	


	//����CPU;
	CChartLineSerie *pLineSerie1;
	pLineSerie1 = m_ChartCPU1.CreateLineSerie();
	pLineSerie1->AddPoints(m_X, m_CPUArr, 100);
	pLineSerie1->SetName(_T("CPU"));
	pLineSerie1->SetColor(RGB(94, 213, 209));

	//����MEM;
	CChartLineSerie *pLineSerie2;
	pLineSerie2 = m_ChartCPU1.CreateLineSerie();
	pLineSerie2->AddPoints(m_X, m_MEMArr, 100);
	pLineSerie2->SetName(L"�ڴ�");
	pLineSerie2->SetColor(RGB(241, 124, 103));


	//��ӱ�ǩ;
	CChartBalloonLabel<SChartXYPoint>* pLable =
		pLineSerie1->CreateBalloonLabel(96, _T("CPU"));
	pLable->SetFont(labelFont);


	//��ӱ�ǩ;
	CChartBalloonLabel<SChartXYPoint>* pLable2 =
		pLineSerie2->CreateBalloonLabel(96, _T("�ڴ�"));
	pLable2->SetFont(labelFont2);


	//����ˢ��;
	m_ChartCPU1.EnableRefresh(true);
	int n = 0;
}

//****************************************************************
// Brief 	: ��������;
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


// CMyDlgC ��Ϣ�������


//****************************************************************
// Brief 	: ��ʼ��;
// Method	: OnInitDialog
// FullName	: CMyDlgC::OnInitDialog
// Access	: virtual protected 
// Returns	: BOOL
//****************************************************************

BOOL CMyDlgC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ȡ�ڴ��С;
	GetMem();

	//��ʼ������;
	InitFont();

	m_bClose = true;

	//�����߳�ˢ��ʹ����;
	hThread=AfxBeginThread(MyThreadFunction, this);

	//��ʼ��ͼ��;
	InitChart();

	//���ö�ʱ��;
	SetTimer(0, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


//****************************************************************
// Brief 	: ��ʱ������ͼ��;
// Method	: OnTimer
// FullName	: CMyDlgC::OnTimer
// Access	: protected 
// Returns	: void
// Parameter: UINT_PTR nIDEvent
//****************************************************************

void CMyDlgC::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (nIDEvent == 0)
	{
		//��ͼ;
		Paint();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CMyDlgC::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�ر��߳�;
	m_bClose = false;

	CDialogEx::OnClose();
}
