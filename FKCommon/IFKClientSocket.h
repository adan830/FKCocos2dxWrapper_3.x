//*************************************************************************
//	��������:	2014-8-4   15:39
//	�ļ�����:	IFKClientSocket.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	�ͻ���socket
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCommonDef.h"
//-------------------------------------------------------------------------
// �Ƽ�����
//-------------------------------------------------------------------------
// �ͻ������socket����
#define MAX_CLIENT_SOCKET_NUM	16
// ��������/������Ϣ��󳤶�
#define MAX_RECVBUF_LEN			8192
#define MAX_SENDBUF_LEN			8192
//-------------------------------------------------------------------------
struct IClientSocket;
// �ͻ������ӻص�����
struct IClientSocketSink
{
	// �ͻ��˽�������
	virtual void	OnRecv( const char* p_szBuf, int32 p_nLen ) = 0;
	// �ͻ������ӹر�
	virtual void	OnCloseConnect() = 0;
	// �ͻ������ӳɹ�
	virtual void	OnConnectOK( IClientSocket* p_pClientSocket ) = 0;
	// �ͻ������Ӵ���
	virtual void	OnConnectError( int p_nSocketID, int p_nErrorCode, const char* p_szErrorMsg ) = 0;
};
//-------------------------------------------------------------------------
// �ͻ�������socket
struct IClientSocket
{
	// �ͷ�
	virtual void	Release() = 0;
	
public:
	// ���ÿͻ��˻ص�����
	virtual void	SetLinkSink( IClientSocketSink* p_pLinkSink ) = 0;
	// ���ӷ�����
	virtual bool	Connect( const char* p_szServerIP, uint16 p_usServerPort ) = 0;
	// �����ر�����
	virtual void	CloseConnect() = 0;
	// ��������
	virtual bool	Send( const char* p_szBuf, int32 p_nLen ) = 0;
	// ��ȡsocketID
	virtual int32	GetSocketID() = 0;
	// ��ȡ�ͻ���״̬
	virtual bool	IsConnect() = 0;
};
//-------------------------------------------------------------------------
// �ͻ��˹�����
struct IClientMgr
{
	// ����
	virtual bool	Create() = 0;
	// ����
	virtual void	Release() = 0;

public:
	// �����ͻ���socket
	virtual IClientSocket* CreateClientSocket( bool p_bEncrypt, IClientSocketSink* p_pClientSocketSink ) = 0;
	// ����ͻ���socket
	virtual void	ClearClientSocket( unsigned long p_dwIndex ) = 0;

public:
	// �߼��̻߳ص�����
	virtual void	OnEvent() = 0;
	// ���ÿͻ��˻ص�����
	virtual void	SetLinkSink( unsigned long p_dwIndex, IClientSocketSink* p_pLinkSink ) = 0;
};