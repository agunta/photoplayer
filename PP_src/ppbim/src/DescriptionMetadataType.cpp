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

DescriptionMetadataType::DescriptionMetadataType()
{
	m_pId = NULL;
	m_pLastUpdate = NULL;

	m_nCreatorCount = 0;
	m_ppCreator = NULL;

	m_pCreationTime = NULL;

}

DescriptionMetadataType::~DescriptionMetadataType()
{
	if(m_pId) delete m_pId;
	if(m_pLastUpdate) delete m_pLastUpdate;
	if(m_nCreatorCount > 0 && m_ppCreator != NULL)
	{
		for(int i = 0; i < m_nCreatorCount; i++)
		{
			if(m_ppCreator[i] != NULL)
			{
				delete m_ppCreator[i];
			}
		}
		delete [] m_ppCreator;
	}
	if(m_pCreationTime) delete m_pCreationTime;
}

bool DescriptionMetadataType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	else
	{
		if(!BimXMLUtil::VerifyName(node, ELEM_NAME_DESCRIPTION_METADATA)) return false;
	}

	m_pId = BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_LAST_UPDATE);
	if(childNode)
	{
		m_pLastUpdate = new TimePointType();
		if(!m_pLastUpdate || !m_pLastUpdate->BuildFromXML(childNode))
		{
			if(m_pLastUpdate)
			{
				delete m_pLastUpdate;
				m_pLastUpdate = NULL;
			}
			return false;
		}
	}
	else
	{
		BimXMLUtil::ReportError("Missing <DescriptionMetadata>/<LastUpdate> element");
		return false;
	}

	m_nCreatorCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_CREATOR);
	if(m_nCreatorCount > 0)
	{
		m_ppCreator = new CreatorType*[m_nCreatorCount];
		int creatorNum = 0;

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		char *nodeName = NULL;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_CREATOR) == 0)
			{
				CreatorType *creator = new CreatorType();
				creator->BuildFromXML(nextNode);
				m_ppCreator[creatorNum++] = creator;
				if(creatorNum == m_nCreatorCount) break;
			}
			if(nodeName) XMLString::release(&nodeName);
		}
		if(nodeName) XMLString::release(&nodeName);
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_CREATION_TIME);
	if(childNode)
	{
		m_pCreationTime = new TimePointType();
		if(!m_pCreationTime || !m_pCreationTime->BuildFromXML(childNode))
		{
			if(m_pCreationTime)
			{
				delete m_pCreationTime;
				m_pCreationTime = NULL;
			}
			return false;
		}
	}
	return true;
}

int DescriptionMetadataType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	if(m_pLastUpdate)
	{
		bitCount += m_pLastUpdate->GetNumberOfBits();
	}
	bitCount++;
	if(m_nCreatorCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nCreatorCount);
		for(int i = 0; i < m_nCreatorCount; i++)
		{
			if(m_ppCreator[i]) bitCount += m_ppCreator[i]->GetNumberOfBits();
		}
	}
	bitCount++;
	if(m_pCreationTime)
	{
		bitCount += m_pCreationTime->GetNumberOfBits();
	}
	return bitCount;
}

bool DescriptionMetadataType::Validate()
{
	bool bret = true; 
	bret = m_pLastUpdate->Validate();
	// to do: validate Creators and CreationTime
	return bret;
}

bool DescriptionMetadataType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret = true;
	RETURN_IFNULL(m_pLastUpdate);
	bool bHasId = (m_pId == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bHasId);
	}
	if(bret && bHasId)
	{
		bret = m_pId->WriteBitstream(writer);
	}
	if(bret)
	{
		bret = m_pLastUpdate->WriteBitstream(writer);
	}
	bool bOptGroup = (m_ppCreator == NULL) ? false : true;
	if(m_nCreatorCount == 0) bOptGroup = false;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nCreatorCount);
		if(bret)
		{
			for(int i = 0; i < m_nCreatorCount; i++)
			{
				RETURN_IFNULL(m_ppCreator[i]);
				bret = m_ppCreator[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	if(bret)
	{
		bOptGroup = (m_pCreationTime == NULL) ? false : true;
		bret = writer->WriteBool(bOptGroup);
		if(bret && bOptGroup)
		{
			bret = m_pCreationTime->WriteBitstream(writer);
		}
	}

	return bret;
}

bool DescriptionMetadataType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pLastUpdate);
	SAFE_DELETEPP(m_ppCreator, m_nCreatorCount);
	SAFE_DELETE(m_pCreationTime);
	
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
		m_pLastUpdate = new TimePointType();
		BREAK_IFNULL(m_pLastUpdate);
		bret = m_pLastUpdate->ReadBitstream(reader);
		if(!bret) break;
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
				m_ppCreator = new CreatorType*[m_nCreatorCount];
				RETURN_IFNULL(m_ppCreator);
				ZEROP(m_ppCreator, m_nCreatorCount);
				for(int i = 0; i < m_nCreatorCount; i++)
				{
					CreatorType *creator = new CreatorType();
					RETURN_IFNULL(creator);
					bret = creator->ReadBitstream(reader);
					if(!bret) return false;
					m_ppCreator[i] = creator;
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
			m_pCreationTime = new TimePointType();
			BREAK_IFNULL(m_pCreationTime);
			bret = m_pCreationTime->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}

void DescriptionMetadataType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << "id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;
	if(m_pLastUpdate)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_LAST_UPDATE, true);
		m_pLastUpdate->PrintXML(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_LAST_UPDATE);
		*fout << std::endl;
	}
	if(m_ppCreator)
	{
		for(int i = 0; i < m_nCreatorCount; i++)
		{
			if(!m_ppCreator[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CREATOR, true);
			*fout << std::endl;
			m_ppCreator[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CREATOR);
			*fout << std::endl;
		}
	}
	if(m_pCreationTime)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CREATION_TIME, true);
		m_pCreationTime->PrintXML(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CREATION_TIME);
		*fout << std::endl;
	}
}
