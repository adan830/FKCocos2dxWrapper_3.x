//*************************************************************************
//	��������:	2014-8-4   16:25
//	�ļ�����:	IFKEventEngine.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCommonDef.h"
//-------------------------------------------------------------------------
// ͶƱ�¼�
struct IEventVoteSink
{
	/** 
	@param   p_ucEventID	���¼�ID
	@param   p_ucSrcType	������Դ����
	@param   p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0��
	@param   p_szContext	: ������
	@param   p_nLen			: �����ĳ���
	@return  �������false	: ���ж�ִ�У������������ִ��
	*/
	virtual bool	OnVote( uint8 p_ucEventID, uint8 p_ucSrcType, uint32 p_unSrcID,
		const char* p_szContext, int p_nLen ) = 0;
};
//-------------------------------------------------------------------------
// ִ���¼�
struct IEventExecuteSink
{
	/** 
	@param   p_ucEventID	���¼�ID
	@param   p_ucSrcType	������Դ����
	@param   p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0��
	@param   p_szContext	: ������
	@param   p_nLen			: �����ĳ���
	*/
	virtual void	OnExecute( uint8 p_ucEventID, uint8 p_ucSrcType, uint32 p_unSrcID,
		const char* p_szContext, int p_nLen ) = 0;
};
//-------------------------------------------------------------------------
// �¼�����
struct IEventEngine
{
	// ����
	virtual bool	Create() = 0;
	// �ͷ�
	virtual void	Release() = 0;
	
public:
	// ����ͶƱ�¼�   
	virtual bool	FireVote( uint8 p_ucEventID, uint8 p_ucSrcType, uint32 p_unSrcID,
		const char* p_szContext, int p_nLen ) = 0;
	// ����ִ���¼�
	virtual void	FireExecute( uint8 p_ucEventID, uint8 p_ucSrcType, uint32 p_unSrcID,
		const char* p_szContext, int p_nLen ) = 0;

	/** ����ͶƱ�¼�
	@param   p_pSink		: �ص�sink
	@param   p_ucEventID	: �¼�ID
	@param   p_ucSrcType	������Դ����
	@param	 p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0�������Ϊ0�����ʾ��������
	@param	 p_szDesc		: �������������������Գ���
	@return  �ɹ�����true,ʧ�ܷ���false 
	*/
	virtual bool	Subscibe( IEventVoteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID, const char* p_szDesc ) = 0;

	/** ����ִ���¼�
	@param   p_pSink		: �ص�sink
	@param   p_ucEventID	: �¼�ID
	@param   p_ucSrcType	������Դ����
	@param	 p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0�������Ϊ0�����ʾ��������
	@param	 p_szDesc		: �������������������Գ���
	@return  �ɹ�����true,ʧ�ܷ���false 
	*/
	virtual bool	Subscibe( IEventExecuteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID, const char* p_szDesc ) = 0;

	/** ȡ������ͶƱ�¼�
	@param   p_pSink		: �ص�sink
	@param   p_ucEventID	: �¼�ID
	@param   p_ucSrcType	������Դ����
	@param	 p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0�������Ϊ0�����ʾ��������
	@return  �ɹ�����true,ʧ�ܷ���false 
	*/
	virtual bool	UnSubscibe( IEventVoteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID ) = 0;

	/** ȡ������ִ���¼�
	@param   p_pSink		: �ص�sink
	@param   p_ucEventID	: �¼�ID
	@param   p_ucSrcType	������Դ����
	@param	 p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0�������Ϊ0�����ʾ��������
	@return  �ɹ�����true,ʧ�ܷ���false 
	*/
	virtual bool	UnSubscibe( IEventExecuteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID ) = 0;
};
//-------------------------------------------------------------------------