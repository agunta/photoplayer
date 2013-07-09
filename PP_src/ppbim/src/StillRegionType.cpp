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

StillRegionType::StillRegionType()
{
	m_pId = NULL;
	m_pChild = NULL;
	m_pVisualDescriptionScheme = NULL;
}

StillRegionType::~StillRegionType()
{
	if(m_pId) delete m_pId;
	if(m_pChild) delete m_pChild;
	if(m_pVisualDescriptionScheme) delete m_pVisualDescriptionScheme;
}

bool StillRegionType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	m_pChild =new SegmentType_content();
	if(!m_pChild || !m_pChild->BuildFromXML(node))
	{
		SAFE_DELETE(m_pChild);
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_VISUAL_DESC_SCHEME);
	if(childNode)
	{
		m_pVisualDescriptionScheme = new StillRegionFeatureType();
		if(!m_pVisualDescriptionScheme || !m_pVisualDescriptionScheme->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pVisualDescriptionScheme);
			return false;
		}
	}
	return true;
}

void StillRegionType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << " id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;
	
	if(m_pChild)
	{
		m_pChild->PrintXML(fout, nIndentLevel);
	}
	if(m_pVisualDescriptionScheme)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_VISUAL_DESC_SCHEME, false);
		*fout << " xsi:type=\"StillRegionFeatureType\"";
		m_pVisualDescriptionScheme->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_VISUAL_DESC_SCHEME);
		*fout << std::endl;
	}
}

int StillRegionType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	if(m_pChild)
	{
		bitCount += m_pChild->GetNumberOfBits();
	}
	bitCount++;
	if(m_pVisualDescriptionScheme)
	{
		bitCount += m_pVisualDescriptionScheme->GetNumberOfBits();
	}
	return bitCount;
}

bool StillRegionType::Validate()
{
	return true;
}

bool StillRegionType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pChild);
	bool bHasId = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bHasId);
	if(bret && bHasId)
	{
		bret = m_pId->WriteBitstream(writer);
	}

	if(bret)
	{
		bret = m_pChild->WriteBitstream(writer);
	}

	bool bOpt = (m_pVisualDescriptionScheme == NULL) ? false : true;
	if(bret) {
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = m_pVisualDescriptionScheme->WriteBitstream(writer);
	}

	return bret;
}

bool StillRegionType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pChild);
	SAFE_DELETE(m_pVisualDescriptionScheme);
	
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
		m_pChild = new SegmentType_content();
		BREAK_IFNULL(m_pChild);
		bret = m_pChild->ReadBitstream(reader);
		if(!bret) break;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pVisualDescriptionScheme = new StillRegionFeatureType();
			BREAK_IFNULL(m_pVisualDescriptionScheme);
			bret = m_pVisualDescriptionScheme->ReadBitstream(reader);
			if(!bret) break;
		}
	}
	while(false);

	return bret;
}
