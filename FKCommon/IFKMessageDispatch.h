//*************************************************************************
//	��������:	2014-8-4   17:53
//	�ļ�����:	IFKMessageDispatch.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCommonDef.h"
//-------------------------------------------------------------------------
// ������Ϣ������
struct IMessageHandler
{
	// ������Ϣ������
	virtual void		OnMessage( const char* p_szData, int p_nLen ) = 0;
};
//-------------------------------------------------------------------------
// ��Ϣ�ַ���
struct IMessageDispatch
{
	// �ͷ�
	virtual void		Release() = 0;

public:
	// ��Ϣ�ַ�����
	virtual void		DispatchMessage( const char* p_szData, int p_nLen ) = 0;
	// ע����Ϣ������
	virtual bool		RegisterMessageHandler( IMessageHandler* p_pHandler, uint8 p_ucMainMsgCode, uint16 p_usSubMsgCode ) = 0;
	// ע����Ϣ������
	virtual bool		UnregisterMessageHandler( IMessageHandler* p_pHandler, uint8 p_ucMainMsgCode, uint16 p_usSubMsgCode ) = 0;
};
//-------------------------------------------------------------------------