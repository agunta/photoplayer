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

ColCreationType::ColCreationType()
{
	m_pId = NULL;
	m_nTitleCount = 0;
	m_ppTitles = NULL;
	m_pTitleMedia = NULL;
	m_nCreatorCount = 0;
	m_ppCreators = NULL;
	m_pCreationCoordinates = NULL;

}

ColCreationType::~ColCreationType()
{
	if(m_pId) delete m_pId;
	if(m_ppTitles != NULL)
	{
		for(int i = 0; i < m_nTitleCount; i++)
		{
			if(m_ppTitles[i]) delete m_ppTitles[i];
		}
		delete [] m_ppTitles;
	}
	if(m_pTitleMedia) delete m_pTitleMedia;
	if(m_ppCreators != NULL)
	{
		for(int i = 0; i < m_nCreatorCount; i++)
		{
			if(m_ppCreators[i]) delete m_ppCreators[i];
		}
		delete [] m_ppCreators;
	}
	if(m_pCreationCoordinates) delete m_pCreationCoordinates;
}

bool ColCreationType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	m_pId =  BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_TITLE_MEDIA);
	if(childNode)
	{
		m_pTitleMedia = new TitleMediaType();
		if(!m_pTitleMedia) return false;
		if(!m_pTitleMedia->BuildFromXML(childNode))
		{
			delete m_pTitleMedia;
			m_pTitleMedia = NULL;
		}
	}
	m_nTitleCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_TITLE);
	if(m_nTitleCount < 1)
	{
		BimXMLUtil::ReportError("<Creation> element doesn't contain <Title> element");
		return false;
	}
	m_nCreatorCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_CREATOR);
	if(m_nTitleCount > 0 || m_nCreatorCount > 0)
	{
		if(m_nTitleCount > 0)
		{
			m_ppTitles = new TextualType*[m_nTitleCount];
			if(!m_ppTitles) return false;
			for(int i = 0; i< m_nTitleCount; i++) m_ppTitles[i] = NULL;
		}
		if(m_nCreatorCount > 0)
		{
			m_ppCreators = new CreatorType*[m_nCreatorCount];
			if(!m_ppCreators) return false;
			for(int i = 0; i< m_nCreatorCount; i++) m_ppCreators[i] = NULL;
		}

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int titleNum = 0;
		int creatorNum = 0;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_TITLE) == 0)
			{
				if(titleNum < m_nTitleCount)
				{
					TextualType *title = new TextualType();
					title->BuildFromXML(nextNode);
					m_ppTitles[titleNum++] = title;
				}
			}
			else if( nodeName != NULL && strcmp(nodeName, ELEM_NAME_CREATOR) == 0)
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
		m_pCreationCoordinates = new CreationCoord_anonymusType();
		if(!m_pCreationCoordinates) return false;
		if(!m_pCreationCoordinates->BuildFromXML(childNode))
		{
			delete m_pCreationCoordinates;
			m_pCreationCoordinates = NULL;
		}
	}
	return true;
}

int ColCreationType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	bitCount += BimXMLUtil::GetVLI5BitSize(m_nTitleCount - 1);
	for(int i = 0; i < m_nTitleCount; i++)
	{
		if(m_ppTitles[i]) bitCount += m_ppTitles[i]->GetNumberOfBits();
	}
	bitCount++;
	if(m_pTitleMedia)
	{
		bitCount += m_pTitleMedia->GetNumberOfBits();
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

bool ColCreationType::Validate()
{
	return true;
}

bool ColCreationType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOpt = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pId->WriteBitstream(writer);
	}
	if(bret)
	{
		assert(m_nTitleCount > 0);
		bret = writer->WriteVarLenInt5(m_nTitleCount - 1);
		if(bret)
		{
			for(int i = 0; i < m_nTitleCount; i++)
			{
				RETURN_IFNULL(m_ppTitles[i]);
				bret = m_ppTitles[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	if(bret)
	{
		bOpt = (m_pTitleMedia == NULL) ? false : true;
		bret = writer->WriteBool(bOpt);
		if(bret && bOpt)
		{
			bret = m_pTitleMedia->WriteBitstream(writer);
		}
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

bool ColCreationType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETEPP(m_ppTitles, m_nTitleCount);
	SAFE_DELETE(m_pTitleMedia);
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

		unsigned int val;
		bret = reader->ReadVarLenInt5(&val);
		if(!bret) break;
		m_nTitleCount = (int)val + 1;
		if(m_nTitleCount > 0)
		{
			m_ppTitles = new TextualType*[m_nTitleCount];
			RETURN_IFNULL(m_ppTitles);
			ZEROP(m_ppTitles, m_nTitleCount);
			for(int i = 0; i < m_nTitleCount; i++)
			{
				TextualType *title = new TextualType();
				RETURN_IFNULL(title);
				bret = title->ReadBitstream(reader);
				if(!bret) return false;
				m_ppTitles[i] = title;
			}
		}
		else
		{
			bret = false;
			break;
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pTitleMedia = new TitleMediaType();
			BREAK_IFNULL(m_pTitleMedia);
			bret = m_pTitleMedia->ReadBitstream(reader);
		}

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
			m_pCreationCoordinates = new CreationCoord_anonymusType();
			RETURN_IFNULL(m_pCreationCoordinates);
			bret = m_pCreationCoordinates->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}

void ColCreationType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pId)
	{
		*fout << " id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	if(m_ppTitles)
	{
		for(int i = 0; i < m_nTitleCount; i++)
		{
			if(!m_ppTitles[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_TITLE, false);
			m_ppTitles[i]->PrintXML(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_TITLE);
			*fout << std::endl;
		}
	}

	if(m_pTitleMedia)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_TITLE_MEDIA, true);
		*fout << std::endl;
		m_pTitleMedia->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_TITLE_MEDIA);
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
