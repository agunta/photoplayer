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

ImageLocatorType::ImageLocatorType()
{
	m_pInlineMedia = NULL;
}

ImageLocatorType::~ImageLocatorType()
{
	if(m_pInlineMedia) delete m_pInlineMedia;
}

bool ImageLocatorType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_INLINE_MEDIA);
	if(childNode)
	{
		m_pInlineMedia = new InlineMediaType();
		if(!m_pInlineMedia) return false;
		if(!m_pInlineMedia->BuildFromXML(childNode))
		{
			delete m_pInlineMedia;
			m_pInlineMedia = NULL;
		}
	}
	return true;
}

int ImageLocatorType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pInlineMedia)
	{
		bitCount += m_pInlineMedia->GetNumberOfBits();
	}
	return bitCount;
}

bool ImageLocatorType::Validate()
{
	return true;
}

bool ImageLocatorType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOpt = (m_pInlineMedia == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pInlineMedia->WriteBitstream(writer);
	}
	return bret;
}

bool ImageLocatorType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pInlineMedia);
	
	bool bret = true;

	do {
		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pInlineMedia = new InlineMediaType();
			BREAK_IFNULL(m_pInlineMedia);
			bret = m_pInlineMedia->ReadBitstream(reader);
			if(!bret) break;
		}
	}
	while(false);

	return bret;
}

void ImageLocatorType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pInlineMedia)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_INLINE_MEDIA, true);
		m_pInlineMedia->PrintXML(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_INLINE_MEDIA);
		*fout << std::endl;
	}
}
