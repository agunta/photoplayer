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

ImageType::ImageType()
{
	m_pId = NULL;
	m_pImage = NULL;
}

ImageType::~ImageType()
{
	if(m_pId) delete m_pId;
	if(m_pImage) delete m_pImage;
}

bool ImageType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_IMAGE);
	if(childNode)
	{
		m_pImage = new StillRegionType();
		if(!m_pImage || !m_pImage->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pImage);
			return false;
		}
	}
	return true;
}

void ImageType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << "id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
	}
	*fout << '>' << std::endl;
	
	if(m_pImage)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_IMAGE, false);
		m_pImage->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_IMAGE);
		*fout << std::endl;
	}
}

int ImageType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	if(m_pImage)
	{
		bitCount += m_pImage->GetNumberOfBits();
	}
	return bitCount;
}

bool ImageType::Validate()
{
	return true;
}

bool ImageType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pImage);
	bool bHasId = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bHasId);
	if(bret && bHasId)
	{
		bret = m_pId->WriteBitstream(writer);
	}
	if(bret)
	{
		bret = m_pImage->WriteBitstream(writer);
	}
	return bret;
}

bool ImageType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pImage);
	
	bool bret = true;

	do {
		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pId = new DescString();
			BREAK_IFNULL(m_pId);
			bret = m_pId->ReadBitstream(reader);
			if(!bret) break;
		}
		m_pImage = new StillRegionType();
		BREAK_IFNULL(m_pImage);
		bret = m_pImage->ReadBitstream(reader);
	}
	while(false);

	return bret;
}
