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

InlineMediaType::InlineMediaType()
{
	m_pType = NULL;
	m_pMediaData16 = NULL;
	m_pMediaData64 = NULL;
}

InlineMediaType::~InlineMediaType()
{
	if(m_pType) delete m_pType;
	if(m_pMediaData16) delete m_pMediaData16;
	if(m_pMediaData64) delete m_pMediaData64;
}

bool InlineMediaType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	m_pType = BimXMLUtil::GetAttributeString(node, ATTR_NAME_TYPE);
	if(m_pType == NULL)
	{
		BimXMLUtil::ReportError("Missing attribute 'type' in element <InlineMedia>"); 
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_MEDIA_DATA16);
	if(childNode)
	{
		m_pMediaData16 = new HexBinary();
		if(!m_pMediaData16) return false;
		if(!m_pMediaData16->BuildFromXML(childNode))
		{
			delete m_pMediaData16;
			m_pMediaData16 = NULL;
		}
	}
	else
	{
		DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_MEDIA_DATA64);
		if(childNode)
		{
			m_pMediaData64 = new Base64Binary();
			if(!m_pMediaData64) return false;
			if(!m_pMediaData64->BuildFromXML(childNode))
			{
				delete m_pMediaData64;
				m_pMediaData64 = NULL;
			}
		}
		else
		{
			BimXMLUtil::ReportError("Missing Element <MediaDataXX> in element <InlineMedia>"); 
		}
	}
	return true;
}

int InlineMediaType::GetNumberOfBits()
{
	int bitCount = 0;
	if(m_pType)
	{
		bitCount += m_pType->GetNumberOfBits();
	}
	bitCount++;
	if(m_pMediaData16)
	{
		bitCount += m_pMediaData16->GetNumberOfBits();
	}
	else if(m_pMediaData64)
	{
		bitCount += m_pMediaData64->GetNumberOfBits();
	}
	return bitCount;
}

bool InlineMediaType::Validate()
{
	return true;
}

bool InlineMediaType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pType);
	bret = m_pType->WriteBitstream(writer);
	int nChoice = (m_pMediaData16 != NULL) ? 0 : 1;
	if(bret)
	{
		bret = writer->WriteBits(nChoice, 1);
	}
	if(bret)
	{
		if(nChoice == 0)
		{
			bret = m_pMediaData16->WriteBitstream(writer);
		}
		else
		{
			bret = m_pMediaData64->WriteBitstream(writer);
		}

	}
	return bret;
}

bool InlineMediaType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pType);
	SAFE_DELETE(m_pMediaData16);
	SAFE_DELETE(m_pMediaData64);
	
	bool bret = true;

	do {
		m_pType = new DescString();
		BREAK_IFNULL(m_pType);
		bret = m_pType->ReadBitstream(reader);
		if(!bret) break;

		int nChoice;
		bret = reader->ReadBits(&nChoice, 1);
		if(!bret) break;
		if(nChoice == 0)
		{
			m_pMediaData16 = new HexBinary();
			BREAK_IFNULL(m_pMediaData16);
			bret = m_pMediaData16->ReadBitstream(reader);
		}
		else
		{
			m_pMediaData64 = new Base64Binary();
			BREAK_IFNULL(m_pMediaData64);
			bret = m_pMediaData64->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}

void InlineMediaType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(m_pMediaData16)
	{
		m_pMediaData16->PrintXML(fout, nIndentLevel);
	}
	else if(m_pMediaData64)
	{
		m_pMediaData64->PrintXML(fout, nIndentLevel);
	}
}
