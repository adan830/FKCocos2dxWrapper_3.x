//*************************************************************************
//	��������:	2014-8-4   15:32
//	�ļ�����:	IFKBaseCore.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCommonDef.h"
#include "IFKClientSocket.h"
#include "IFKTimerAxis.h"
#include "IFKEventEngine.h"
#include "IFKScriptLoader.h"
#include "IFKMessageDispatch.h"
//-------------------------------------------------------------------------
#if defined(_WIN32)
	#if defined(_USRDLL)
		#define FK_BASECORE_API __declspec(dllexport)
	#else
		#define FK_BASECORE_API __declspec(dllimport)
	#endif
#else
	#define FK_BASECORE_API
#endif
//-------------------------------------------------------------------------
// �����
#define FK_BASECORE_DLL_NAME	TEXT("FKBaseCore.dll")
//-------------------------------------------------------------------------
// �ӿڶ���
struct IBaseCore
{
	// ����
	virtual void	Release() = 0;
	// ����
	virtual bool	Create() = 0;

public:
	// ����socket
	virtual bool	CreateClientSocket( IClientSocketSink* p_pClientSocketSink,
		IClientSocket** p_ppClientSocket, bool p_bEncrypt = false ) = 0;
	// ����ʱ����
	virtual bool	CreateTimerAxis( ITimerAxis** p_ppTimerAxis ) = 0;
	// �����¼�����
	virtual bool	CreateEventEngine( IEventEngine** p_ppEventEngine ) = 0;
	// �����ű�ϵͳ
	virtual bool	CreateScriptLoader( IScriptLoader** p_ppScriptLoader ) = 0;
	// ������Ϣ�ַ���
	virtual bool	CreateMessageDispath( IMessageDispatch** p_ppMessageDispath ) = 0;
public:
	// ���º���
	virtual void	Update() = 0;
};
//-------------------------------------------------------------------------
// ����BaseCore�ӿ�
extern "C" FK_BASECORE_API bool CreateFKBaseCore( IBaseCore** p_ppBaseCore );
//-------------------------------------------------------------------------
// BaseCore����������
class CBaseCoreCreateHelper
{
#if _WIN32
	typedef bool (*ProcCreateBaseCore)(IBaseCore ** ppCore);
#endif

public:
	CBaseCoreCreateHelper()
		: m_pCore( NULL )
	{

	}
	~CBaseCoreCreateHelper()
	{

	}
public:
	bool	Create()
	{
#if _WIN32
		try
		{
			m_pCore = NULL;

			m_hDll = ::LoadLibrary( FK_BASECORE_DLL_NAME );
			if( m_hDll == NULL )
			{
				throw "Cannot load library FKBaseCore.dll";
			}

			ProcCreateBaseCore pProcFun = NULL;
			pProcFun = ( ProcCreateBaseCore )::GetProcAddress( m_hDll, "CreateFKBaseCore" );
			if( pProcFun == NULL )
			{
				throw "Cannot get func address - CreateFKBaseCore";
			}

			if( !pProcFun(&m_pCore) )
			{
				throw "Create basecore failed";
			}

			return true;
		}
		catch ( const char* p_szMsg )
		{
			OutputDebugStringA( p_szMsg );
			OutputDebugStringA("\r\n");
		}
		catch( ... )
		{
			OutputDebugStringA( "Create BaseCore Error" );
			OutputDebugStringA("\r\n");
		}

		return false;
#else
		return CreateFKBaseCore( &m_pCore );
#endif
	}

	void	Close()
	{
		if( m_pCore != NULL )
		{
			m_pCore->Release();
			m_pCore = NULL;
		}
#if _WIN32	
		if( m_hDll != NULL )
		{
			::FreeLibrary( m_hDll );
			m_hDll = NULL;
		}
#endif
	}
public:
	IBaseCore*		m_pCore;
private:
#if _WIN32
	HINSTANCE		m_hDll;		// ��̬����
#endif
};
