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


ControlledTermUseType::ControlledTermUseType()
{
	m_pHref = NULL;
	m_pTermDefinition = NULL;
}

ControlledTermUseType::~ControlledTermUseType()
{
	if(m_pHref) delete m_pHref;
	if(m_pTermDefinition) delete m_pTermDefinition;
}

bool ControlledTermUseType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *hrefNode = BimXMLUtil::GetAttributeNode(node, ATTR_NAME_HREF);
	if(hrefNode)
	{
		m_pHref = new TermReferenceType();
		if(!m_pHref) return false;
		if(!m_pHref->BuildFromXML(hrefNode))
		{
			delete m_pHref;
			m_pHref = NULL;
			return false;
		}
	}
	else
	{
		return false;
	}
	m_pTermDefinition = new InlineTermDefinitionType();
	if(!m_pTermDefinition || !m_pTermDefinition->BuildFromXML(node)) 
	{
		SAFE_DELETE(m_pTermDefinition);
		return false;
	}

	return true;
}

int ControlledTermUseType::GetNumberOfBits()
{
	int bitCount = 0;
	if(m_pHref)
	{
		bitCount += m_pHref->GetNumberOfBits();
	}
	if(m_pTermDefinition)
	{
		bitCount += m_pTermDefinition->GetNumberOfBits();
	}
	return bitCount;
}

bool ControlledTermUseType::Validate()
{
	return true;
}

bool ControlledTermUseType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pHref);
	RETURN_IFNULL(m_pTermDefinition);
	bret = m_pHref->WriteBitstream(writer);
	if(bret)
	{
		bret = m_pTermDefinition->WriteBitstream(writer);
	}
	return bret;
}

bool ControlledTermUseType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pHref);
	SAFE_DELETE(m_pTermDefinition);
	
	bool bret;

	do {
		m_pHref = new TermReferenceType();
		RETURN_IFNULL(m_pHref);
		bret = m_pHref->ReadBitstream(reader);
		if(!bret) break;

		m_pTermDefinition = new InlineTermDefinitionType();
		RETURN_IFNULL(m_pTermDefinition);
		bret = m_pTermDefinition->ReadBitstream(reader);
	}
	while(false);

	return bret;
}

void  ControlledTermUseType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pHref)
	{
		*fout << " href=\"";
		m_pHref->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;
	if(m_pTermDefinition)
	{
		m_pTermDefinition->PrintXML(fout, nIndentLevel + 1);
	}
}
