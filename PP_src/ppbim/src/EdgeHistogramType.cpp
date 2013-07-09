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

#define EDGE_BIN_COUNT 80
#define EDGE_ITEM_LENGTH 3

EdgeHistogramType::EdgeHistogramType()
{
	m_pBinCounts = NULL;
}

EdgeHistogramType::~EdgeHistogramType()
{
	if(m_pBinCounts) delete m_pBinCounts;
}

bool EdgeHistogramType::BuildFromXML(DOMNode *node)
{
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_BIN_COUNTS);
	if(childNode)
	{
		m_pBinCounts = new ValueListType(EDGE_BIN_COUNT, EDGE_ITEM_LENGTH, 20);
		if(!m_pBinCounts || !m_pBinCounts->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pBinCounts);
			return false;
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (EdgeHistogramType)");
		return false;
	}
	return true;
}

void EdgeHistogramType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(m_pBinCounts)
	{
		BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pBinCounts, fout, nIndentLevel + 1,ELEM_NAME_BIN_COUNTS , true);
	}
}

int EdgeHistogramType::GetNumberOfBits()
{
	int bitCount = 0;
	if(m_pBinCounts)
	{
		bitCount += m_pBinCounts->GetNumberOfBits();
	}
	return bitCount;
}

bool EdgeHistogramType::Validate()
{
	return true;
}

bool EdgeHistogramType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pBinCounts);

	bret = m_pBinCounts->WriteBitstream(writer);

	return bret;
}

bool EdgeHistogramType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pBinCounts);
	
	bool bret = true;

	do {
		m_pBinCounts = new ValueListType(EDGE_BIN_COUNT, EDGE_ITEM_LENGTH, 20);
		BREAK_IFNULL(m_pBinCounts);
		bret = m_pBinCounts->ReadBitstream(reader);
	}
	while(false);

	return bret;
}
