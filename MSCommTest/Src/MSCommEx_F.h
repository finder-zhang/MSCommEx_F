#pragma once

#include "Mscomm.h"
#include <queue>
using namespace std;


typedef void (CALLBACK *fnRxHandler)(void);
const DWORD IDC_CTRL_MSCOMM = 1200;			//�ؼ�ID�ţ����Ը���Ӧ����Ҫ���޸ģ��������Զ����ɵ�ID���ظ�

class CMSCommEx_F : public CMSComm
{
	//FINDER �Լ��ӵĴ���
protected:	
	queue<BYTE>		_queRxBytes;		//����FIFO
	fnRxHandler		_pfnRxHandler;		//���մ�����
	afx_msg void BaseRxHandler(void);	//��������¼����������н����¼�ʱ�Զ��ص�

public:
	CMSCommEx_F(void);
	virtual ~CMSCommEx_F(void);

	using CMSComm::Create;			//ʹ����� Create ������Ȼ����ֱ�ӵ���
	BOOL Create(CWnd* pParentWnd);		//����һ��������õĴ�������

	BOOL Open(int iCommNum,int iBaudRate);
	void Close();

	size_t Read(void* pData,size_t iLen);
	size_t Write(const void* pData,size_t iLen);

	fnRxHandler SetRxHandler(fnRxHandler pfn);		//���ý����¼�������
		
	DECLARE_EVENTSINK_MAP()			//�����¼�����������ר��
public:

};
