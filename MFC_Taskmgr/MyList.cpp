// MyList.cpp : 实现文件
//

#include "stdafx.h"
#include "MyList.h"

bool CMyList::Isint = true;
bool CMyList::method = false;

IMPLEMENT_DYNAMIC(CMyList, CListCtrl)

CMyList::CMyList()
{

}

CMyList::~CMyList()
{
}


BEGIN_MESSAGE_MAP(CMyList, CListCtrl)

END_MESSAGE_MAP()



// CMyList 消息处理程序


//****************************************************************
// Brief 	: 添加列;
// Method	: AddColumn
// FullName	: CMyList::AddColumn
// Access	: public 
// Returns	: void
// Parameter: DWORD dwCount;参数组数量;
// Parameter: ...列名;列宽;
//****************************************************************

void CMyList::AddColumn(DWORD dwCount, ...)
{
	//设置列表框属性;
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	va_list vlist;
	va_start(vlist, dwCount);
	//循环获取参数;
	for (DWORD i = 0; i < dwCount; i++)
	{
		TCHAR * pName = va_arg(vlist, TCHAR*);
		int	nWidth = va_arg(vlist, int);

		//插入行;
		InsertColumn(i, pName, NULL, nWidth);
	}
	va_end(vlist);
}

//****************************************************************
// Brief 	: 插入行;
// Method	: AddItem
// FullName	: CMyList::AddItem
// Access	: public 
// Returns	: void
// Parameter: DWORD dwCount
// Parameter: ...行名;行名;
//****************************************************************

void CMyList::AddItem(DWORD dwCount, ...)
{
	//获取行数;
	int nItemNum = GetItemCount();

	//初始化;
	va_list vlist;
	va_start(vlist, dwCount);

	//添加行;
	TCHAR* pText = va_arg(vlist, TCHAR*);
	InsertItem(nItemNum, pText);

	//设置其他行;
	for (DWORD i = 1; i < dwCount; i++)
	{
		//设置内容;
		pText = va_arg(vlist, TCHAR*);
		SetItemText(nItemNum, i, pText);
	}

	va_end(vlist);
}


//****************************************************************
// Brief 	: 加强版排序;
// Method	: SortItemsPlus
// FullName	: CMyList::SortItemsPlus
// Access	: public 
// Returns	: void
// Parameter: int nColum	//行号
// Parameter: bool Isint	//是否int型排序;默认是;
//****************************************************************

void CMyList::SortItemsPlus(int nColum, bool Isint /*= true*/)
{
	this->Isint = Isint;
	//获取行数;
	int nCount = GetItemCount();
	for (int i = 0; i < nCount; i++)
		SetItemData(i, i);

	DATA dData;
	dData.SubItem = nColum;
	dData.pList = this;

	//反序排列;
	method = !method;

	//排序;
	SortItems(ListCompare, (LPARAM)&dData);
}

//****************************************************************
// Brief 	: 点击列头排序回调函数;		 
// Method	: ListCompare
// FullName	: CMyList::ListCompare
// Access	: public static 
// Returns	: int CALLBACK
// Parameter: LPARAM lParam1
// Parameter: LPARAM lParam2
// Parameter: LPARAM lParamSort
//****************************************************************

int CALLBACK CMyList::ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	DATA* pListCtrl = (DATA*)lParamSort;
	int Col = pListCtrl->SubItem;		//点击的列号;

	//得到该列前两项;
	CString StrItem1 = (pListCtrl->pList)->GetItemText(lParam1, Col);
	CString StrItem2 = (pListCtrl->pList)->GetItemText(lParam2, Col);

	//字符串排序;
	if (!Isint)
	{
		int nTemp = StrItem1.CompareNoCase(StrItem2);	//对比字符串;一样返回0，否则返回-1;

		if (method)	//true升序;
		{
			if (nTemp <= 0)
				return -1;
			else
				return 1;
		}
		else
		{
			if (nTemp >= 0)
				return -1;
			else
				return 1;
		}
	}
	//整型排序;
	else if (Isint)	
	{
		INT64 n1 = _wtoi64(StrItem1);
		INT64 n2 = _wtoi64(StrItem2);
		if (method)
		{
			if (n1 <= n2)
				return -1;
			else
				return 1;
		}

		else
		{
			if (n1 >= n2)
				return -1;
			else
				return 1;
		}

	}
	return -1;	//-1表示第一项在第二项前面,0表示两项相等;
}




