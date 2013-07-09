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

static int nSCCoeffsTab[5] = { 128, 16, 256, 32, 64 };
static int nSCBPDiscardedTab[7] = { 0, 1, 2, 3, 4, 6, 8 };

ScalableColorType::ScalableColorType()
{
	m_nNumOfBitplanesDiscarded = 0;
	m_nNumOfCoeff = 0;
	m_pCoeffs = NULL;
}

ScalableColorType::~ScalableColorType()
{
	SAFE_DELETE(m_pCoeffs);
}

bool ScalableColorType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_NUM_OF_COEFF);
	m_nNumOfCoeff = -1;
	if(attr)
	{
		m_nNumOfCoeff = _wtoi(attr);
	}
	int tval = GetAttributeValue(m_nNumOfCoeff, nSCCoeffsTab, 5);
	if(tval < 0)	
	{
		BimXMLUtil::ReportError("invalid (ScalableColor)");
		return false;
	}
	
	attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_NUM_OF_BITPLANES_DISCARDED);
	m_nNumOfBitplanesDiscarded = -1;
	if(attr)
	{
		m_nNumOfBitplanesDiscarded = _wtoi(attr);
	}
	tval = GetAttributeValue(m_nNumOfBitplanesDiscarded, nSCBPDiscardedTab, 7);
	if(tval < 0)	
	{
		BimXMLUtil::ReportError("Invalid (ScalableColor)");
		return false;
	}

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_COEFF);
	if(childNode)
	{
		m_pCoeffs = new MPEG7IntegerVector();
		if(!m_pCoeffs || !m_pCoeffs->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pCoeffs);
			return false;
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (ScalableColor)");
		return false;
	}

	return true;
}

void ScalableColorType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	*fout << " " << ATTR_NAME_NUM_OF_BITPLANES_DISCARDED << "=\"" << m_nNumOfBitplanesDiscarded;
	*fout << "\" " << ATTR_NAME_NUM_OF_COEFF << "=\"" << m_nNumOfCoeff << '"';
	*fout << '>' << std::endl;

	if(m_pCoeffs)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_COEFF);
		*fout << std::endl;
		BimXMLUtil::PrintIndent(fout, nIndentLevel + 1);
		m_pCoeffs->PrintXML(fout, nIndentLevel + 1);
		*fout << std::endl;
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_COEFF);
		*fout << std::endl;
	}
}

int ScalableColorType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount += 3 + 3;
	if(m_pCoeffs) bitCount += m_pCoeffs->GetNumberOfBits();
	return bitCount;
}

bool ScalableColorType::Validate()
{
	return true;
}

bool ScalableColorType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pCoeffs);

	bret = writer->WriteBits(m_nNumOfBitplanesDiscarded, 3);

	if(bret)
	{
		bret = writer->WriteBits(m_nNumOfCoeff, 3);
	}

	if(bret)
	{
		bret = m_pCoeffs->WriteBitstream(writer);
	}

	return bret;
}

bool ScalableColorType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pCoeffs);
	
	bool bret = true;

	do {
		bret = reader->ReadBits(&m_nNumOfBitplanesDiscarded, 3);
		if(!bret) break;

		bret = reader->ReadBits(&m_nNumOfCoeff, 3);
		if(m_nNumOfCoeff > 5) 
		{
			bret = false;
		}
		if(!bret) break;

		m_pCoeffs = new MPEG7IntegerVector();
		BREAK_IFNULL(m_pCoeffs);
		bret = m_pCoeffs->ReadBitstream(reader);
	}
	while(false);

	return bret;
}

int ScalableColorType::GetAttributeValue(int value, int *table, int tableLength)
{
	if(table == NULL) return -1;
	for(int i = 0; i< tableLength; i++)
	{
		if(table[i] == value) return i;
	}
	return -1;
}

bool  ScalableColorType::SetNumberOfCoefficients(int val)
{
	m_nNumOfCoeff = GetAttributeValue(val, nSCCoeffsTab, 5);
	return (m_nNumOfCoeff >= 0) ? true : false;
}

bool  ScalableColorType::SetNumberOfBitplanesDiscarded(int val)
{
	m_nNumOfBitplanesDiscarded = GetAttributeValue(val, nSCBPDiscardedTab, 7);
	return (m_nNumOfBitplanesDiscarded >= 0) ? true : false;
}
