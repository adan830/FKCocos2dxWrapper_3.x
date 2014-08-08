//*************************************************************************
//	��������:	2014-8-7   15:47
//	�ļ�����:	TEventEngine.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../../FKCommon/FKCommonDef.h"
#include <list>
#include <string>
using std::list;
using std::string;
//-------------------------------------------------------------------------
// ����Ͳ���[ ��ֹ�ݹ���ѭ�� ]
#define FIRE_MAX_LAYER_NUM			20
// ����������
#define REF_MAX_LAYER_NUM			5
//-------------------------------------------------------------------------
// �¼�Key
struct _SEventKey
{
	uint32			m_dwSrcID;		// Դ��ʾ
	unsigned char	m_ucSrcType;	// Դ����
	unsigned char	m_ucEventID;	// �¼�ID
	unsigned char	m_usReserve1;	// ����λ
	unsigned char	m_usReserve2;	// ����λ
};
struct SEventKey
{
	union
	{
		_SEventKey	m_Key;
		longlong	m_Value;
	};

	SEventKey()
		: m_Value ( 0 )
	{
	}

	bool operator == ( const SEventKey& p_Other )const
	{
		return m_Value == p_Other.m_Value;
	}
	
	bool operator < ( const SEventKey& p_Other )const
	{
		return m_Value < p_Other.m_Value;
	}
};
//-------------------------------------------------------------------------
#if _WIN32
	namespace stdext
	{
		inline size_t hash_value(const SEventKey &eventkey)
		{
			uint32 k1 = (eventkey.m_Key.m_dwSrcID & 0xFFFF) << 16;
			uint32 k2 = (eventkey.m_Key.m_ucEventID & 0xFF) << 8;
			uint32 k3 = eventkey.m_Key.m_ucSrcType;
			return k1 + k2 + k3;
		}
	}
#else
	namespace stdext
	{
		template <>
		struct hash<SEventKey>
		{
			inline size_t operator() (const SEventKey &eventkey) const
			{
				uint32 k1 = (eventkey.m_Key.m_dwSrcID & 0xFFFF) << 16;
				uint32 k2 = (eventkey.m_Key.m_ucEventID & 0xFF) << 8;
				uint32 k3 = eventkey.m_Key.m_ucSrcType;
				return k1 + k2 + k3;
			}
		};
	}
#endif
//-------------------------------------------------------------------------
// �¼�����ģ��
template< class TSink, class TOnEventObject >
class TEventEngine
{
private:
	struct SSubscibeInfo
	{
		TSink*		m_pSink;
		char		m_szDesc[32];
		int			m_nCallCount;
		bool		m_bRemoveFlag;

		SSubscibeInfo( TSink* p_pSink, const char* p_szDesc )
		{
			m_pSink = p_pSink;
			m_nCallCount = 0;
			m_bRemoveFlag = false;
			if( p_szDesc != NULL )
			{
				memcpy( m_szDesc, p_szDesc, sizeof(m_szDesc) );
			}
			else
			{
				m_szDesc[0] = '\0';
			}
		}

		void Add()
		{
			m_nCallCount++;
		}

		void Sub()
		{
			m_nCallCount--;
			if( m_nCallCount <= 0 )
			{
				m_nCallCount = 0;
			}
		}
	};

	// һ���¼�KEY�Ķ������б�
	typedef list< SSubscibeInfo >								LIST_SUBSCIBEINFOS;
	// �����¼�KEY�Ķ������б�
	typedef stdext::hash_map< SEventKey, LIST_SUBSCIBEINFOS >	MAP_SUBSCIBEINFOS;
public:
	TEventEngine()
		: m_nFireLayerNum( 0 )
	{
	}

	virtual ~TEventEngine()
	{
		typename MAP_SUBSCIBEINFOS::iterator Ite = m_mapSink.begin();
		for( ; Ite != m_mapSink.end(); ++Ite )
		{
			LIST_SUBSCIBEINFOS* pList = &((*Ite).second);
			pList->clear();
		}
		m_mapSink.clear();
	}

	bool Subscibe( TSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID, const char* p_szDesc )
	{
		if( p_pSink == NULL )
			return false;

		SEventKey key;
		key.m_Key.m_ucEventID = p_ucEventID;
		key.m_Key.m_ucSrcType = p_ucSrcType;
		key.m_Key.m_dwSrcID = p_unSrcID;

		// ��������Ϣ
		SSubscibeInfo tagSubscibeInfo( p_pSink, p_szDesc );

		// ��ӵ������б�
		typename MAP_SUBSCIBEINFOS::iterator Ite = m_mapSink.find( key );
		if( Ite == m_mapSink.end() )
		{
			LIST_SUBSCIBEINFOS list;
			list.push_front( tagSubscibeInfo );

			m_mapSink.insert( std::make_pair( key, list ) );
		}
		else
		{
			LIST_SUBSCIBEINFOS* pList = &((*Ite).second);
			pList->push_front( tagSubscibeInfo );
		}

		return true;
	}

	bool UnSubscibe( TSink* p_pSink, uint8 p_ucEventID, uint8 p_ucSrcType,
		uint32 p_unSrcID )
	{
		if( p_pSink == NULL )
			return false;

		SEventKey key;
		key.m_Key.m_ucEventID = p_ucEventID;
		key.m_Key.m_ucSrcType = p_ucSrcType;
		key.m_Key.m_dwSrcID = p_unSrcID;

		typename MAP_SUBSCIBEINFOS::iterator Ite = m_mapSink.find( key );
		if( Ite == m_mapSink.end() )
		{
			LIST_SUBSCIBEINFOS* pList = &((*Ite).second);

			typename LIST_SUBSCIBEINFOS::iterator IteList = pList->begin();
			for( ; IteList != pList->end(); ++IteList )
			{
				SSubscibeInfo* pSubscibeInfo = &(*IteList);
				if( pSubscibeInfo->m_pSink == p_pSink )
				{
					if( pSubscibeInfo->m_nCallCount == 0 )
					{
						pList->erase( IteList );
					}
					else
					{
						pSubscibeInfo->m_bRemoveFlag = true;
					}
					break;
				}
			}
		}

		return true;
	}
	
	bool Fire( uint8 p_ucEventID, uint8 p_ucSrcType, uint32 p_unSrcID,
		const char* p_szContext, int p_nLen )
	{
		SEventKey key;
		key.m_Key.m_ucEventID = p_ucEventID;
		key.m_Key.m_ucSrcType = p_ucSrcType;

		// ���ʹ���Դָ���
		key.m_Key.m_dwSrcID = p_unSrcID;
		if( key.m_Key.m_dwSrcID != 0 )
		{
			bool bRet = __Fire( key, p_ucEventID, p_ucSrcType, p_unSrcID, p_szContext, p_nLen );
			if( !bRet )
			{
				return false;
			}
		}

		// ���Ͳ���Դָ���
		key.m_Key.m_dwSrcID = 0;
		bool bRet = __Fire( key, p_ucEventID, p_ucSrcType, p_unSrcID, p_szContext, p_nLen );
		if( !bRet )
		{
			return false;
		}

		return true;
	}

private:
	bool __Fire( SEventKey& p_Key, uint8 p_ucEventID, uint8 p_ucSrcType, 
		uint32 p_unSrcID, const char* p_szContext, int p_nLen )
	{
		m_nFireLayerNum++;
		if( m_nFireLayerNum >= FIRE_MAX_LAYER_NUM )
		{
			// ��ѭ�����ߵݹ���
			assert( false );
			m_nFireLayerNum--;
			return false;
		}

		typename MAP_SUBSCIBEINFOS::iterator Ite = m_mapSink.find( p_Key );
		if( Ite != m_mapSink.end() )
		{
			LIST_SUBSCIBEINFOS* pList = &((*Ite).second);
			typename LIST_SUBSCIBEINFOS::iterator IteList = pList->begin();
			for( ; IteList != pList->end(); )
			{
				SSubscibeInfo* pInfo = &(*IteList);

				if( pInfo->m_nCallCount >= REF_MAX_LAYER_NUM )
				{
					// ��ѭ�����ߵݹ���
					assert( false );
					m_nFireLayerNum--;
					return false;
				}

				if( !pInfo->m_bRemoveFlag )
				{
					bool bResult =  false;
					try
					{
						pInfo->Add();
						bResult = m_FireEventObject( pInfo->m_pSink, p_ucEventID, p_ucSrcType, p_unSrcID, p_szContext, p_nLen );
						pInfo->Sub();
					}
					catch( ... )
					{
						pInfo->Sub();
						assert( false );
						return false;
					}

					if( pInfo->m_bRemoveFlag && pInfo->m_nCallCount == 0 )
					{
						IteList = pList->erase( IteList );
					}
					else
					{
						++IteList;
					}

					if( !bResult )
					{
						m_nFireLayerNum--;
						return false;
					}
				}
				else
				{
					// �Ƴ���ǩ�Ѿ���ʾ
					if( pInfo->m_nCallCount == 0 )
					{
						IteList = pList->erase( IteList );
					}
					else
					{
						++IteList;
					}
				}
			}
		}

		m_nFireLayerNum--;
		return true;
	}
private:
	TOnEventObject		m_FireEventObject;	// �¼����Ͷ���
	MAP_SUBSCIBEINFOS	m_mapSink;			// �¼������б�
	int					m_nFireLayerNum;	// �¼����Ͳ�������ֹ�ݹ���ѭ����
};
//-------------------------------------------------------------------------