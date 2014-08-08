//*************************************************************************
//	��������:	2014-8-7   14:27
//	�ļ�����:	TimerAxis.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	ʱ����
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCommon/IFKTimerAxis.h"
#include <string>
#include <list>
#include <vector>
using std::string;
using std::list;
using std::vector;
//-------------------------------------------------------------------------
class CTimerAxis : public ITimerAxis
{
public:
	CTimerAxis();
	~CTimerAxis();
public:
		// ����
	virtual bool	Create();
	// ����
	virtual void	Release();

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
		const char* p_szDebugInfo = NULL );

	/**
	@purpose				: ɾ����ʱ��
	@param	 p_ulTimerID	: ��ʱ��ID
	@param   p_pHandler		: ����ӿ�
	@return					: �����Ƿ�ɾ���ɹ�
	*/
	virtual bool	KillTimer( unsigned long p_ulTimerID, ITimerHandler* p_pHandler );

	// ÿ֡����
	virtual void	CheckTimer();

protected:
	struct STimer
	{
		unsigned long		m_dwTimerID;
		unsigned long		m_dwInterval;		// ���ʱ��
		unsigned long		m_dwCallTimes;		// �ۼƵ��ô���
		unsigned long		m_dwLastCallTick;	// �ϴε���ʱ��
		unsigned long		m_dwGridIndex;		// ����ʱ��̶�
		ITimerHandler*		m_pHandler;			// �ص�����
		string				m_szDebugInfo;		// ������Ϣ
		list<STimer*>::iterator	m_ItePos;		// ��¼�Լ���ʱ�����е�Ite����������
	};

	typedef list<STimer>		TIMER_INFO;		// ��ITimerHandler�б���Ķ�ʱ��������Ϣ
	typedef list<STimer*>		TIMER_LIST;		// ��ʱ��̶��д�ŵĶ�ʱ���б�
	typedef vector< TIMER_LIST > TIMER_AXIS;	// ����ʱ��̶ȵ�һ��ʱ����

	TIMER_AXIS				m_TimerAxis;		// ʱ�������
	unsigned long			m_dwLastCheckTick;	// ��һ�μ���ʱ��
	unsigned long			m_dwInitTick;		// ��ʼ��ʱ����ʱ��
};
//-------------------------------------------------------------------------