//*************************************************************************
//	��������:	2014-8-6   15:08
//	�ļ�����:	TSwitchList.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include <list>
#include <mutex>
using std::list;
using std::mutex;
//-------------------------------------------------------------------------
template< class SCommand >
class TSwitchList
{
public:
	typedef list<SCommand>		COMMAND_LIST;		// �����б�
private:
	COMMAND_LIST*				m_pCurrentInList;	// ��ǰ�����б�
	COMMAND_LIST*				m_pCurrentOutList;	// ��ǰ����б�
	mutex						m_hLock;			// ��
	COMMAND_LIST				m_tagList1;
	COMMAND_LIST				m_tagList2;
public:
	TSwitchList()
		: m_pCurrentInList( &m_tagList1 )
		, m_pCurrentOutList( &m_tagList2 )
	{
		
	}
	
	virtual ~TSwitchList(){

	}

	void PushFront( SCommand* p_pCmd )
	{
		if( p_pCmd == NULL ) 
			return;

		m_hLock.lock();
		m_pCurrentInList->push_front( *p_pCmd );
		m_hLock.unlock();
	}

	void Push( SCommand* p_pCmd )
	{
		if( p_pCmd == NULL )
			return;

		m_hLock.lock();
		m_pCurrentInList->push_back( *p_pCmd );
		m_hLock.unlock();
	}

	// ��ת
	bool SwitchState()
	{
		m_hLock.lock();

		if( m_pCurrentOutList->size() > 0 )
		{
			m_hLock.unlock();
			return false;
		}

		COMMAND_LIST* pList = m_pCurrentInList;
		m_pCurrentInList = m_pCurrentOutList;
		m_pCurrentOutList = pList;

		m_hLock.unlock();
		return true;
	}

	COMMAND_LIST& GetOutList()
	{
		return *m_pCurrentOutList;
	}
};