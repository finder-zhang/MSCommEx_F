
#include "StdAfx.h"
#include "mscomm.h"
#include "MSCommEx_F.h"

CMSCommEx_F::CMSCommEx_F(void)
{
	_pfnRxHandler = NULL;
}

CMSCommEx_F::~CMSCommEx_F(void)
{
}

BOOL CMSCommEx_F::Create(CWnd* pParentWnd)
{
	return Create(
		_T("MSCOMM_WND"),		//这个名称是可以任意的，为""也行
		WS_CHILD,				//这个为0也是可以的
		CRect(0,0,100,100),		//这个矩形也是任意的，CRect(0,0,0,0)也行
		pParentWnd,
		IDC_CTRL_MSCOMM);
}

BOOL CMSCommEx_F::Open(int iCommNum,int iBaudRate)
{
	BOOL bRet = 1;
	TRY 
	{
		if (get_PortOpen())
			put_PortOpen(0);	//如果之前已开，先关掉

		put_CommPort(iCommNum);		

		put_RTSEnable(0);
		put_DTREnable(0);
		put_EOFEnable(0);
		put_Handshaking(NoHandshaking);
		put_InBufferSize(1024);
		put_InputLen(0);
		put_InputMode(comInputModeBinary);	//二进制方式
		put_NullDiscard(0);
		put_OutBufferSize(512);
		put_ParityReplace(_T("?"));
		put_RThreshold(1);
		put_SThreshold(0);

		CString str;
		str.Format(_T("%d,n,8,1"),iBaudRate);
		put_Settings(str);

		if (!get_PortOpen())
			put_PortOpen(1);	//如果之前未开，现在开

		get_Input();			//清旧内容
	}
	CATCH (CException, e)
	{
		TCHAR tch[1024];
		e->GetErrorMessage(tch,1024);
		TRACE(tch);
		bRet = 0;
	}
	END_CATCH
	return bRet;
}

void CMSCommEx_F::Close()
{
	if (get_PortOpen())
		put_PortOpen(0);	//如果是开着的，则关掉
}

size_t CMSCommEx_F::Read(void* pData,size_t iLen)
{
	BYTE* pbyData = (BYTE*)pData;
	size_t i = 0;
	for (i=0; i<iLen; ++i) {
		if ( _queRxBytes.empty() ) {
			break;
		}
		*pbyData = _queRxBytes.front();
		_queRxBytes.pop();
		++pbyData;
	}
	return i;
}
size_t CMSCommEx_F::Write(const void* pData,size_t iLen)
{
	size_t i=0;
	CByteArray binData;
	binData.RemoveAll();
	const BYTE* pbyBuf = (const BYTE*)pData;
	for (i=0;i<iLen;i++)
	{
		binData.Add(*(pbyBuf+i));
	}
	put_Output(COleVariant(binData));
	return iLen;
}

fnRxHandler CMSCommEx_F::SetRxHandler(fnRxHandler pfn)
{
	fnRxHandler pfnOld = _pfnRxHandler;
	_pfnRxHandler = pfn;
	return pfnOld;
}

//窗体接收事件定义
BEGIN_EVENTSINK_MAP(CMSCommEx_F, CMSComm)
	//{{AFX_EVENTSINK_MAP(CMarker02Dlg)
	ON_EVENT(CMSCommEx_F, IDC_CTRL_MSCOMM, 1 /* OnComm */, BaseRxHandler, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//窗体接收事件处理函数
void CMSCommEx_F::BaseRxHandler(void)
{
	COleVariant myVar;
	COleSafeArray safearray_inp;
	BYTE byRxBuf;	
	myVar.Attach(get_Input());
	safearray_inp = myVar;
	
	DWORD index = 0;
	//int iLen = safearray_inp.GetElemSize();
	//int iLen = safearray_inp.GetDim();
	DWORD iLen = safearray_inp.GetOneDimSize();
	for (index=0; index<iLen; ++index)
	{
		safearray_inp.GetElement((long*)&index,&byRxBuf);
		_queRxBytes.push(byRxBuf);
		TRACE("Rx = 0x%02x\r\n",byRxBuf);
	}

	if ( _pfnRxHandler ) {
		_pfnRxHandler();
	}
}

