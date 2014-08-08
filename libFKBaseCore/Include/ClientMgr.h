//*************************************************************************
//	��������:	2014-8-6   14:06
//	�ļ�����:	ClientMgr.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCommon/FKCommonDef.h"
#include "../../FKCommon/IFKClientSocket.h"
#include "../../FKCommon/DataStructure/TSwitchList.h"
#include <vector>
using std::vector;
//-------------------------------------------------------------------------
enum ENUM_ClientMgrConnectType
{
	eCMCT_Success		= 0,		// ���ӳɹ�
	eCMCT_Recv,						// ������
	eCMCT_ConnectError,				// ����ʧ��
	eCMCT_Close,					// ���ӹر�
	eCMCT_MemError,					// �ڴ����
	eCMCT_UnknownError,				// δ֪����
};
//-------------------------------------------------------------------------
struct SClientMgrCmd
{
	unsigned long			m_dwType;
	unsigned long			m_dwIndex;
	unsigned long			m_dwClientSocket;
	unsigned long			m_dwLen;
	vector<char>			m_vecData;

	SClientMgrCmd(){
		m_dwType = m_dwIndex = m_dwClientSocket = m_dwLen = 0;
	}

	SClientMgrCmd( const SClientMgrCmd& p_Other )
	{
		m_dwType		= p_Other.m_dwType;
		m_dwIndex		= p_Other.m_dwIndex;
		m_dwClientSocket= p_Other.m_dwClientSocket;
		m_dwLen			= p_Other.m_dwLen;
		m_vecData.clear();
		
		if( m_dwLen > 0 && m_dwLen <= MAX_RECVBUF_LEN )
		{
			m_vecData.resize( m_dwLen );
			if( m_vecData.capacity() >= m_dwLen )
			{
				std::copy( p_Other.m_vecData.begin(), p_Other.m_vecData.end(),
					m_vecData.begin() );
			}
			else
			{
				// �ڴ����ʧ��
				m_dwType = eCMCT_MemError;
				m_dwLen = 0;
			}
		}
		else
		{
			m_dwType = eCMCT_UnknownError;
			m_dwLen = 0;
		}
	}

	SClientMgrCmd& operator =( const SClientMgrCmd& p_Other )
	{
		m_dwType		= p_Other.m_dwType;
		m_dwIndex		= p_Other.m_dwIndex;
		m_dwClientSocket= p_Other.m_dwClientSocket;
		m_dwLen			= p_Other.m_dwLen;
		m_vecData.clear();

		if( m_dwLen > 0 && m_dwLen <= MAX_RECVBUF_LEN )
		{
			m_vecData.resize( m_dwLen );
			if( m_vecData.capacity() >= m_dwLen )
			{
				std::copy( p_Other.m_vecData.begin(), p_Other.m_vecData.end(),
					m_vecData.begin() );
			}
			else
			{
				// �ڴ����ʧ��
				m_dwType = eCMCT_MemError;
				m_dwLen = 0;
			}
		}
		else
		{
			m_dwType = eCMCT_UnknownError;
			m_dwLen = 0;
		}

		return *this;
	}
};
//-------------------------------------------------------------------------
class CClientMgr : public IClientMgr
{
public:
	CClientMgr();
	~CClientMgr();
public:
	// ����
	virtual bool	Create();
	// ����
	virtual void	Release();

public:
	// �����ͻ���socket
	virtual IClientSocket* CreateClientSocket( bool p_bEncrypt, IClientSocketSink* p_pClientSocketSink );
	// ����ͻ���socket
	virtual void	ClearClientSocket( unsigned long p_dwIndex );

public:
	// �߼��̻߳ص�����
	virtual void	OnEvent();
	// ���ÿͻ��˻ص�����
	virtual void	SetLinkSink( unsigned long p_dwIndex, IClientSocketSink* p_pLinkSink );

public:
	// ���ӳɹ�
	void			PushConnectSuccess( unsigned long p_dwIndex, IClientSocket* p_pClientSocket );
	// ���Ӵ���
	void			PushConnectError( unsigned long p_dwIndex, int p_nError, const char* p_szErrorMsg );
	// ��������
	void			PushRecv( unsigned long p_dwIndex, const char* p_szBuf, int p_nLen );
	// ���ӹر�
	void			PushClose( unsigned long p_dwIndex );
private:
	// ���ӳɹ�����
	void			__OnConnectSuccess( unsigned long p_dwIndex, IClientSocket* p_pClientSocket );
	// ���Ӵ�����
	void			__OnConnectError( unsigned long p_dwIndex, int p_nError, const char* p_szErrorMsg );
	// �������ݴ���
	void			__OnRecv( unsigned long p_dwIndex, const char* p_szBuf, int p_nLen );
	// ���ӹرմ���
	void			__OnClose( unsigned long p_dwIndex );

private:
	unsigned long					m_dwIndex;				// ���
	vector< IClientSocketSink* >	m_vecClientSocketSink;	// �ص������б�
	TSwitchList< SClientMgrCmd >	m_listCmdList;			// �������	
};