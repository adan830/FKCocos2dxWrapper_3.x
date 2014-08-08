//*************************************************************************
//	��������:	2014-8-7   17:38
//	�ļ�����:	EventEngine.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCommon/FKCommonDef.h"
#include "../../FKCommon/IFKEventEngine.h"
#include "TEventEngine.h"
#include <list>
using std::list;
//-------------------------------------------------------------------------
struct OnVoteObject
{
	bool operator() ( IEventVoteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint8 p_ucSrcID, const char* p_szContext, int p_nLen )
	{
		if( p_pSink == NULL )
			return false;

		return p_pSink->OnVote(p_ucEventID, p_ucSrcType, p_ucSrcID, p_szContext, p_nLen );
	}
};
//-------------------------------------------------------------------------
struct OnExecuteObject
{
	bool operator() ( IEventExecuteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint8 p_ucSrcID, const char* p_szContext, int p_nLen )
	{
		if( p_pSink == NULL )
			return false;

		p_pSink->OnExecute( p_ucEventID, p_ucSrcType, p_ucSrcID, p_szContext, p_nLen );
		return true;
	}
};
//-------------------------------------------------------------------------
class CEventEngine : public IEventEngine
{
	typedef TEventEngine< IEventVoteSink, OnVoteObject >		CENTER_VOTE;
	typedef TEventEngine< IEventExecuteSink, OnExecuteObject >	CENTER_EXECUTE;
public:
	CEventEngine();
	virtual ~CEventEngine();

public:
		// ����
	virtual bool	Create();
	// �ͷ�
	virtual void	Release();
	
public:
	// ����ͶƱ�¼�   
	virtual bool	FireVote( uint8 p_ucEventID, uint8 p_ucSrcType, uint32 p_unSrcID,
		const char* p_szContext, int p_nLen );
	// ����ִ���¼�
	virtual void	FireExecute( uint8 p_ucEventID, uint8 p_ucSrcType, uint32 p_unSrcID,
		const char* p_szContext, int p_nLen );

	/** ����ͶƱ�¼�
	@param   p_pSink		: �ص�sink
	@param   p_ucEventID	: �¼�ID
	@param   p_ucSrcType	������Դ����
	@param	 p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0�������Ϊ0�����ʾ��������
	@param	 p_szDesc		: �������������������Գ���
	@return  �ɹ�����true,ʧ�ܷ���false 
	*/
	virtual bool	Subscibe( IEventVoteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID, const char* p_szDesc );

	/** ����ִ���¼�
	@param   p_pSink		: �ص�sink
	@param   p_ucEventID	: �¼�ID
	@param   p_ucSrcType	������Դ����
	@param	 p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0�������Ϊ0�����ʾ��������
	@param	 p_szDesc		: �������������������Գ���
	@return  �ɹ�����true,ʧ�ܷ���false 
	*/
	virtual bool	Subscibe( IEventExecuteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID, const char* p_szDesc );

	/** ȡ������ͶƱ�¼�
	@param   p_pSink		: �ص�sink
	@param   p_ucEventID	: �¼�ID
	@param   p_ucSrcType	������Դ����
	@param	 p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0�������Ϊ0�����ʾ��������
	@return  �ɹ�����true,ʧ�ܷ���false 
	*/
	virtual bool	UnSubscibe( IEventVoteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID );

	/** ȡ������ִ���¼�
	@param   p_pSink		: �ص�sink
	@param   p_ucEventID	: �¼�ID
	@param   p_ucSrcType	������Դ����
	@param	 p_unSrcID		: ����Դ��ʶ��ʵ��ΪUID��"���к�"���ݣ���ʵ���Ϊ0�������Ϊ0�����ʾ��������
	@return  �ɹ�����true,ʧ�ܷ���false 
	*/
	virtual bool	UnSubscibe( IEventExecuteSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID );

private:
	CENTER_VOTE		m_VoteCenter;		// ͶƱ����
	CENTER_EXECUTE	m_ExecuteCenter;	// ִ������
};
//-------------------------------------------------------------------------