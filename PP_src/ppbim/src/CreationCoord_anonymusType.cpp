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

CreationCoord_anonymusType::CreationCoord_anonymusType()
{
	m_pLocation = NULL;
	m_pDate = NULL;
}

CreationCoord_anonymusType::~CreationCoord_anonymusType()
{
	if(m_pLocation) delete m_pLocation;
	if(m_pDate) delete m_pDate;
}

bool CreationCoord_anonymusType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_LOCATION);
	if(childNode)
	{
		m_pLocation = new PlaceType();
		if(!m_pLocation) return false;
		if(!m_pLocation->BuildFromXML(childNode))
		{
			delete m_pLocation;
			m_pLocation = NULL;
		}
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_DATE);
	if(childNode)
	{
		m_pDate = new TimeType();
		if(!m_pDate) return false;
		if(!m_pDate->BuildFromXML(childNode))
		{
			delete m_pDate;
			m_pDate = NULL;
		}
	}
	return true;
}

int CreationCoord_anonymusType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pLocation) bitCount += m_pLocation->GetNumberOfBits();
	bitCount++;
	if(m_pDate) bitCount += m_pDate->GetNumberOfBits();
	return bitCount;
}

bool CreationCoord_anonymusType::Validate()
{
	return true;
}

bool CreationCoord_anonymusType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret = true;
	bool bOptGroup = (m_pLocation == NULL) ? false : true;
	bret = writer->WriteBool(bOptGroup);
	if(bret && bOptGroup)
	{
		bret = m_pLocation->WriteBitstream(writer);
	}
	bOptGroup = (m_pDate == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = m_pDate->WriteBitstream(writer);
	}
	return bret;
}

bool CreationCoord_anonymusType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pLocation);
	SAFE_DELETE(m_pDate);
	
	bool bret;
	do {
		bool bOptGroup;
		bret = reader->ReadBool(&bOptGroup);
		if(!bret) break;
		if(bOptGroup)
		{
			m_pLocation = new PlaceType();
			RETURN_IFNULL(m_pLocation);
			bret = m_pLocation->ReadBitstream(reader);
			if(!bret) break;
		}

		bret = reader->ReadBool(&bOptGroup);
		if(!bret) break;
		if(bOptGroup)
		{
			m_pDate = new TimeType();
			RETURN_IFNULL(m_pDate);
			bret = m_pDate->ReadBitstream(reader);
		}
	}
	while(false);
	
	return bret;
}

void CreationCoord_anonymusType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pLocation)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_LOCATION, false);
		m_pLocation->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_LOCATION);
		*fout << std::endl;
	}

	if(m_pDate)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_DATE, true);
		*fout << std::endl;
		m_pDate->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_DATE);
		*fout << std::endl;
	}
}
