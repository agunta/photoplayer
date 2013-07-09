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

static int nCSColorQuantTab[8] = { -1, 32, 64, 128, 256, -1, -1, -1, };

ColorStructureType::ColorStructureType()
{
	m_nColorQuant = 0;
	m_pValues = NULL;
}

ColorStructureType::~ColorStructureType()
{
	SAFE_DELETE(m_pValues);
}

bool ColorStructureType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_COLOR_QUANT);
	m_nColorQuant = -1;
	if(attr)
	{
		m_nColorQuant = _wtoi(attr);
	}
	if(m_nColorQuant < 1 || m_nColorQuant > 4)	
	{
		BimXMLUtil::ReportError("invalid (ColorStructure)");
		return false;
	}
	

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_VALUES);
	if(childNode)
	{
		m_pValues = new CSValues_anonymousType();
		if(!m_pValues || !m_pValues->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pValues);
			return false;
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (ColorStructure)");
		return false;
	}
	// to do: check if m_nColorQant is consistent with the number of values

	return true;
}

void ColorStructureType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	*fout << " " << ATTR_NAME_COLOR_QUANT << "=\"" << m_nColorQuant << '"';
	*fout << '>' << std::endl;

	if(m_pValues)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_VALUES);
		*fout << std::endl;
		BimXMLUtil::PrintIndent(fout, nIndentLevel + 1);
		m_pValues->PrintXML(fout, nIndentLevel + 1);
		*fout << std::endl;
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_VALUES);
		*fout << std::endl;
	}
}

int ColorStructureType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount += 3;
	if(m_pValues) bitCount += m_pValues->GetNumberOfBits();
	return bitCount;
}

bool ColorStructureType::Validate()
{
	return true;
}

bool ColorStructureType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pValues);

	bret = writer->WriteBits(m_nColorQuant, 3);

	if(bret)
	{
		bret = m_pValues->WriteBitstream(writer);
	}

	return bret;
}

bool ColorStructureType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pValues);
	
	bool bret = true;

	do {
		bret = reader->ReadBits(&m_nColorQuant, 3);
		if(!bret) break;

		m_pValues = new CSValues_anonymousType();
		BREAK_IFNULL(m_pValues);
		bret = m_pValues->ReadBitstream(reader);
	}
	while(false);

	return bret;
}
