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

GeographicPosition_anonymusType::GeographicPosition_anonymusType()
{
	m_pDatum = NULL;
	m_pPoint = NULL;
}

GeographicPosition_anonymusType::~GeographicPosition_anonymusType()
{
	if(m_pPoint) delete m_pPoint;
	if(m_pDatum) delete m_pDatum;
}

bool GeographicPosition_anonymusType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pDatum = BimXMLUtil::GetAttributeString(node, ATTR_NAME_DATUM);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_POINT);
	if(childNode)
	{
		m_pPoint = new GeographicPointType();
		if(!m_pPoint) return false;
		if(!m_pPoint->BuildFromXML(childNode))
		{
			delete m_pPoint;
			m_pPoint = NULL;
		}
	}
	if(!m_pPoint)
	{
		BimXMLUtil::ReportError("Missing or invalid element <Point> in element <GeographicPosition>");
		return false;
	}
	return true;
}

int GeographicPosition_anonymusType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pDatum)
	{
		bitCount += m_pDatum->GetNumberOfBits();
	}
	if(m_pPoint)
	{
		bitCount += m_pPoint->GetNumberOfBits();
	}
	return bitCount;
}

bool GeographicPosition_anonymusType::Validate()
{
	return false;
}

bool GeographicPosition_anonymusType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pPoint);
	bool bHasOpt = (m_pDatum == NULL) ? false : true;
	bret = writer->WriteBool(bHasOpt);
	if(bret && bHasOpt)
	{
		bret = m_pDatum->WriteBitstream(writer);
	}
	if(bret)
	{
		bret = m_pPoint->WriteBitstream(writer);
	}
	return bret;
}

bool GeographicPosition_anonymusType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pDatum);
	SAFE_DELETE(m_pPoint);
	
	bool bret = true;

	do {
		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pDatum = new DescString();
			BREAK_IFNULL(m_pDatum);
			bret = m_pDatum->ReadBitstream(reader);
			if(!bret) break;
		}
		m_pPoint = new GeographicPointType();
		BREAK_IFNULL(m_pPoint);
		bret = m_pPoint->ReadBitstream(reader);
	}
	while(false);

	return bret;
}

void GeographicPosition_anonymusType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pDatum)
	{
		*fout << " datum=\"";
		m_pDatum->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	if(m_pPoint)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_POINT, false);
		m_pPoint->PrintXML(fout, nIndentLevel + 1);
	}

}