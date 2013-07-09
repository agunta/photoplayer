//////////////////////////////////////////////////////////////////////
// This software module was originally developed by
// 
// Mitsubishi Electric ITE B.V. Visual Information Laboratory
// ('VIL')
// 
// in the course of development of the
// 
// MPEG-A Photo-Player Multimedia Application Format 
// ('Photo-Player MAF').
// 
// This software module is an implementation of a part of one or more
// Photo-Player MAF tools as specified by ISO/IEC 23000-3. ISO/IEC gives
// users of Photo-Player MAF free license to this software module or
// modifications thereof for use in hardware or software products claiming
// conformance to Photo-Player MAF. Those intending to use this software
// module in hardware or software products are advised that its use may
// infringe existing patents. The original developer of this software
// module and his/her company, the subsequent editors and their companies,
// and ISO/IEC have no liability for use of this software module or
// modifications thereof in an implementation.
// 
// Copyright is not released for non Photo-Player MAF conforming products.
// VIL retains full right to use the code for its own purpose, assign or
// donate the code to a third party, and to inhibit third parties from
// using the code for non Photo-Player MAF conforming products.
// 
// To the fullest extent allowed by applicable law, this software program
// is issued "as is". Any warranties, conditions or representations,
// expressed or implied, statutory or otherwise, are hereby excluded. Such
// exclusions shall include without limitation all implied warranties of
// satisfactory quality or fitness for any particular purpose and non-
// infringement of third party rights. In no event shall any user of this
// software program have any claim of any nature whatsoever against VIL or
// any third party supplier to VIL regarding this software. No warranties
// express or implied are made by VIL or any such third party supplier.
// All limitations and exclusions of liability apply equally for the
// benefit of VIL and all such suppliers. VIL does not warrant that the
// software is error-free or that it will operate without interruption.
// 
// This copyright notice must be included in all copies or derivative
// works.
// 
// Copyright © 2007

#include "collectionbim.h"
#include "utfutil.h"

#define MAX_DESCSTRING_LENGTH 1000000

DescString::DescString()
{
	m_pData = NULL;
	m_nLength = 0;
	m_pUTF8Data = NULL;
	m_nUTF8Length = 0;
}

DescString::~DescString()
{
	if(m_pData)
	{
		delete m_pData;
	}
	if(m_pUTF8Data)
	{
		delete m_pUTF8Data;
	}
}

bool DescString::BuildFromXML(DOMNode *node)
{
	if(node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	wchar_t *data = BimXMLUtil::GetNodeText(node);
	SetData(data);
	return true;
}

int DescString::GetNumberOfBits()
{
	int bitCount = BimXMLUtil::GetVLI5BitSize(m_nUTF8Length);
	bitCount += m_nUTF8Length * 8;
	return bitCount;
}

bool DescString::Validate()
{
	bool bret = true;
	//if(m_nLength == 0 || m_pData == NULL) bret = false;
	return bret;
}

bool DescString::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bret = writer->WriteVarLenInt5(m_nUTF8Length);
	if(!bret) return bret;
	char *pdata = m_pUTF8Data;
	if(!pdata && m_nUTF8Length > 0) return false;
	for(int i = 0; i < m_nUTF8Length; i++)
	{
		int k = (int)(*(pdata + i));
		bret = writer->WriteBits(k, 8);
		if(!bret)  return bret;
	}
	return bret;
}

bool DescString::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pData);
	SAFE_DELETE(m_pUTF8Data);
	m_nLength = 0;
	m_nUTF8Length = 0;

	bool bret;
	do {
		unsigned int len;
		bret = reader->ReadVarLenInt5(&len);
		if(!bret) break;
		m_nUTF8Length = (int) len;
		if(m_nUTF8Length >= 0)
		{
			m_pUTF8Data = new char[m_nUTF8Length + 1];
			if(!m_pUTF8Data) { bret = false; break; }
			char *pdata = m_pUTF8Data;
			for(int i = 0; i < m_nUTF8Length; i++)
			{
				int k;
				bret = reader->ReadBits(&k, 8);
				*(pdata + i) = (char)k;
				if(!bret)  break;
			}
			m_pUTF8Data[m_nUTF8Length] = 0;
			bret = UTF8ToWideChar(m_pUTF8Data, &m_pData, &m_nLength);
		}
	}
	while(false);

	if(!bret)
	{
		SAFE_DELETE(m_pData);
		SAFE_DELETE(m_pUTF8Data);
		m_nLength = 0;
		m_nUTF8Length = 0;
	}
	return bret;
}

void DescString::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	if(m_pUTF8Data) *fout << m_pUTF8Data;
}

wchar_t *DescString::GetData()
{
	return m_pData;
}

char *DescString::GetUTF8Data()
{
	return m_pUTF8Data;
}

void DescString::SetData(wchar_t *data)
{
	if(data)
	{
		m_nLength = (int)wcslen(data);
		if(m_nLength > 0)
		{
			m_pData = _wcsdup(data);
			if(m_pData)
			{
				WideCharToUTF8(m_pData, &m_pUTF8Data, &m_nUTF8Length);
			}
		}
	}
	else
	{
		if(m_pData) delete m_pData;
		m_pData = NULL;
		if(m_pUTF8Data) delete m_pUTF8Data;
		m_pUTF8Data = NULL;
		m_nLength = 0;
		m_nUTF8Length = 0;
	}
}
