#pragma once

#include "Mscomm.h"
#include <queue>
using namespace std;


typedef void (CALLBACK *fnRxHandler)(void);
const DWORD IDC_CTRL_MSCOMM = 1200;			//控件ID号，可以根据应用需要而修改，避免与自动生成的ID号重复

class CMSCommEx_F : public CMSComm
{
	//FINDER 自己加的代码
protected:	
	queue<BYTE>		_queRxBytes;		//接收FIFO
	fnRxHandler		_pfnRxHandler;		//接收处理函数
	afx_msg void BaseRxHandler(void);	//窗体接收事件处理函数，有接收事件时自动回调

public:
	CMSCommEx_F(void);
	virtual ~CMSCommEx_F(void);

	using CMSComm::Create;			//使基类的 Create 方法仍然可以直接调用
	BOOL Create(CWnd* pParentWnd);		//给出一个最简单易用的创建方法

	BOOL Open(int iCommNum,int iBaudRate);
	void Close();

	size_t Read(void* pData,size_t iLen);
	size_t Write(const void* pData,size_t iLen);

	fnRxHandler SetRxHandler(fnRxHandler pfn);		//设置接收事件处理函数
		
	DECLARE_EVENTSINK_MAP()			//客体事件声明，接收专用
public:

};
