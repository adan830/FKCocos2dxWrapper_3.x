//*************************************************************************
//	��������:	2014-8-4   16:46
//	�ļ�����:	IFKScriptLoader.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCommonDef.h"
#include <string>
#include "../../../external/tinyxml2/tinyxml2.h"
//-------------------------------------------------------------------------
using std::string;
//-------------------------------------------------------------------------
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
//-------------------------------------------------------------------------
enum ENUM_VariantType
{
	eVT_Empty	= 0,
	eVT_Int,			// ��������
	eVT_Float,			// ��������
	eVT_String,			// �ַ�����
	eVT_Iterator,		// ���������ͣ�֧�ֶ�ά�ű���
	
	eVT_Max,
};
//-------------------------------------------------------------------------
struct IFKScriptIterator;

// ���ϱ�����Ԫ
typedef struct _VARIANT
{
	ENUM_VariantType		m_eType;		// ��������

	union _VALUE_
	{
		long				m_lValue;		// ��������������
		double				m_dValue;		// �����Ǹ�������
		const char*			m_szValue;		// �������ַ�������
		IFKScriptIterator*	m_iteValue;		// �����ǵ������ӽű���֧�ֶ�ά�ű���
	} m_Data;

}SVariant;
//-------------------------------------------------------------------------
struct IFKScriptIterator
{
	// �ͷ���Դ
	virtual void	Release() = 0;

public:
	// ��ȡ���ݸ���
	virtual int		Size() const = 0;

public:
	virtual SVariant&	GetData( int p_nIndex ) = 0;
	virtual SVariant&	operator[] ( int p_nIndex ) = 0;
	virtual SVariant&	GetData( const char* p_szName ) = 0;
	virtual SVariant&	operator[] ( const char* p_szName ) = 0;
};
//-------------------------------------------------------------------------
// �������ͷ�װ������ʹ��
class CVariant : protected SVariant
{
public:
	CVariant()
		: m_Variant(*((SVariant *)this))
	{
		m_Variant.m_eType = eVT_Empty;
		m_Variant.m_Data.m_lValue = 0;
	}

	CVariant( char* p_szStr ) 
		: m_Variant(*((SVariant *)this))
	{
		m_Variant.m_eType = eVT_String;
		m_Variant.m_Data.m_szValue = p_szStr;
	}

	CVariant( long p_lValue ) 
		: m_Variant(*((SVariant *)this))
	{
		m_Variant.m_eType = eVT_Int;
		m_Variant.m_Data.m_lValue = p_lValue;
	}

	CVariant( double p_dValue ) 
		: m_Variant(*((SVariant *)this))
	{
		m_Variant.m_eType = eVT_Float;
		m_Variant.m_Data.m_dValue = p_dValue;
	}

	CVariant( IFKScriptIterator* p_pIte ) 
		: m_Variant(*((SVariant *)this))
	{
		m_Variant.m_eType = eVT_Iterator;
		m_Variant.m_Data.m_iteValue = p_pIte;
	}

	CVariant(SVariant &value)
		: m_Variant(value)
	{
	}

	CVariant operator[]( int p_nIndex )
	{
		ASSERT( m_Variant.m_eType == eVT_Iterator );
		return (*m_Variant.m_Data.m_iteValue)[p_nIndex];
	}

	CVariant operator[](LPCSTR szName)
	{
		ASSERT( m_Variant.m_eType == eVT_Iterator );
		return (*m_Variant.m_Data.m_iteValue)[szName];
	}

	CVariant operator[](int8* szName)
	{
		ASSERT( m_Variant.m_eType == eVT_Iterator );
		return (*m_Variant.m_Data.m_iteValue)[szName];
	}

	int Size()
	{
		ASSERT( m_Variant.m_eType == eVT_Iterator );
		return m_Variant.m_Data.m_iteValue->Size();
	}

	operator const char*()
	{
		if ( m_Variant.m_eType != eVT_String)
		{
			_ChangeType( eVT_String );
		}

		return m_Variant.m_Data.m_szValue;
	}

	operator int()
	{
		if ( m_Variant.m_eType != eVT_Int )
		{
			_ChangeType( eVT_Int );
		}

		return m_Variant.m_Data.m_lValue;
	}

	operator long()
	{
		if ( m_Variant.m_eType != eVT_Int )
		{
			_ChangeType( eVT_Int );
		}

		return m_Variant.m_Data.m_lValue;
	}

	operator double()
	{
		if ( m_Variant.m_eType != eVT_Float )
		{
			_ChangeType( eVT_Float );
		}

		return m_Variant.m_Data.m_dValue;
	}

	ENUM_VariantType GetDataType()
	{
		return m_Variant.m_eType;
	}
protected:
	typedef void (CVariant::*ChangeTypeFunProc)();

	// ����ת��
	void _ChangeType( ENUM_VariantType p_eType )
	{
		static ChangeTypeFunProc sTypeChangePoc[eVT_Max][eVT_Max] =
		{
			{NULL, NULL, NULL, NULL, NULL},
			{NULL, NULL, &CVariant::_ChangeType_long2float, NULL, NULL},
			{NULL, &CVariant::_ChangeType_float2long, NULL, NULL, NULL},
			{NULL, &CVariant::_ChangeType_str2long, &CVariant::_ChangeType_str2float, NULL, NULL},
			{NULL, NULL, NULL, NULL, NULL}
		};

		if ( sTypeChangePoc[m_Variant.m_eType][p_eType] != NULL)
		{
			(this->*sTypeChangePoc[m_Variant.m_eType][p_eType])();
		}
	}

	// long -> float
	void _ChangeType_long2float()
	{
		m_Variant.m_Data.m_dValue = m_Variant.m_Data.m_lValue;
		m_Variant.m_eType = eVT_Float;
	}

	// float -> long
	void _ChangeType_float2long()
	{
		m_Variant.m_Data.m_lValue = (long)m_Variant.m_Data.m_dValue;
		m_Variant.m_eType = eVT_Int;
	}

	// str -> long
	void _ChangeType_str2long()
	{
		long lTemp = 0;
		lTemp = atoi(m_Variant.m_Data.m_szValue);
		m_Variant.m_Data.m_lValue = lTemp;
		m_Variant.m_eType = eVT_Int;
	}

	// str -> float
	void _ChangeType_str2float()
	{
		float dTemp = 0.0f;
		dTemp =(float) atof(m_Variant.m_Data.m_szValue);
		m_Variant.m_Data.m_dValue = dTemp;
		m_Variant.m_eType = eVT_Float;
	}
public:
	SVariant&		m_Variant;
};
//-------------------------------------------------------------------------
// Ini�����ļ����ػص�
struct IIniFileSink
{
	virtual void		OnLoadData( const char* p_szFileName, const char* p_szSection,
		const char* p_szKeyName, const char* p_szValue ) = 0;
};
//-------------------------------------------------------------------------
struct IIniFile
{
	virtual bool		isOpen() = 0;
	virtual bool		create(LPCSTR FileName) = 0;
	virtual bool		open(LPCSTR FileName) = 0;
	virtual bool		open(LPCSTR FileName,IIniFileSink *pSink) = 0;

	virtual bool		open(LPCSTR lpszData,int nSize) = 0;
	virtual bool		save() = 0;
	virtual bool		save(std::string &lpszData) = 0;
	virtual void		close() = 0;
	virtual void		clear() = 0;
	virtual void		clearSection(int8* lpSection) = 0;
	virtual void		getString(
		LPCSTR	lpSection,		// points to section name
		LPCSTR	lpKeyName,		// points to key name
		LPCSTR	lpDefault,		// points to default string
		int8*	lpRString,		// points to destination buffer
		unsigned long	dwSize			// size of string buffer
		) = 0;
	virtual void		getString(
		LPCSTR	lpSection,		// points to section name
		LPCSTR	lpKeyName,		// points to key name
		LPCSTR	lpDefault,		// points to default string
		std::string &RString	// points to destination buffer
		) = 0;

	virtual void		getInteger(
		LPCSTR	lpSection,		// points to section name
		LPCSTR	lpKeyName,		// points to key name
		int		nDefault,		// default value
		int		*Value          // return value
		) = 0;
	virtual void		getFloat(
		LPCSTR	lpSection,		// points to section name
		LPCSTR	lpKeyName,		// points to key name
		float32 fDefault,		// default value
		float32 *Value          // return value
		) = 0;
	virtual void		getStruct(
		LPCSTR	lpSection,		// pointer to section name
		LPCSTR	lpKeyName,		// pointer to key name
		void*	lpStruct,		// pointer to buffer that contains data to add
		unsigned long dwSize	// size, in bytes, of the buffer
		) = 0;

	virtual void		writeString(
		LPCSTR	lpSection,		// pointer to section name
		LPCSTR	lpKeyName,		// pointer to key name
		LPCSTR	lpString		// pointer to string to add
		) = 0;
	virtual void		writeInteger(
		LPCSTR	lpSection,		// pointer to section name
		LPCSTR	lpKeyName,		// pointer to key name
		int	    Value			// value to write
		) = 0;
	virtual void		writeFloat(
		LPCSTR	lpSection,		// pointer to section name
		LPCSTR	lpKeyName,		// pointer to key name
		float32   Value			// value to write
		) = 0;
	virtual void		writeStruct(
		LPCSTR	lpSection,		// pointer to section name
		LPCSTR	lpKeyName,		// pointer to key name
		void*	lpStruct,		// pointer to buffer that contains data to add
		unsigned long 	dwSize			// size, in bytes, of the buffer
		) = 0;
};
//-------------------------------------------------------------------------
// �ļ����ػص�
struct IScriptLoadSink
{
	// �ļ����� 
	virtual bool	OnFileLoad( const char* p_szFile, IFKScriptIterator* p_pFilePointer, 
		tinyxml2::XMLDocument* p_pTiXmlDocument) = 0;
	// �ļ�����
	virtual bool	OnFileUpdate( const char* p_szFile, IFKScriptIterator* p_pFilePointer, 
		tinyxml2::XMLDocument* p_pTiXmlDocument) = 0;
};
//-------------------------------------------------------------------------
// �ļ�������
struct IScriptLoader
{
	// ����
	virtual bool	Create() = 0;
	// ����
	virtual void	Release() = 0;

public:
	// ����: ����һ���ļ�(���лص�����)
	// (*ע��) ֻ�е�����Ҫ ���ĳ������ ���ű�ˢ�¡� ���ܵ�ʱ�� ��ʹ�øú��� 
	// ����һ��XML CSV��ȡ�����ĸ�ʽ�ļ� ͨ���ص���ȡ�ļ�����ָ��
	// ע�⣺�ص�ʱ��ȡ���ļ�����ָ�� ����Ҫ�ͷ� ����������ڲ�������
	// ���� : LPCSTR szFileName			-----�ļ���
	//		  IFileLoadSink *pSikn		-----�ļ����غ�Ļص�ָ�룬��ı���Ҫʵ�ָ÷���
	virtual bool LoadFile( const char* p_szFileName, IScriptLoadSink* p_pSink ) = 0;

	// ����Ҫˢ�ű����ܵ�ʱ�� ʹ���������
	virtual bool UpdateFile( const char* p_szFileName) = 0;

	// ��ȡCSV�ű��ļ��ĵ�����
	virtual IFKScriptIterator*		LoadCSVFile( const char* p_szFileName,bool p_bEncrypt = false ) = 0;
	virtual IFKScriptIterator*		LoadCSVFile( const char* p_szFileName, const char* p_szBuffData, int p_nLen ) = 0;

	// ����XML �ļ��Լ��ͷŰ�
	virtual tinyxml2::XMLDocument*	LoadXmlFile( const char* p_szFileName,bool p_bEncrypt = false ) = 0;
	virtual tinyxml2::XMLDocument*	LoadXmlFile( const char* p_szFileName, const char* p_szBuffData, int p_nLen ) = 0;

	// ����INI �ļ� ��ȡINI���� 
	virtual bool					LoadIniFile( const char* p_szFileName,IIniFileSink* p_pSink ) = 0;
	virtual IIniFile*				LoadIniFile( const char* p_szFileName ) = 0;

	// ����INI
	virtual bool					UpdateIniFile( const char* p_szFileName ) = 0;
};
//-------------------------------------------------------------------------