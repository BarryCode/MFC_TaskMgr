#pragma once

class CMyList : public CListCtrl
{
	DECLARE_DYNAMIC(CMyList)

public:
	CMyList();
	virtual ~CMyList();

protected:
	DECLARE_MESSAGE_MAP()
	//排序;
	static bool Isint;		//是否int型排序;默认是;
	static bool method;		//排序方向;
	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);		//排序回调函数：
public:
	void AddColumn(DWORD dwCount, ...);					//插入列;
	void AddItem(DWORD dwCount, ...);					//插入行;
	void SortItemsPlus(int nColum, bool Isint = true);	//排序;
};

//排序结构体;
typedef struct  _DATA
{
	int SubItem;
	CMyList* pList;
}DATA;

