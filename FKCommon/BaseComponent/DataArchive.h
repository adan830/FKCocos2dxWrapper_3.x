//*************************************************************************
//	��������:	2014-8-7   13:37
//	�ļ�����:	DataArchive.h
//  �� �� ��:   ������ FreeKnight	
//	��Ȩ����:	MIT
//	˵    ��:	
//			ʹ�ã�CArchive��Ҫ�ⲿ�ṩ������������һ���ֲ�buffer��������:
//			char buf[1024];
//			CDataArchive archive(buf,1024);
//			archive.Write("hello",5);
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../FKCommonDef.h"
//-------------------------------------------------------------------------
class CDataArchive
{
protected:
	CDataArchive()
		: m_nOffset( 0 )
		, m_pBindBuffer( NULL )
		, m_nMaxSize( 0 )
	{

	}

	void _Init( void* p_szBindBuffer, int p_nMaxSize )
	{
		if( p_szBindBuffer == NULL || p_nMaxSize <= 0 )
			return;
		m_nOffset = 0;
		m_pBindBuffer = p_szBindBuffer;
		m_nMaxSize = p_nMaxSize;
	}

public:
	CDataArchive( void* p_szBindBuf, int p_nMaxSize )
	{
		_Init( p_szBindBuf, p_nMaxSize );
	}
	~CDataArchive(){

	}

public:
	// д������
	virtual bool Write( const void* p_pBuf, int p_nLen )
	{
		if( p_pBuf == NULL || p_nLen <= 0 || m_nOffset + p_nLen > m_nMaxSize )
			return false;

		memcpy( (char*)m_pBindBuffer + m_nOffset, p_pBuf, p_nLen );
		m_nOffset += p_nLen;
		return true;
	}

	// ��������
	virtual bool Read( FK_OUT_PARAM void* p_pBuf, int p_nLen )
	{
		if( p_pBuf == NULL || p_nLen <= 0 || m_nOffset + p_nLen > m_nMaxSize )
			return false;

		memcpy( p_pBuf, (char*)m_pBindBuffer + m_nOffset, p_nLen );
		m_nOffset += p_nLen;
		return true;
	}

	// ��ȡ��ǰ����ָ��
	virtual void* GetCurBufferHead()
	{
		return (char*)m_pBindBuffer + m_nOffset;
	}

	// ��ȡ��ǰƫ��ֵ
	virtual int GetBufferOffset()
	{
		return m_nOffset;
	}

	// ��ȡʣ��ֵ��δƫ��ֵ��
	// ������ʱ����ȡʣ�����ݳ��ȡ�
	// д����ʱ����ȡ��д��ʣ�໺�峤�ȡ�
	virtual int GetLeftBufferLen()
	{
		return m_nMaxSize - m_nOffset;
	}

	// ��ָ������ƶ�ָ���ֽڣ������ƶ�ǰ��ָ��
	// ����Ҫ����ƶ����ָ�룬seek֮�� GetCurBufferHead()����
	// ���ƶ�ʧ�ܣ��򷵻� -1
	virtual int Seek( int p_nOffset )
	{
		if( m_nOffset + p_nOffset > m_nMaxSize || m_nOffset + p_nOffset < 0 )
			return -1;
		int nOld = m_nOffset;
		m_nOffset += p_nOffset;
		return nOld;
	}

	// ��ָ���ƶ���ָ��λ�ã������ƶ�ǰ��ָ��
	virtual int SeekTo( int p_nPtr = 0 )
	{
		if( p_nPtr > m_nMaxSize || p_nPtr < 0 )
			return -1;

		int nOld = m_nOffset;
		m_nOffset = p_nPtr;
		return nOld;
	}
private:
	int			m_nOffset;
	void*		m_pBindBuffer;
	int			m_nMaxSize;
};
//-------------------------------------------------------------------------