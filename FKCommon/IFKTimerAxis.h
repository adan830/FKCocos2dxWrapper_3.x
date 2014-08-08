//*************************************************************************
//	��������:	2014-8-4   15:51
//	�ļ�����:	IFKTimerAxis.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCommonDef.h"
//-------------------------------------------------------------------------
#ifndef _WIN32
#include <sys/time.h>
#endif
//-------------------------------------------------------------------------
// �Ƽ�����
//-------------------------------------------------------------------------
// ���Ƶ�� 16 ms
#define CHECK_FREQUENCY			16
// ʱ��̶ȵ�λ 64 ms
#define TIME_GRID				64
// ʱ�����ܳ��� ms [2����]
#define TIME_AXIS_LENGTH		720000
// ��Ч�Ķ�ʱ��ID
#define INVALID_TIMER			0xffffffff
// ��ʱ���ص����� �����
#define INFINITY_CALL			0xffffffff
//-------------------------------------------------------------------------
// ��ʱ���ص��ӿ�
struct ITimerHandler
{
	ITimerHandler()
		: m_pInfoPtr( NULL )
	{

	}
	// ��ʱ�������ص�
	virtual void	OnTimer( uint32 p_unEventID ) = 0;
	/**
	@purpose          : ȡ�ô�Ŷ�ʱ���ڲ����ݵ�ָ��
	@note             : ���������Ӧ�ò��޹�,����TimerHandler�����ʵ�ּ���
	@note             : ʱ�������ʵ������Ҫ��߶�ʱ���Ĳ���ɾ��Ч�ʣ�������Ӧ�ò��¼һ��ָ��
	@return		      : ����ָ����ʱ��Ϣ��ָ��,��ָ������ʼ����0
	*/
	virtual void**	GetTimerInfoPtr(){
		return &m_pInfoPtr;
	}

protected:
	void*	m_pInfoPtr;		// ָ����ʱ���ݵ�ָ��
};
//-------------------------------------------------------------------------
// ʱ����
struct ITimerAxis
{
	// ����
	virtual bool	Create() = 0;
	// ����
	virtual void	Release() = 0;

public:
	/**
	@purpose				: ����һ����ʱ��
	@param	 p_ulTimerID	: ��ʱ��ID
	@param   p_ulInterval	: ��ʱ�����ü��
	@param   p_pHandler		: ����ӿ�
	@param   p_ulCallTimes	: ���ô���,Ĭ�ϵ��������
	@param   p_szDebugInfo	: ������Ϣ
	@return					: ������óɹ��򷵻�true
	*/
	virtual bool	SetTimer( unsigned long p_ulTimerID, unsigned long p_ulInterval,
		ITimerHandler* p_pHandler, unsigned long p_ulCallTimes = INFINITY_CALL,
		const char* p_szDebugInfo = NULL ) = 0;

	/**
	@purpose				: ɾ����ʱ��
	@param	 p_ulTimerID	: ��ʱ��ID
	@param   p_pHandler		: ����ӿ�
	@return					: �����Ƿ�ɾ���ɹ�
	*/
	virtual bool	KillTimer( unsigned long p_ulTimerID, ITimerHandler* p_pHandler ) = 0;

	// ��ȡ��ǰʱ��
	inline unsigned long getTickCount() 
	{
#ifdef	_WIN32
		return ::GetTickCount();
#else
		struct timeval tv;
		if(gettimeofday(&tv,NULL)!=0)
			return 0;
		return (tv.tv_sec*1000) + (tv.tv_usec/1000);
#endif
	}
};
//-------------------------------------------------------------------------