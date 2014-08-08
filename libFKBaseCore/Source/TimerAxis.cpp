//-------------------------------------------------------------------------
#include "../Include/TimerAxis.h"
//-------------------------------------------------------------------------
CTimerAxis::CTimerAxis()
{

}
//-------------------------------------------------------------------------
CTimerAxis::~CTimerAxis()
{

}
//-------------------------------------------------------------------------
// ����
bool CTimerAxis::Create()
{
	try
	{
		m_TimerAxis.resize( (TIME_AXIS_LENGTH + TIME_GRID - 1) / TIME_GRID );
		m_dwInitTick = getTickCount();
		m_dwLastCheckTick = m_dwInitTick;
	}
	catch( ... )
	{
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------
// ����
void CTimerAxis::Release()
{
	try
	{
		for( unsigned long i = 0; i < m_TimerAxis.size(); ++i )
		{
			TIMER_LIST& TimerList = m_TimerAxis.at(i);
			TIMER_LIST::iterator Ite = TimerList.begin();
			for( ; Ite != TimerList.end(); ++Ite )
			{
				STimer* pTimer = *Ite;
				if( pTimer )
				{
					KillTimer( pTimer->m_dwTimerID, pTimer->m_pHandler );
				}
			}
		}

		m_TimerAxis.clear();
		delete this;
	}
	catch( ... )
	{
	}
}
//-------------------------------------------------------------------------
/**
@purpose				: ����һ����ʱ��
@param	 p_ulTimerID	: ��ʱ��ID
@param   p_ulInterval	: ��ʱ�����ü��
@param   p_pHandler		: ����ӿ�
@param   p_ulCallTimes	: ���ô���,Ĭ�ϵ��������
@param   p_szDebugInfo	: ������Ϣ
@return					: ������óɹ��򷵻�true
*/
bool CTimerAxis::SetTimer( unsigned long p_ulTimerID, unsigned long p_ulInterval,
	ITimerHandler* p_pHandler, unsigned long p_ulCallTimes, const char* p_szDebugInfo )
{
	if( p_pHandler == NULL )
		return false;
	if( p_ulInterval == 0 )
	{
		p_ulInterval = 1;
	}
	void** ppTimerInfo = p_pHandler->GetTimerInfoPtr();
	if( ppTimerInfo == NULL )
		return false;

	TIMER_INFO* pTimerInfo = *(TIMER_INFO**)ppTimerInfo;
	if( pTimerInfo == NULL )
	{
		pTimerInfo = new TIMER_INFO;
		*ppTimerInfo = pTimerInfo;
	}

	// ����Ƿ��Ѿ�����˵�ǰTimer
	TIMER_INFO::iterator Ite = pTimerInfo->begin();
	for( ; Ite != pTimerInfo->end(); ++Ite )
	{
		STimer& Timer = *Ite;
		if( Timer.m_dwTimerID == p_ulTimerID )
		{
			// �Ѿ�����
			return false;
		}
	}

	STimer Timer;
	Timer.m_dwTimerID = p_ulTimerID;
	Timer.m_dwInterval = p_ulInterval;
	Timer.m_dwCallTimes = p_ulCallTimes;
	Timer.m_dwLastCallTick = m_dwLastCheckTick;
	Timer.m_pHandler = p_pHandler;
	Timer.m_szDebugInfo = p_szDebugInfo;
	Timer.m_dwGridIndex = ((Timer.m_dwLastCallTick + Timer.m_dwInterval - m_dwInitTick ) / TIME_GRID) % m_TimerAxis.size();

	pTimerInfo->push_back( Timer );

	STimer& TimerRef = pTimerInfo->back();
	STimer* TimerPtr = &TimerRef;
	m_TimerAxis[Timer.m_dwGridIndex].push_back( TimerPtr );
	TimerPtr->m_ItePos = --m_TimerAxis[Timer.m_dwGridIndex].end();

	return true;
}
//-------------------------------------------------------------------------

/**
@purpose				: ɾ����ʱ��
@param	 p_ulTimerID	: ��ʱ��ID
@param   p_pHandler		: ����ӿ�
@return					: �����Ƿ�ɾ���ɹ�
*/
bool CTimerAxis::KillTimer( unsigned long p_ulTimerID, ITimerHandler* p_pHandler )
{
	if( p_pHandler == NULL )
		return false;

	void** ppTimerInfo = p_pHandler->GetTimerInfoPtr();
	if( ppTimerInfo == NULL )
		return false;

	TIMER_INFO* pTimerInfo = *(TIMER_INFO**)ppTimerInfo;
	if( pTimerInfo == NULL )
		return false;

	// ����������STimer
	TIMER_INFO::iterator Ite = pTimerInfo->begin();
	for( ; Ite != pTimerInfo->end(); ++Ite )
	{
		STimer& Timer = *Ite;
		if( Timer.m_dwTimerID == p_ulTimerID )
		{
			// �ҵ���
			// ע�⣺���ﲢδ��ʱ������ʵ��ɾ���ö��󣬽�����Ϊ 0 
			*Timer.m_ItePos = 0;

			pTimerInfo->erase( Ite );

			if( pTimerInfo->empty() )
			{
				delete pTimerInfo;
				*ppTimerInfo = 0;
			}

			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------
// ÿ֡����
void CTimerAxis::CheckTimer()
{
	unsigned long ulNow = getTickCount();
	// ����16ms�����и��¼��
	if( ulNow - m_dwLastCheckTick < CHECK_FREQUENCY )
		return;

	unsigned long ulStartGrid = ((m_dwLastCheckTick - m_dwInitTick) / TIME_GRID ) % m_TimerAxis.size();
	unsigned long ulCurGrid = ((ulNow - m_dwInitTick) / TIME_GRID ) % m_TimerAxis.size();

	// ��¼���һ�μ��ʱ��
	m_dwLastCheckTick = ulNow;

	// �������ʱ��֮���ʱ��̶�
	unsigned long i = ulStartGrid;
	do 
	{
		TIMER_LIST& TimerList = m_TimerAxis[i];
		TIMER_LIST::iterator Ite = TimerList.begin();
		for( ; Ite != TimerList.end(); )
		{
			// ����ʱ��̶��ڵ�ʱ�����
			STimer* pTimer = *Ite;
			if( pTimer == NULL )
			{
				// �յ�Timer���󣬿�����KillTimer��ɵĿ�Timer��ֱ��ɾ��
				Ite = TimerList.erase( Ite );
				continue;
			}

			// ������ʱ��
			if( ulNow - pTimer->m_dwLastCallTick >= pTimer->m_dwInterval )
			{
				pTimer->m_dwLastCallTick = ulNow;
				pTimer->m_dwCallTimes -= 1;

				// ִ�лص�
				pTimer->m_pHandler->OnTimer( pTimer->m_dwTimerID );

				if( pTimer->m_dwCallTimes <= 0 )
				{
					// ���ô�������������ɾ����
					KillTimer( pTimer->m_dwTimerID, pTimer->m_pHandler );
				}
				else
				{
					// ��Ǩ����һ�δ�����ʱ���
					pTimer->m_dwGridIndex = (( pTimer->m_dwLastCallTick + pTimer->m_dwInterval - m_dwInitTick ) / TIME_GRID ) % m_TimerAxis.size();
					Ite = TimerList.erase( Ite );
					m_TimerAxis[ pTimer->m_dwGridIndex ].push_back( pTimer );
					pTimer->m_ItePos = --m_TimerAxis[ pTimer->m_dwGridIndex ].end();

					continue;
				}
			}

			// ûɾ����ֱ�ӵ���[ ��ɾ�����ֿ��ܣ�һ����Э��֮ǰɾ����һ����ִ����֮��ɾ�� ]
			++Ite;
		}

		// �����յ�
		if( i == ulCurGrid )
		{
			break;
		}
		else
		{
			i = (i+1) % m_TimerAxis.size();
		}
	} while ( i != ulCurGrid );
}
//-------------------------------------------------------------------------