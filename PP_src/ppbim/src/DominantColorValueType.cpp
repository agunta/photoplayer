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

#define DCINDEX_LIST_COUNT 3
#define DCINDEX_ITEM_LENGTH 12
#define DC_CV_ITEM_LENGTH 1

DominantColorValueType::DominantColorValueType()
{
	m_nPercentage = 0;
	m_pIndex = NULL;
	m_pColorVariance = NULL;
}

DominantColorValueType::~DominantColorValueType()
{
	SAFE_DELETE(m_pIndex);
	SAFE_DELETE(m_pColorVariance);
}

bool DominantColorValueType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_PERCENTAGE);
	if(childNode)
	{
		wchar_t *data = BimXMLUtil::GetNodeText(childNode);
		if(data)
		{
			m_nPercentage = _wtoi(data);
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (DominantColor)");
		return false;
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_INDEX);
	if(childNode)
	{
		m_pIndex = new ValueListType(DCINDEX_LIST_COUNT, DCINDEX_ITEM_LENGTH);
		if(!m_pIndex || !m_pIndex->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pIndex);
			return false;
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (DominantColor)");
		return false;
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_COLOR_VARIANCE);
	if(childNode)
	{
		m_pColorVariance = new ValueListType(DCINDEX_LIST_COUNT, DC_CV_ITEM_LENGTH);
		if(!m_pColorVariance || !m_pColorVariance->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pColorVariance);
			return false;
		}
	}

	return true;
}

void DominantColorValueType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_PERCENTAGE);
	*fout << m_nPercentage;
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_PERCENTAGE);
	*fout << std::endl;

	if(m_pIndex)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_INDEX);
		m_pIndex->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_INDEX);
		*fout << std::endl;
	}

	if(m_pColorVariance)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_INDEX);
		m_pColorVariance->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_INDEX);
		*fout << std::endl;
	}
}

int DominantColorValueType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount += 5;

	if(m_pIndex)
	{
		bitCount += m_pIndex->GetNumberOfBits();
	}

	bitCount++;
	if(m_pColorVariance)
	{
		bitCount += m_pColorVariance->GetNumberOfBits();
	}

	return bitCount;
}

bool DominantColorValueType::Validate()
{
	return true;
}

bool DominantColorValueType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pIndex);

	bret = writer->WriteBits(m_nPercentage, 5);

	if(bret)
	{
		bret = m_pIndex->WriteBitstream(writer);
	}

	bool bOpt = (m_pColorVariance == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = m_pColorVariance->WriteBitstream(writer);
	}

	return true;
}

bool DominantColorValueType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pIndex);
	SAFE_DELETE(m_pColorVariance);

	bool bret;
	do {
		bret = reader->ReadBits(&m_nPercentage, 5);
		if(!bret) break;

		m_pIndex = new ValueListType(DCINDEX_LIST_COUNT, DCINDEX_ITEM_LENGTH);
		BREAK_IFNULL(m_pIndex);
		bret = m_pIndex->ReadBitstream(reader);

		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pColorVariance = new ValueListType(DCINDEX_LIST_COUNT, DC_CV_ITEM_LENGTH);
			BREAK_IFNULL(m_pColorVariance);
			bret = m_pIndex->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}
