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

TitleMediaType::TitleMediaType()
{
	m_pTitleImage = NULL;
}

TitleMediaType::~TitleMediaType()
{
	if(m_pTitleImage) delete m_pTitleImage;
}

bool TitleMediaType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_TITLE_IMAGE);
	if(childNode)
	{
		m_pTitleImage = new ImageLocatorType();
		if(!m_pTitleImage) return false;
		if(!m_pTitleImage->BuildFromXML(childNode))
		{
			delete m_pTitleImage;
			m_pTitleImage = NULL;
		}
	}
	return true;
}
int TitleMediaType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pTitleImage)
	{
		bitCount += m_pTitleImage->GetNumberOfBits();
	}
	return bitCount;
}

bool TitleMediaType::Validate()
{
	return true;
}
bool TitleMediaType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOpt = (m_pTitleImage == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pTitleImage->WriteBitstream(writer);
	}
	return bret;
}
bool TitleMediaType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pTitleImage);
	
	bool bret = true;

	do {
		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pTitleImage = new ImageLocatorType();
			BREAK_IFNULL(m_pTitleImage);
			bret = m_pTitleImage->ReadBitstream(reader);
			if(!bret) break;
		}
	}
	while(false);

	return bret;
}

void TitleMediaType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pTitleImage)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_TITLE_IMAGE, true);
		*fout << std::endl;
		m_pTitleImage->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_TITLE_IMAGE);
		*fout << std::endl;
	}
}
