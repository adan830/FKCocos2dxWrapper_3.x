//-------------------------------------------------------------------------
#include "../include/ClientSocket.h"
#include "../Include/ClientMgr.h"
#include "../include/TCPWarpper.h"
//-------------------------------------------------------------------------
// �ͻ���IO��������
static void* ClientIOThreadProc( void* p_pClientSocket )
{
	CClientSocket* pClientSocket = (CClientSocket*)p_pClientSocket;
	if( pClientSocket == NULL )
		return NULL;

	int nSocket = pClientSocket->GetSocket();
	unsigned long dwEventIndex = 0;

	while( !pClientSocket->IsExit() )
	{
		if( !pClientSocket->OnWSAEvent(nSocket) )
		{
			return NULL;
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
CClientSocket::CClientSocket()
	: m_bIsExit( false )
	, m_pClientMgr( NULL )
	, m_dwIndex( 0 )
	, m_nSocket( INVALID_SOCKET )
	, m_nRecvLeaveLen( 0 )
	, m_bIsConnect( false )
	, m_pIOThread( NULL )
{
	memset( m_szBuffer, 0, sizeof(m_szBuffer) );
}
//-------------------------------------------------------------------------
CClientSocket::~CClientSocket()
{
	// ֹͣ�ͻ����߳�
	if( m_pIOThread != NULL )
	{
		m_bIsExit = true;
		m_pIOThread->join();
		m_pIOThread->detach();
		// �Ƿ�Ҫdelete��ֵ����ȶ
		delete m_pIOThread;
		m_pIOThread = NULL;
	}

	// ����socket
	ClearSocket();

	// ��mgr�Ƴ�
	if( m_pClientMgr != NULL )
	{
		m_pClientMgr->ClearClientSocket( m_dwIndex );
		m_pClientMgr = NULL;
	}
}
//-------------------------------------------------------------------------
// ���������ͻ���socket
bool CClientSocket::Create( unsigned long p_dwIndex, CClientMgr* p_pClientMgr )
{
	if( p_pClientMgr == NULL )
		return false;

	m_pClientMgr = p_pClientMgr;
	m_dwIndex = p_dwIndex;

	return true;
}
//-------------------------------------------------------------------------
// �ͷ�
void CClientSocket::Release()
{
	CloseConnect();
	delete this;
}
//-------------------------------------------------------------------------
// ���ÿͻ��˻ص�����
void CClientSocket::SetLinkSink( IClientSocketSink* p_pLinkSink )
{
	if( m_pClientMgr )
	{
		m_pClientMgr->SetLinkSink( m_dwIndex, p_pLinkSink );
	}
}
//-------------------------------------------------------------------------
// ���ӷ�����
bool CClientSocket::Connect( const char* p_szServerIP, uint16 p_usServerPort )
{
	FKLOG("try to connect server: ip = %s, port = %d", p_szServerIP, p_usServerPort );
	// ����TCP Socket
	m_nSocket = create_tcp_socket();

	// ���ý��ܻ���
	int nRecvBufSize = 65535;
	if( setsockopt( m_nSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nRecvBufSize, sizeof(int) ))
	{
		close_tcp_socket( m_nSocket );
		m_nSocket = INVALID_SOCKET;
		FKLOG("Close socket: ip = %s, port = %d", p_szServerIP, p_usServerPort );
		return false;
	}

	// ���÷��ͻ���
	int nSendBufSize = 65535;
	if( setsockopt( m_nSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nSendBufSize, sizeof(int) ))
	{
		close_tcp_socket( m_nSocket );
		m_nSocket = INVALID_SOCKET;
		FKLOG("Close socket: ip = %s, port = %d", p_szServerIP, p_usServerPort );
		return false;
	}

	// ���ý�ֹʹ��Nagle�ϲ���
	bool bNoDelay = true;
	if( setsockopt( m_nSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bool) ))
	{
		close_tcp_socket( m_nSocket );
		m_nSocket = INVALID_SOCKET;
		FKLOG("Close socket: ip = %s, port = %d", p_szServerIP, p_usServerPort );
		return false;
	}

	int nTimeOut = 5000;
	// ���ý��պͷ��ͳ�ʱ
	if( setsockopt( m_nSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&nTimeOut, sizeof(int)))
	{
		close_tcp_socket( m_nSocket );
		m_nSocket = INVALID_SOCKET;
		FKLOG("Close socket: ip = %s, port = %d", p_szServerIP, p_usServerPort );
		return false;
	}
	if( setsockopt( m_nSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&nTimeOut, sizeof(int)))
	{
		close_tcp_socket( m_nSocket );
		m_nSocket = INVALID_SOCKET;
		FKLOG("Close socket: ip = %s, port = %d", p_szServerIP, p_usServerPort );
		return false;
	}

	// ��ʵ����
	int nRet = connect_server( m_nSocket, (char*)p_szServerIP, p_usServerPort );
	if( nRet != 0 )
	{
		close_tcp_socket( m_nSocket );
		m_nSocket = INVALID_SOCKET;
		FKLOG("Connect Server failed! Close socket: ip = %s, port = %d", p_szServerIP, p_usServerPort );
		return false;
	}
	
	// ���ӳɹ�,�����߳̽���

	m_pIOThread = new thread( ClientIOThreadProc, (void*)this );
	if( m_pIOThread == NULL )
	{
		close_tcp_socket( m_nSocket );
		m_nSocket = INVALID_SOCKET;
		FKLOG("Connect Server failed! Close socket: ip = %s, port = %d", p_szServerIP, p_usServerPort );
		return false;
	}

	// �ص�
	OnConnectSuccess();

	// ��¼
	strcpy_s( m_szServerIP, 18, p_szServerIP );
	m_usServerPort =  p_usServerPort;

	FKLOG("connect server success: ip = %s, port = %d", p_szServerIP, p_usServerPort );

	return true;
}
//-------------------------------------------------------------------------
// �����ر�����
void CClientSocket::CloseConnect()
{
	m_bIsConnect = false;

	ClearSocket();

	if( m_pClientMgr )
	{
		m_pClientMgr->PushClose( m_dwIndex );
	}
}
//-------------------------------------------------------------------------
// ��������
bool CClientSocket::Send( const char* p_szBuf, int32 p_nLen )
{
	if( p_szBuf == NULL || p_nLen < 0 || p_nLen >= MAX_SENDBUF_LEN )
		return false;

	m_ClientSendThread.PushSend( p_szBuf, p_nLen );
	return true;
}
//-------------------------------------------------------------------------
// ��ȡsocketID
int32 CClientSocket::GetSocketID()
{
	return m_dwIndex;
}
//-------------------------------------------------------------------------
// ��ȡ�ͻ���״̬
bool CClientSocket::IsConnect()
{
	return m_bIsConnect;
}
//-------------------------------------------------------------------------
// WSA�¼�����
bool CClientSocket::OnWSAEvent( int p_nSocket )
{
	fd_set	tagFDSet;

	// ����ļ������������������ļ���
	int		nMaxSocket = p_nSocket;
	// ��ʼ���ļ�����������
	FD_ZERO( &tagFDSet );
	FD_SET( m_nSocket, &tagFDSet );

	/*
	//Select ����ԭ��
	//			int select��nfds�� readfds�� writefds�� exceptfds�� timeout��
	//nfds:		select���ӵ��ļ������,�ӽ����д򿪵��ļ���������һ����ΪҪ���Ӹ��ļ��е�����ļ��ż�һ
	//readfds:	select���ӵĿɶ��ļ��������
	//writefds:	select���ӵĿ�д�ļ�������ϡ�
	//exceptfds:select���ӵ��쳣�ļ�������ϡ�
	//timeout:	����select�ĳ�ʱ����ʱ�䡣
	*/
	int nRet = select( nMaxSocket + 1, &tagFDSet, NULL, NULL, NULL );
	if( FD_ISSET( p_nSocket, &tagFDSet ) )
	{
		m_nRecvBytes = recv( p_nSocket, m_szRecvBuf, sizeof( m_szRecvBuf ), 0 );
		if( m_nRecvBytes > 0 )
		{
			OnRecv();
		}
		else if( m_nRecvBytes == -1 )
		{
			if( m_bIsConnect )
			{
				CloseConnect();
			}
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------
// ���ӳɹ�
bool CClientSocket::OnConnectSuccess()
{
	if( !m_ClientSendThread.Start( m_nSocket ) )
	{
		return false;
	}

	m_bIsConnect = true;

	if( m_pClientMgr )
	{
		m_pClientMgr->PushConnectSuccess( m_dwIndex, this );
	}
	return true;
}
//-------------------------------------------------------------------------
// ����ʧ��
bool CClientSocket::OnConnectError( const char* p_szErrorMsg, int p_nErrorCode )
{
	m_bIsConnect = false;
	ClearSocket();

	if( m_pClientMgr != NULL )
	{
		m_pClientMgr->PushConnectError( m_dwIndex, p_nErrorCode, p_szErrorMsg );
	}
	return true;
}
//-------------------------------------------------------------------------
// ����
bool CClientSocket::OnRecv()
{
	char* pBuf = m_szRecvBuf;
	int nLen = m_nRecvBytes;

	if( pBuf == NULL || nLen <= 0 )
		return false;

	int nBufOffset = 0;		// ƫ����
	uint16 usCmdLen = 0;

	// �ϴν��հ���ʣ���δ������
	if( m_nRecvLeaveLen > 0 )
	{
		if( m_nRecvLeaveLen == sizeof(unsigned char) )
		{
			nBufOffset = 1;	// ����һ���ֽ�
			memcpy( m_szBuffer + m_nRecvLeaveLen, pBuf, nBufOffset );

			pBuf += nBufOffset;
			nLen -= nBufOffset;
			m_nRecvLeaveLen += nBufOffset;
		}

		// �ȴ��´ν���
		if( nLen <= 0 )
		{
			return false;
		}

		if( m_nRecvLeaveLen >= sizeof(uint16) )
		{
			// ʣ�೤��������һ�� uint16 ��
			usCmdLen = *((uint16*) m_szBuffer );

			if( usCmdLen > MAX_RECVBUF_LEN )
			{
				// ���ȷǷ����ر�
				CloseConnect();
				return false;
			}

			// ����ʼ����
			if( m_nRecvLeaveLen >= (int)(usCmdLen + sizeof(uint16)) )
			{
				// ������
				nBufOffset = usCmdLen - m_nRecvLeaveLen + sizeof( uint16 );	// ��һ������
				memcpy( m_szBuffer + m_nRecvLeaveLen, pBuf, nBufOffset );

				if( !ProcFullPackage(&m_szBuffer[2], usCmdLen) )
				{
					return false;
				}

				pBuf += nBufOffset;
				nLen -= nBufOffset;
				m_nRecvLeaveLen = 0;
			}
			else
			{
				// ���ְ������������ȱ�������
				memcpy( m_szBuffer + m_nRecvLeaveLen, pBuf, nLen );
				m_nRecvLeaveLen += nLen;
				return true;
			}
		}
	}

	// ���ճ���
	if( m_nRecvLeaveLen < 0 )
	{
		CloseConnect();
		return false;
	}

	// ��һ�ν���
	while( true )
	{
		if( nLen <= 0 )
		{
			// �ȴ��´ν���
			return true;
		}

		if( nLen >= sizeof(uint16) )
		{
			usCmdLen = *((uint16*)pBuf);

			if( usCmdLen > MAX_RECVBUF_LEN )
			{
				// ���ȷǷ����ر�
				CloseConnect();
				return false;
			}

			if( nLen >= usCmdLen )
			{
				// ����һ��������
				if( !ProcFullPackage((unsigned char*)pBuf, usCmdLen ) )
				{
					return false;
				}

				pBuf += usCmdLen;
				nLen -= usCmdLen;
			}
			else
			{
				// �������İ�����������
				memcpy( m_szBuffer, pBuf, nLen );
				m_nRecvLeaveLen = nLen;
				return true;
			}
		}
		else
		{
			// ֻ��һ���ֽڣ����棬�ȴ��´ν���
			memcpy( m_szBuffer, pBuf, nLen );
			m_nRecvLeaveLen = nLen;
			return true;
		}
	}

	return true;
}
//-------------------------------------------------------------------------
// �ر���Ϣ
bool CClientSocket::OnClose()
{
	CloseConnect();
	return true;
}
//-------------------------------------------------------------------------
// �����������հ�
bool CClientSocket::ProcFullPackage( unsigned char* p_szBuf, uint16 p_usLen )
{
	if( m_pClientMgr == NULL )
		return true;

	m_pClientMgr->PushRecv( m_dwIndex, (const char*)p_szBuf, p_usLen );
	return true;
}
//-------------------------------------------------------------------------
// ��ȡsocket
int CClientSocket::GetSocket()
{
	return m_nSocket;
}
//-------------------------------------------------------------------------
// ���socket
void CClientSocket::ClearSocket()
{
	if( m_nSocket > 0 && m_nSocket != INVALID_SOCKET )
	{
		close_tcp_socket( m_nSocket );
	}
	m_ClientSendThread.Stop();
}
//-------------------------------------------------------------------------
// �Ƿ��˳�
bool CClientSocket::IsExit()
{
	return m_bIsExit;
}
//-------------------------------------------------------------------------