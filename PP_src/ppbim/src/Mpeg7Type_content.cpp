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

Mpeg7Type_content::Mpeg7Type_content()
{
	m_pDescriptionMetadata = NULL;
	m_pDescriptionProfile = NULL;
}

Mpeg7Type_content::~Mpeg7Type_content()
{
	if(m_pDescriptionMetadata) delete m_pDescriptionMetadata;
	if(m_pDescriptionProfile) delete m_pDescriptionProfile;
}

bool Mpeg7Type_content::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_DESCRIPTION_PROFILE);
	if(childNode)
	{
		m_pDescriptionProfile = new DescriptionProfileType();
		if(!m_pDescriptionProfile || !m_pDescriptionProfile->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pDescriptionProfile);
		}
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_DESCRIPTION_METADATA);
	if(childNode)
	{
		m_pDescriptionMetadata = new DescriptionMetadataType();
		if(!m_pDescriptionMetadata || !m_pDescriptionMetadata->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pDescriptionMetadata);
		}
	}
	if(!m_pDescriptionMetadata)
	{
		BimXMLUtil::ReportError("Missing <DescriptionMetadata> element");
		return false;
	}
	return true;
}

int Mpeg7Type_content::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount +=1;
	if(m_pDescriptionProfile)
	{
		bitCount += m_pDescriptionProfile->GetNumberOfBits();
	}
	if(m_pDescriptionMetadata)
	{
		bitCount += m_pDescriptionMetadata->GetNumberOfBits();
	}

	return bitCount;
}

bool Mpeg7Type_content::Validate()
{
	return true;
}

bool Mpeg7Type_content::WriteBitstream(BitstreamWriter *writer)
{
	bool bret = true;
	if(!m_pDescriptionMetadata) bret = false;
	bool bOptProfile = (m_pDescriptionProfile == NULL) ? false : true;
	bret = writer->WriteBool(bOptProfile);
	if(bret && bOptProfile)
	{
		bret = m_pDescriptionProfile->WriteBitstream(writer);
	}
	if(bret && m_pDescriptionMetadata)
	{
		bret = m_pDescriptionMetadata->WriteBitstream(writer);
	}
	return true;
}

bool Mpeg7Type_content::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pDescriptionProfile);
	SAFE_DELETE(m_pDescriptionMetadata);
	
	bool bret = true;

	do {
		bool bOptProfile;
		bret = reader->ReadBool(&bOptProfile);
		if(!bret) break;
		if(bOptProfile)
		{
			m_pDescriptionProfile = new DescriptionProfileType();
			BREAK_IFNULL(m_pDescriptionProfile);
			bret = m_pDescriptionProfile->ReadBitstream(reader);
			if(!bret) break;
		}

		m_pDescriptionMetadata = new DescriptionMetadataType();
		BREAK_IFNULL(m_pDescriptionMetadata);
		bret = m_pDescriptionMetadata->ReadBitstream(reader);
	}
	while(false);

	return bret;
}

void  Mpeg7Type_content::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	if(m_pDescriptionProfile)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_DESCRIPTION_PROFILE, false);
		m_pDescriptionProfile->PrintXML(fout, nIndentLevel);
		*fout << "/>" << std::endl;
	}
	if(m_pDescriptionMetadata)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_DESCRIPTION_METADATA, false);

		m_pDescriptionMetadata->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_DESCRIPTION_METADATA);
		*fout << std::endl;
	}
}
