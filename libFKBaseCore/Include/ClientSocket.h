//*************************************************************************
//	��������:	2014-8-6   18:05
//	�ļ�����:	ClientSocket.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCommon/IFKClientSocket.h"
#include "ClientSendThread.h"
#include <thread>
using std::thread;
//-------------------------------------------------------------------------
enum ENUM_SocketConnectState
{
	eSCS_Connnecting	=	0,	// ��������
	eSCS_Connected,				// ������
	eSCS_ConnectClosed,			// ���������
};
//-------------------------------------------------------------------------
class CClientMgr;
class CClientSocket : public IClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
public:
	// ���������ͻ���socket
	bool			Create( unsigned long p_dwIndex, CClientMgr* p_pClientMgr );
	// �ͷ�
	virtual void	Release();

public:
	// ���ÿͻ��˻ص�����
	virtual void	SetLinkSink( IClientSocketSink* p_pLinkSink );
	// ���ӷ�����
	virtual bool	Connect( const char* p_szServerIP, uint16 p_usServerPort );
	// �����ر�����
	virtual void	CloseConnect();
	// ��������
	virtual bool	Send( const char* p_szBuf, int32 p_nLen );
	// ��ȡsocketID
	virtual int32	GetSocketID();
	// ��ȡ�ͻ���״̬
	virtual bool	IsConnect();
	// WSA�¼�����
	bool			OnWSAEvent( int p_nSocket );
	// ���ӳɹ�
	bool			OnConnectSuccess();
	// ����ʧ��
	bool			OnConnectError( const char* p_szErrorMsg, int p_nErrorCode );
	// ����
	bool			OnRecv();
	// �ر���Ϣ
	bool			OnClose();
	// �����������հ�
	bool			ProcFullPackage( unsigned char* p_szBuf, uint16 p_usLen );
	// ��ȡsocket
	int				GetSocket();
	// ���socket
	void			ClearSocket();
	// �Ƿ��˳�
	bool			IsExit();
private:
	bool				m_bIsExit;			// �Ƿ����
	unsigned long		m_dwIndex;			// socketID
	CClientMgr*			m_pClientMgr;		// �ͻ��˹�����
	CClientSendThread	m_ClientSendThread;	// �ͻ��˷����߳�
	int					m_nSocket;			// �ͻ���socket
	int					m_nRecvBytes;		// ���ճ���
	int					m_nRecvLeaveLen;	// ����ʣ�೤��
	unsigned char		m_szBuffer[MAX_RECVBUF_LEN];	// �������
	char				m_szRecvBuf[MAX_RECVBUF_LEN];	// �������ջ���
	void*				m_hThread;			// �߳̾��
	bool				m_bIsConnect;		// �Ƿ�������
	thread*				m_pIOThread;		// IO�����߳�
	char				m_szServerIP[18];	// ������IP
	uint16				m_usServerPort;		// �������˿�
};
//-------------------------------------------------------------------------