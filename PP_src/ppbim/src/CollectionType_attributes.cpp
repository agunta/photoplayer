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

CollectionType_attributes::CollectionType_attributes()
{
	m_pId = NULL;
	m_pName = NULL;
}

CollectionType_attributes::~CollectionType_attributes()
{
	if(m_pId) delete m_pId;
	if(m_pName) delete m_pName;
}

bool CollectionType_attributes::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId = BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	m_pName = BimXMLUtil::GetAttributeString(node, ATTR_NAME_NAME);

	return true;
}

int CollectionType_attributes::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	bitCount++;
	if(m_pName)
	{
		bitCount += m_pName->GetNumberOfBits();
	}
	return bitCount;
}

bool CollectionType_attributes::Validate()
{
	return true;
}

bool CollectionType_attributes::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bHasId = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bHasId);
	if(bret && bHasId)
	{
		bret = m_pId->WriteBitstream(writer);
	}
	if(bret)
	{
		bool bHasName = (m_pName == NULL) ? false : true;
		bret = writer->WriteBool(bHasName);
		if(bret && bHasName)
		{
			bret = m_pName->WriteBitstream(writer);
		}
	}
	return bret;
}

bool CollectionType_attributes::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pName);
	
	bool bret;
	do {
		bool bHasOpt;
		bret = reader->ReadBool(&bHasOpt);
		if(!bret) break;
		if(bHasOpt)
		{
			m_pId = new DescString();
			RETURN_IFNULL(m_pId);
			bret = m_pId->ReadBitstream(reader);
			if(!bret) break;
		}
		bret = reader->ReadBool(&bHasOpt);
		if(!bret) break;
		if(bHasOpt)
		{
			m_pName = new DescString();
			RETURN_IFNULL(m_pName);
			bret = m_pName->ReadBitstream(reader);
		}
	}
	while(false);
	
	return bret;
}

void CollectionType_attributes::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << " id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	if(m_pName && m_pName->GetData() != NULL)
	{
		*fout << " name=\"";
		m_pName->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
}
