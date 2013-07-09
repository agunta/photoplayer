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


ItemCreationType::ItemCreationType()
{
	m_pId = NULL;
	m_pTitle = NULL;
	m_nCreatorCount = 0;
	m_ppCreators = NULL;
	m_pCreationCoordinates = NULL;
}

ItemCreationType::~ItemCreationType()
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pTitle);
	SAFE_DELETEPP(m_ppCreators, m_nCreatorCount);
	SAFE_DELETE(m_pCreationCoordinates);
}

bool ItemCreationType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_TITLE);
	if(childNode)
	{
		m_pTitle = new TextualType();
		if(!m_pTitle || !m_pTitle->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pTitle);
		}
	}
	else
	{
		BimXMLUtil::ReportError("<Creation> element doesn't contain <Title> element");
		return false;
	}

	m_nCreatorCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_CREATOR);
	if(m_nCreatorCount > 0)
	{
		m_ppCreators = new CreatorType*[m_nCreatorCount];
		RETURN_IFNULL(m_ppCreators);
		ZEROP(m_ppCreators, m_nCreatorCount);

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int creatorNum = 0;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if( nodeName != NULL && strcmp(nodeName, ELEM_NAME_CREATOR) == 0)
			{
				if(creatorNum < m_nCreatorCount)
				{
					CreatorType *creator = new CreatorType();
					creator->BuildFromXML(nextNode);
					m_ppCreators[creatorNum++] = creator;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
		}
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_CREATION_COORD);
	if(childNode)
	{
		m_pCreationCoordinates = new ItemCreationCoord_anonymusType();
		RETURN_IFNULL(m_pCreationCoordinates);
		if(!m_pCreationCoordinates || !m_pCreationCoordinates->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pCreationCoordinates);
		}
	}
	return true;
}

void ItemCreationType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pId)
	{
		*fout << " id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	if(m_pTitle)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_TITLE, false);
		m_pTitle->PrintXML(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_TITLE);
		*fout << std::endl;
	}

	if(m_ppCreators)
	{
		for(int i = 0; i < m_nCreatorCount; i++)
		{
			if(!m_ppCreators[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CREATOR, true);
			*fout << std::endl;
			m_ppCreators[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CREATOR);
			*fout << std::endl;
		}
	}

	if(m_pCreationCoordinates)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CREATION_COORD, true);
		*fout << std::endl;
		m_pCreationCoordinates->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CREATION_COORD);
		*fout << std::endl;
	}
}

int ItemCreationType::GetNumberOfBits()
{
	int bitCount = 0;

	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}

	if(m_pTitle)
	{
		bitCount += m_pTitle->GetNumberOfBits();
	}

	bitCount++;
	if(m_nCreatorCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nCreatorCount);
		for(int i = 0; i < m_nCreatorCount; i++)
		{
			if(m_ppCreators[i]) bitCount += m_ppCreators[i]->GetNumberOfBits();
		}
	}

	bitCount++;
	if(m_pCreationCoordinates)
	{
		bitCount += m_pCreationCoordinates->GetNumberOfBits();
	}

	return bitCount;
}

bool ItemCreationType::Validate()
{
	return true;
}

bool ItemCreationType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pTitle);

	bool bOpt = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pId->WriteBitstream(writer);
	}

	if(bret)
	{
		bret = m_pTitle->WriteBitstream(writer);
	}

	bOpt = (m_ppCreators == NULL) ? false : true;
	if(m_nCreatorCount == 0) bOpt = false;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = writer->WriteVarLenInt5(m_nCreatorCount);
		if(bret)
		{
			for(int i = 0; i < m_nCreatorCount; i++)
			{
				RETURN_IFNULL(m_ppCreators[i]);
				bret = m_ppCreators[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}

	if(bret)
	{
		bOpt = (m_pCreationCoordinates == NULL) ? false : true;
		bret = writer->WriteBool(bOpt);
		if(bret && bOpt)
		{
			bret = m_pCreationCoordinates->WriteBitstream(writer);
		}
	}

	return bret;
}

bool ItemCreationType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pTitle);
	SAFE_DELETEPP(m_ppCreators, m_nCreatorCount);
	SAFE_DELETE(m_pCreationCoordinates);
	
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

		m_pTitle = new TextualType();
		RETURN_IFNULL(m_pTitle);
		bret = m_pTitle->ReadBitstream(reader);
		if(!bret) return false;

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nCreatorCount = (int)val;
			if(m_nCreatorCount > 0)
			{
				m_ppCreators = new CreatorType*[m_nCreatorCount];
				RETURN_IFNULL(m_ppCreators);
				ZEROP(m_ppCreators, m_nCreatorCount);
				for(int i = 0; i < m_nCreatorCount; i++)
				{
					CreatorType *creator = new CreatorType();
					RETURN_IFNULL(creator);
					bret = creator->ReadBitstream(reader);
					if(!bret) return false;
					m_ppCreators[i] = creator;
				}
			}
			else
			{
				bret = false;
				break;
			}
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pCreationCoordinates = new ItemCreationCoord_anonymusType();
			RETURN_IFNULL(m_pCreationCoordinates);
			bret = m_pCreationCoordinates->ReadBitstream(reader);
		}

	}
	while(false);

	return bret;
}
