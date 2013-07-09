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

StillRegionFeatureType::StillRegionFeatureType()
{
	m_pId = NULL;
	m_pDominantColor = NULL;;
	m_pScalableColor = NULL;;
	m_pColorStructure = NULL;;
	m_pColorLayout = NULL;;
	m_pEdge = NULL;;
	m_pHomogeneousTexture = NULL;;

}

StillRegionFeatureType::~StillRegionFeatureType()
{
	if(m_pId) delete m_pId;
	if(m_pDominantColor) delete m_pDominantColor;
	if(m_pScalableColor) delete m_pScalableColor;
	if(m_pColorStructure) delete m_pColorStructure;
	if(m_pColorLayout) delete m_pColorLayout;
	if(m_pEdge) delete m_pEdge;
	if(m_pHomogeneousTexture) delete m_pHomogeneousTexture;
}

bool StillRegionFeatureType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_DOMINANT_COLOR);
	if(childNode)
	{
		m_pDominantColor = new DominantColorType();
		if(!m_pDominantColor || !m_pDominantColor->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pDominantColor);
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_SCALABLE_COLOR);
	if(childNode)
	{
		m_pScalableColor = new ScalableColorType();
		if(!m_pScalableColor || !m_pScalableColor->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pScalableColor);
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_COLOR_STRUCTURE);
	if(childNode)
	{
		m_pColorStructure = new ColorStructureType();
		if(!m_pColorStructure || !m_pColorStructure->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pColorStructure);
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_COLOR_LAYOUT);
	if(childNode)
	{
		m_pColorLayout = new ColorLayoutType();
		if(!m_pColorLayout || !m_pColorLayout->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pColorLayout);
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_EDGE);
	if(childNode)
	{
		m_pEdge = new EdgeHistogramType();
		if(!m_pEdge || !m_pEdge->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pEdge);
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_HOMOGENEOUS_PATTERN);
	if(childNode)
	{
		m_pHomogeneousTexture = new HomogeneousTextureType();
		if(!m_pHomogeneousTexture || !m_pHomogeneousTexture->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pHomogeneousTexture);
		}
	}

	return true;
}

void StillRegionFeatureType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << " id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pDominantColor, fout, nIndentLevel, ELEM_NAME_DOMINANT_COLOR, true);
	BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pScalableColor, fout, nIndentLevel, ELEM_NAME_SCALABLE_COLOR, false);
	BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pColorStructure, fout, nIndentLevel, ELEM_NAME_COLOR_STRUCTURE, false);
	BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pColorLayout, fout, nIndentLevel, ELEM_NAME_COLOR_LAYOUT, true);
	BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pEdge, fout, nIndentLevel, ELEM_NAME_EDGE, true);
	BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pHomogeneousTexture, fout, nIndentLevel, ELEM_NAME_HOMOGENEOUS_PATTERN, true);
}

int StillRegionFeatureType::GetNumberOfBits()
{
	int bitCount = 0;

	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}

	bitCount++;
	if(m_pDominantColor)
	{
		bitCount += m_pDominantColor->GetNumberOfBits();
	}

	bitCount++;
	if(m_pScalableColor)
	{
		bitCount += m_pScalableColor->GetNumberOfBits();
	}

	bitCount++;
	if(m_pColorStructure)
	{
		bitCount += m_pColorStructure->GetNumberOfBits();
	}

	bitCount++;
	if(m_pColorLayout)
	{
		bitCount += m_pColorLayout->GetNumberOfBits();
	}

	bitCount++;
	if(m_pEdge)
	{
		bitCount += m_pEdge->GetNumberOfBits();
	}

	bitCount++;
	if(m_pHomogeneousTexture)
	{
		bitCount += m_pHomogeneousTexture->GetNumberOfBits();
	}

	return bitCount;
}

bool StillRegionFeatureType::Validate()
{
	return true;
}

bool StillRegionFeatureType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;

	bret = BimXMLUtil::WriteOptionalDS((PPMpeg7Desc*)m_pId, writer);

	if(bret)
	{
		bret = BimXMLUtil::WriteOptionalDS((PPMpeg7Desc*)m_pDominantColor, writer);
	}

	if(bret)
	{
		bret = BimXMLUtil::WriteOptionalDS((PPMpeg7Desc*)m_pScalableColor, writer);
	}

	if(bret)
	{
		bret = BimXMLUtil::WriteOptionalDS((PPMpeg7Desc*)m_pColorStructure, writer);
	}

	if(bret)
	{
		bret = BimXMLUtil::WriteOptionalDS((PPMpeg7Desc*)m_pColorLayout, writer);
	}
	
	if(bret)
	{
		bret = BimXMLUtil::WriteOptionalDS((PPMpeg7Desc*)m_pEdge, writer);
	}

	if(bret)
	{
		bret = BimXMLUtil::WriteOptionalDS((PPMpeg7Desc*)m_pHomogeneousTexture, writer);
	}

	return bret;
}

bool StillRegionFeatureType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pDominantColor);
	SAFE_DELETE(m_pScalableColor);
	SAFE_DELETE(m_pColorStructure);
	SAFE_DELETE(m_pColorLayout);
	SAFE_DELETE(m_pEdge);
	SAFE_DELETE(m_pHomogeneousTexture);
	
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
		if(!bret) break;
		
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pDominantColor = new DominantColorType();
			BREAK_IFNULL(m_pDominantColor);
			bret = m_pDominantColor->ReadBitstream(reader);
			if(!bret) break;
		}
		
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pScalableColor = new ScalableColorType();
			BREAK_IFNULL(m_pScalableColor);
			bret = m_pScalableColor->ReadBitstream(reader);
			if(!bret) break;
		}
		
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pColorStructure = new ColorStructureType();
			BREAK_IFNULL(m_pColorStructure);
			bret = m_pColorStructure->ReadBitstream(reader);
			if(!bret) break;
		}
		
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pColorLayout = new ColorLayoutType();
			BREAK_IFNULL(m_pColorLayout);
			bret = m_pColorLayout->ReadBitstream(reader);
			if(!bret) break;
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pEdge = new EdgeHistogramType();
			BREAK_IFNULL(m_pEdge);
			bret = m_pEdge->ReadBitstream(reader);
			if(!bret) break;
		}
		
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pHomogeneousTexture = new HomogeneousTextureType();
			BREAK_IFNULL(m_pHomogeneousTexture);
			bret = m_pHomogeneousTexture->ReadBitstream(reader);
		}		
	}
	while(false);

	return bret;
}
