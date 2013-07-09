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

#define TERM_ALIAS_REFERENCE 0
#define TERM_URI_REFERENCE   1

TermReferenceType::TermReferenceType()
{
	m_pReferenceAlias = NULL;
	m_pReferenceURI = NULL;

}

TermReferenceType::~TermReferenceType()
{
	if(m_pReferenceAlias) delete m_pReferenceAlias;
	if(m_pReferenceURI) delete m_pReferenceURI;
}

bool TermReferenceType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ATTRIBUTE_NODE)
	{
		return false;
	}

	wchar_t *attrValue = (wchar_t*)node->getNodeValue(); 
	if(attrValue != NULL)
	{
		// to do: implement checking the termAlias/termURI
		if(IsAlias(attrValue))
		{
			m_pReferenceAlias = new DescString();
			if(m_pReferenceAlias) m_pReferenceAlias->SetData(attrValue);
		}
		else
		{
			m_pReferenceURI = new DescString();
			if(m_pReferenceURI) m_pReferenceURI->SetData(attrValue);
		}
	}
	return true;
}

int TermReferenceType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pReferenceAlias)
	{
		bitCount += m_pReferenceAlias->GetNumberOfBits();
	}
	else
	{
		bitCount += m_pReferenceURI->GetNumberOfBits();
	}
	return bitCount;
}

bool TermReferenceType::Validate()
{
	return true;
}

bool TermReferenceType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	if(m_pReferenceAlias == NULL && m_pReferenceURI == NULL) return false;
	int nChoice = (m_pReferenceAlias != NULL) ? TERM_ALIAS_REFERENCE : TERM_URI_REFERENCE;
	if(nChoice == TERM_URI_REFERENCE && m_pReferenceURI == NULL)
	{
		BimXMLUtil::ReportError("Invalid <TermReferenceType>");
		return false;
	}
	bret = writer->WriteBits(nChoice, 1);

	if(bret)
	{
		if(nChoice == TERM_ALIAS_REFERENCE)
		{
			bret = m_pReferenceAlias->WriteBitstream(writer);
		}
		else
		{
			bret = m_pReferenceURI->WriteBitstream(writer);
		}
	}
	return bret;
}

bool TermReferenceType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pReferenceAlias);
	SAFE_DELETE(m_pReferenceURI);
	
	bool bret;

	do {
		int nChoice;
		bret = reader->ReadBits(&nChoice, 1);
		if(!bret) break;
		if(nChoice == TERM_ALIAS_REFERENCE)
		{
			m_pReferenceAlias = new DescString();
			BREAK_IFNULL(m_pReferenceAlias);
			bret = m_pReferenceAlias->ReadBitstream(reader);
		}
		else
		{
			m_pReferenceURI = new DescString();
			BREAK_IFNULL(m_pReferenceURI);
			bret = m_pReferenceURI->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}

bool TermReferenceType::IsAlias(wchar_t *str)
{
	// to do: implement valid alias checking

	int strLen = (int)wcslen(str);
	int chCount = 0;
	for(int i = 0; i < strLen; i++)
	{
		if(str[i] == L':')
		{
			chCount++;
		}
	}
	return ((chCount > 1) ? true : false);
}

void TermReferenceType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(m_pReferenceURI)
	{
		m_pReferenceURI->PrintXML(fout, nIndentLevel);
	}
	else if(m_pReferenceAlias)
	{
		m_pReferenceAlias->PrintXML(fout, nIndentLevel);
	}
}
