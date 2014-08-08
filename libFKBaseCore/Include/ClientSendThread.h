//*************************************************************************
//	��������:	2014-8-6   15:35
//	�ļ�����:	ClientSendThread.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCommon/FKCommonDef.h"
#include "../../FKCommon/IFKClientSocket.h"
#include "../../FKCommon/DataStructure/TSwitchList.h"
#include "../../FKCommon/BaseComponent/TSemaphore.h"
#include <vector>
#include <thread>
using std::vector;
using std::thread;
//-------------------------------------------------------------------------
struct SSendDataPack
{
	unsigned long		m_dwLen;		// ����
	vector<char>		m_vecData;		// ����

	SSendDataPack() 
		:m_dwLen ( 0 )
	{

	}

	SSendDataPack( const SSendDataPack& p_Other )
	{
		m_dwLen = p_Other.m_dwLen;
		m_vecData.clear();

		if( m_dwLen > 0 && m_dwLen <= MAX_SENDBUF_LEN )
		{
			m_vecData.resize( m_dwLen );
			if( m_vecData.capacity() >= m_dwLen )
			{
				std::copy( p_Other.m_vecData.begin(),
					p_Other.m_vecData.end(), m_vecData.begin() );
			}
			else
			{
				m_dwLen = 0;
			}
		}
	}

	SSendDataPack& operator = ( const SSendDataPack& p_Other )
	{
		m_dwLen = p_Other.m_dwLen;
		m_vecData.clear();

		if( m_dwLen > 0 && m_dwLen <= MAX_SENDBUF_LEN )
		{
			m_vecData.resize( m_dwLen );
			if( m_vecData.capacity() >= m_dwLen )
			{
				std::copy( p_Other.m_vecData.begin(),
					p_Other.m_vecData.end(), m_vecData.begin() );
			}
			else
			{
				m_dwLen = 0;
			}
		}

		return *this;
	}
};
//-------------------------------------------------------------------------
class CClientSendThread
{
public:
	CClientSendThread();
	~CClientSendThread();
public:
	// ����
	bool	Start( int p_nSocketID );
	// ֹͣ
	void	Stop();
public:
	// ������
	bool	PushSend( const char* p_szBuf, int p_nLen );
	// �յ������¼�
	void	OnSendEvent();
	// ��ʵ����
	void	OnSend( const char* p_szBuf, int p_nLen );
	// ��ȡ�ź���
	semaphore*	GetSemphore();
	// �ж��Ƿ�����˳�
	bool	IsExit();
private:
	bool						m_bEncrypt;				// �Ƿ����
	int							m_nSocketID;			// socket
	bool						m_bIsExit;				// �Ƿ��˳�
	thread*						m_pThreadHandle;		// �̶߳���
	bool						m_bIsHadThread;			// �߳��Ƿ񴴽�
	semaphore*					m_pSemphore;			// �ź���
	char						m_szTmpSendBuf[MAX_SENDBUF_LEN+2];	// ���ͻ���
	TSwitchList<SSendDataPack>	m_WaitingSendList;		// �ȴ����Ͷ���
};	
//-------------------------------------------------------------------------