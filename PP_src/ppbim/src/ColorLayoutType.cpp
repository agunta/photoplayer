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

int GetNumberOfCLCoeff(int coeff_type)
{
	int coeff_num = -1;
	switch (coeff_type)
	{
	case CL_COEFF_14:
		coeff_num = 14;
		break;
	case CL_COEFF_20:
		coeff_num = 20;
		break;
	case CL_COEFF_27:
		coeff_num = 27;
		break;
	case CL_COEFF_5:
		coeff_num = 5;
		break;
	case CL_COEFF_63:
		coeff_num = 63;
		break;
	case CL_COEFF_9:
		coeff_num = 9;
		break;
	default:
		break;
	}
	return coeff_num;
}


ColorLayoutType::ColorLayoutType()
{
	m_nYDCCoeff = 0;
	m_nCbDCCoeff = 0;
	m_nCrDCCoeff = 0;

	m_pYACCoeff = NULL;
	m_pCbACCoeff = NULL;
	m_pCrACCoeff = NULL;

	m_nYACType = -1;
	m_nCbCrACType = -1;
}

ColorLayoutType::~ColorLayoutType()
{
	SAFE_DELETE(m_pYACCoeff);
	SAFE_DELETE(m_pCbACCoeff);
	SAFE_DELETE(m_pCrACCoeff);
}

bool ColorLayoutType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_YDCCOEFF);
	if(!BimXMLUtil::ParseElementInt(childNode, &m_nYDCCoeff))
	{
		BimXMLUtil::ReportError("invalid (ColorLayout)");
		return false;
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_CBDCCOEFF);
	if(!BimXMLUtil::ParseElementInt(childNode, &m_nCbDCCoeff))
	{
		BimXMLUtil::ReportError("invalid (ColorLayout)");
		return false;
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_CRDCCOEFF);
	if(!BimXMLUtil::ParseElementInt(childNode, &m_nCrDCCoeff))
	{
		BimXMLUtil::ReportError("invalid (ColorLayout)");
		return false;
	}

	m_pYACCoeff = ParseVectorList(ELEM_NAME_YACCOEFF14, node, 14);
	if(m_pYACCoeff) m_nYACType = CL_COEFF_14;
	if(!m_pYACCoeff)
	{
		m_pYACCoeff = ParseVectorList(ELEM_NAME_YACCOEFF20, node, 20);
		if(m_pYACCoeff) m_nYACType = CL_COEFF_20;
	}
	if(!m_pYACCoeff)
	{
		m_pYACCoeff = ParseVectorList(ELEM_NAME_YACCOEFF27, node, 27);
		if(m_pYACCoeff) m_nYACType = CL_COEFF_27;
	}
	if(!m_pYACCoeff)
	{
		m_pYACCoeff = ParseVectorList(ELEM_NAME_YACCOEFF5, node, 5);
		if(m_pYACCoeff) m_nYACType = CL_COEFF_5;
	}
	if(!m_pYACCoeff)
	{
		m_pYACCoeff = ParseVectorList(ELEM_NAME_YACCOEFF63, node, 63);
		if(m_pYACCoeff) m_nYACType = CL_COEFF_63;
	}
	if(!m_pYACCoeff)
	{
		m_pYACCoeff = ParseVectorList(ELEM_NAME_YACCOEFF9, node, 9);
		if(m_pYACCoeff) m_nYACType = CL_COEFF_9;
	}
	if(!m_pYACCoeff)
	{
		BimXMLUtil::ReportError("invalid (ColorLayout-YACCoeff)");
		return false;
	}


	m_pCbACCoeff = ParseVectorList(ELEM_NAME_CBACCOEFF14, node, 14);
	if(m_pCbACCoeff) m_nCbCrACType = CL_COEFF_14;
	if(!m_pCbACCoeff)
	{
		m_pCbACCoeff = ParseVectorList(ELEM_NAME_CBACCOEFF20, node, 20);
		if(m_pCbACCoeff) m_nCbCrACType = CL_COEFF_20;
	}
	if(!m_pCbACCoeff)
	{
		m_pCbACCoeff = ParseVectorList(ELEM_NAME_CBACCOEFF27, node, 27);
		if(m_pCbACCoeff) m_nCbCrACType = CL_COEFF_27;
	}
	if(!m_pCbACCoeff)
	{
		m_pCbACCoeff = ParseVectorList(ELEM_NAME_CBACCOEFF5, node, 5);
		if(m_pCbACCoeff) m_nCbCrACType = CL_COEFF_5;
	}
	if(!m_pCbACCoeff)
	{
		m_pCbACCoeff = ParseVectorList(ELEM_NAME_CBACCOEFF63, node, 63);
		if(m_pCbACCoeff) m_nCbCrACType = CL_COEFF_63;
	}
	if(!m_pCbACCoeff)
	{
		m_pCbACCoeff = ParseVectorList(ELEM_NAME_CBACCOEFF9, node, 9);
		if(m_pCbACCoeff) m_nCbCrACType = CL_COEFF_9;
	}
	if(!m_pCbACCoeff)
	{
		BimXMLUtil::ReportError("invalid (ColorLayout-CbACCoeff)");
		return false;
	}

	m_pCrACCoeff = ParseVectorList(ELEM_NAME_CRACCOEFF14, node, 14);
	if(m_pCbACCoeff && (m_nCbCrACType != CL_COEFF_14)) SAFE_DELETE(m_pCrACCoeff);
	if(!m_pCrACCoeff)
	{
		m_pCrACCoeff = ParseVectorList(ELEM_NAME_CRACCOEFF20, node, 20);
		if(m_pCbACCoeff && (m_nCbCrACType != CL_COEFF_20)) SAFE_DELETE(m_pCrACCoeff);
	}
	if(!m_pCrACCoeff)
	{
		m_pCrACCoeff = ParseVectorList(ELEM_NAME_CRACCOEFF27, node, 27);
		if(m_pCbACCoeff && (m_nCbCrACType != CL_COEFF_27)) SAFE_DELETE(m_pCrACCoeff);
	}
	if(!m_pCrACCoeff)
	{
		m_pCrACCoeff = ParseVectorList(ELEM_NAME_CRACCOEFF5, node, 5);
		if(m_pCbACCoeff && (m_nCbCrACType != CL_COEFF_5)) SAFE_DELETE(m_pCrACCoeff);
	}
	if(!m_pCrACCoeff)
	{
		m_pCrACCoeff = ParseVectorList(ELEM_NAME_CRACCOEFF63, node, 63);
		if(m_pCbACCoeff && (m_nCbCrACType != CL_COEFF_63)) SAFE_DELETE(m_pCrACCoeff);
	}
	if(!m_pCrACCoeff)
	{
		m_pCrACCoeff = ParseVectorList(ELEM_NAME_CRACCOEFF9, node, 9);
		if(m_pCbACCoeff && (m_nCbCrACType != CL_COEFF_9)) SAFE_DELETE(m_pCrACCoeff);
	}
	if(!m_pCrACCoeff)
	{
		BimXMLUtil::ReportError("invalid (ColorLayout-CrACCoeff)");
		return false;
	}

	return true;
}

void ColorLayoutType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_YDCCOEFF);
	*fout << m_nYDCCoeff;
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_YDCCOEFF);
	*fout << std::endl;

	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CBDCCOEFF);
	*fout << m_nCbDCCoeff;
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CBDCCOEFF);
	*fout << std::endl;

	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CRDCCOEFF);
	*fout << m_nCrDCCoeff;
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CRDCCOEFF);
	*fout << std::endl;

	char *ename = NULL;
	if(m_nYACType == CL_COEFF_14){ ename = ELEM_NAME_YACCOEFF14; }
	else if(m_nYACType == CL_COEFF_20){ ename = ELEM_NAME_YACCOEFF20; }
	else if(m_nYACType == CL_COEFF_27){ ename = ELEM_NAME_YACCOEFF27; }
	else if(m_nYACType == CL_COEFF_5){ ename = ELEM_NAME_YACCOEFF5; }
	else if(m_nYACType == CL_COEFF_63){ ename = ELEM_NAME_YACCOEFF63; }
	else if(m_nYACType == CL_COEFF_9){ ename = ELEM_NAME_YACCOEFF9; }
	if(ename && m_pYACCoeff)
	{
		BimXMLUtil::PrintXMLDS( (PPMpeg7Desc*) m_pYACCoeff, fout, nIndentLevel, ename, true);
	}	

	ename = NULL;
	if(m_nCbCrACType == CL_COEFF_14){ ename = ELEM_NAME_CBACCOEFF14; }
	else if(m_nCbCrACType == CL_COEFF_20){ ename = ELEM_NAME_CBACCOEFF20; }
	else if(m_nCbCrACType == CL_COEFF_27){ ename = ELEM_NAME_CBACCOEFF27; }
	else if(m_nCbCrACType == CL_COEFF_5){ ename = ELEM_NAME_CBACCOEFF5; }
	else if(m_nCbCrACType == CL_COEFF_63){ ename = ELEM_NAME_CBACCOEFF63; }
	else if(m_nCbCrACType == CL_COEFF_9){ ename = ELEM_NAME_CBACCOEFF9; }
	if(ename && m_pCbACCoeff)
	{
		BimXMLUtil::PrintXMLDS( (PPMpeg7Desc*) m_pCbACCoeff, fout, nIndentLevel, ename, true);
	}	

	ename = NULL;
	if(m_nCbCrACType == CL_COEFF_14){ ename = ELEM_NAME_CRACCOEFF14; }
	else if(m_nCbCrACType == CL_COEFF_20){ ename = ELEM_NAME_CRACCOEFF20; }
	else if(m_nCbCrACType == CL_COEFF_27){ ename = ELEM_NAME_CRACCOEFF27; }
	else if(m_nCbCrACType == CL_COEFF_5){ ename = ELEM_NAME_CRACCOEFF5; }
	else if(m_nCbCrACType == CL_COEFF_63){ ename = ELEM_NAME_CRACCOEFF63; }
	else if(m_nCbCrACType == CL_COEFF_9){ ename = ELEM_NAME_CRACCOEFF9; }
	if(ename && m_pCrACCoeff)
	{
		BimXMLUtil::PrintXMLDS( (PPMpeg7Desc*) m_pCrACCoeff, fout, nIndentLevel, ename, true);
	}	
}

int ColorLayoutType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount += 6;
	bitCount += 6;
	bitCount += 6;

	bitCount += 3;
	if(m_pYACCoeff)
	{
		bitCount += m_pYACCoeff->GetNumberOfBits();
	}

	bitCount += 3;
	if(m_pCbACCoeff)
	{
		bitCount += m_pCbACCoeff->GetNumberOfBits();
	}
	if(m_pCrACCoeff)
	{
		bitCount += m_pCrACCoeff->GetNumberOfBits();
	}
	return bitCount;
}

bool ColorLayoutType::Validate()
{
	return true;
}

bool ColorLayoutType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pYACCoeff);
	RETURN_IFNULL(m_pCbACCoeff);
	RETURN_IFNULL(m_pCrACCoeff);
	if(m_nCbCrACType < 0 || m_nYACType < 0) return false;

	bret = writer->WriteBits(m_nYDCCoeff, 6);
	if(bret)
	{
		bret = writer->WriteBits(m_nCbDCCoeff, 6);
	}
	if(bret)
	{
		bret = writer->WriteBits(m_nCrDCCoeff, 6);
	}

	if(bret)
	{
		bret = writer->WriteBits(m_nYACType, 3);
	}
	if(bret)
	{
		bret = m_pYACCoeff->WriteBitstream(writer);
	}

	if(bret)
	{
		bret = writer->WriteBits(m_nCbCrACType, 3);
	}
	if(bret)
	{
		bret = m_pCbACCoeff->WriteBitstream(writer);
	}
	if(bret)
	{
		bret = m_pCrACCoeff->WriteBitstream(writer);
	}

	return bret;
}

bool ColorLayoutType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pYACCoeff);
	SAFE_DELETE(m_pCbACCoeff);
	SAFE_DELETE(m_pCrACCoeff);
	
	bool bret = true;

	do {
		bret = reader->ReadBits(&m_nYDCCoeff, 6);
		if(!bret) break;

		bret = reader->ReadBits(&m_nCbDCCoeff, 6);
		if(!bret) break;

		bret = reader->ReadBits(&m_nCrDCCoeff, 6);
		if(!bret) break;

		bret = reader->ReadBits(&m_nYACType, 3);
		if(!bret) break;

		int nMax = -1;
		nMax = GetNumberOfCLCoeff(m_nYACType);
		if(nMax <= 0) {
			bret = false;
			break;
		}
		m_pYACCoeff = new ValueListType(nMax, 5, 20);
		BREAK_IFNULL(m_pYACCoeff);
		bret = m_pYACCoeff->ReadBitstream(reader);
		if(!bret) break;

		bret = reader->ReadBits(&m_nCbCrACType, 3);
		if(!bret) break;

		nMax = -1;
		nMax = GetNumberOfCLCoeff(m_nCbCrACType);
		if(nMax <= 0) {
			bret = false;
			break;
		}
		m_pCbACCoeff = new ValueListType(nMax, 5, 20);
		BREAK_IFNULL(m_pCbACCoeff);
		bret = m_pCbACCoeff->ReadBitstream(reader);
		if(!bret) break;

		m_pCrACCoeff = new ValueListType(nMax, 5, 20);
		BREAK_IFNULL(m_pCrACCoeff);
		bret = m_pCrACCoeff->ReadBitstream(reader);
		if(!bret) break;
	}
	while(false);

	return bret;
}


ValueListType *ColorLayoutType::ParseVectorList(char *elem_name, DOMNode *node, int length)
{
	ValueListType *pValues = NULL;
	if(!elem_name || !node || length <= 0) return NULL;
	int nLen = 0;
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, elem_name);
	if(!childNode) return NULL;

	pValues = new ValueListType(length, 5, 20);
	if(!pValues) return NULL;
	if(!pValues->BuildFromXML(childNode))
	{
		SAFE_DELETE(pValues);
	}
	return pValues;
}
