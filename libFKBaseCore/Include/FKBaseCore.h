//*************************************************************************
//	��������:	2014-8-6   14:01
//	�ļ�����:	FKBaseCore.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCommon/IFKBaseCore.h"
//-------------------------------------------------------------------------
class CBaseCore : public IBaseCore
{
public:
	CBaseCore();
	virtual ~CBaseCore();
public:
	// ����
	virtual void	Release();
	// ����
	virtual bool	Create();

public:
	// ����socket
	virtual bool	CreateClientSocket( IClientSocketSink* p_pClientSocketSink,
		IClientSocket** p_ppClientSocket, bool p_bEncrypt = false );
	// ����ʱ����
	virtual bool	CreateTimerAxis( ITimerAxis** p_ppTimerAxis );
	// �����¼�����
	virtual bool	CreateEventEngine( IEventEngine** p_ppEventEngine );
	// �����ű�ϵͳ
	virtual bool	CreateScriptLoader( IScriptLoader** p_ppScriptLoader );
	// ������Ϣ�ַ���
	virtual bool	CreateMessageDispath( IMessageDispatch** p_ppMessageDispath );
public:
	// ���º���
	virtual void	Update();
private:
	CClientMgr				m_ClientMgr;
};