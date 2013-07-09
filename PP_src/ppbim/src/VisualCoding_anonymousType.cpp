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

VisualCoding_anonymousType::VisualCoding_anonymousType()
{
	m_pFrame = NULL;
}

VisualCoding_anonymousType::~VisualCoding_anonymousType()
{
	SAFE_DELETE(m_pFrame);
}

bool VisualCoding_anonymousType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_FRAME);
	if(childNode)
	{
		m_pFrame = new Frame_anonymousType();
		if(!m_pFrame || !m_pFrame->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pFrame);
			return false;
		}
	}
	return true;
}

void VisualCoding_anonymousType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	if(m_pFrame)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_FRAME, false);
		m_pFrame->PrintXML(fout, nIndentLevel + 1);
		*fout << std::endl;
	}

}

int VisualCoding_anonymousType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pFrame)
	{
		bitCount += m_pFrame->GetNumberOfBits();
	}
	return bitCount;
}

bool VisualCoding_anonymousType::Validate()
{
	return true;
}

bool VisualCoding_anonymousType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOpt = (m_pFrame == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pFrame->WriteBitstream(writer);
	}
	return bret;
}

bool VisualCoding_anonymousType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pFrame);
	
	bool bret = true;

	do {
		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pFrame = new Frame_anonymousType();
			BREAK_IFNULL(m_pFrame);
			bret = m_pFrame->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}
