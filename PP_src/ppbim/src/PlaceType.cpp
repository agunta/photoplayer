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

PlaceType::PlaceType()
{
	m_pId = NULL;
	m_nNameCount = 0;
	m_nDescriptionCount = 0;
	m_ppNames = NULL;
	m_ppDescriptions = NULL;
	m_pGeographicPosition = NULL;
	m_pPostalAddress = NULL;
}

PlaceType::~PlaceType()
{
	if(m_pId) delete m_pId;
	if(m_ppNames)
	{
		for(int i = 0; i< m_nNameCount; i++)
		{
			if(m_ppNames[i]) delete m_ppNames[i];
		}
		delete [] m_ppNames;
	}
	if(m_ppDescriptions)
	{
		for(int i = 0; i< m_nDescriptionCount; i++)
		{
			if(m_ppDescriptions[i]) delete m_ppDescriptions[i];
		}
		delete [] m_ppDescriptions;
	}
	if(m_pGeographicPosition) delete m_pGeographicPosition;
	if(m_pPostalAddress) delete m_pPostalAddress;
}

bool PlaceType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	m_nNameCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_NAME);
	m_nDescriptionCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_PLACE_DESCRIPTION);
	if(m_nNameCount > 0 || m_nDescriptionCount > 0)
	{
		int i;
		if(m_nNameCount > 0)
		{
			m_ppNames = new TextualType*[m_nNameCount];
			if(!m_ppNames) return false;
			for(i = 0; i< m_nNameCount; i++) m_ppNames[i] = NULL;
		}
		if(m_nDescriptionCount > 0)
		{
			m_ppDescriptions = new TextualType*[m_nDescriptionCount];
			if(!m_ppDescriptions) return false;
			for(i = 0; i< m_nDescriptionCount; i++) m_ppDescriptions[i] = NULL;
		}
		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int nameNum = 0;
		int descriptionNum = 0;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_NAME) == 0)
			{
				if(nameNum < m_nNameCount)
				{
					TextualType *name = new TextualType();
					name->BuildFromXML(nextNode);
					m_ppNames[nameNum++] = name;
				}
			}
			else if( nodeName != NULL && strcmp(nodeName, ELEM_NAME_PLACE_DESCRIPTION) == 0)
			{
				if(descriptionNum < m_nDescriptionCount)
				{
					TextualType *description = new TextualType();
					description->BuildFromXML(nextNode);
					m_ppDescriptions[descriptionNum++] = description;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
		}
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_GEOGRAPHIC_POSITION);
	if(childNode)
	{
		m_pGeographicPosition = new GeographicPosition_anonymusType();
		if(!m_pGeographicPosition) return false;
		if(!m_pGeographicPosition->BuildFromXML(childNode))
		{
			delete m_pGeographicPosition;
			m_pGeographicPosition = NULL;
		}
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_POSTAL_ADDRESS);
	if(childNode)
	{
		m_pPostalAddress = new PostalAddress_anonymousType();
		if(!m_pPostalAddress) return false;
		if(!m_pPostalAddress->BuildFromXML(childNode))
		{
			delete m_pPostalAddress;
			m_pPostalAddress = NULL;
		}
	}
	return true;
}

int PlaceType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	bitCount++;
	if(m_nNameCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nNameCount);
		for(int i = 0; i < m_nNameCount; i++)
		{
			if(m_ppNames[i]) bitCount += m_ppNames[i]->GetNumberOfBits();
		}
	}
	bitCount++;
	if(m_nDescriptionCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nDescriptionCount);
		for(int i = 0; i < m_nDescriptionCount; i++)
		{
			if(m_ppDescriptions[i]) bitCount += m_ppDescriptions[i]->GetNumberOfBits();
		}
	}
	bitCount++;
	if(m_pGeographicPosition)
	{
		bitCount += m_pGeographicPosition->GetNumberOfBits();
	}
	bitCount++;
	if(m_pPostalAddress)
	{
		bitCount += m_pPostalAddress->GetNumberOfBits();
	}
	return bitCount;
}

bool PlaceType::Validate()
{
	return true;
}

bool PlaceType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bHasId = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bHasId);
	if(bret && bHasId)
	{
		bret = m_pId->WriteBitstream(writer);
	}

	bool bOptGroup = (m_ppNames == NULL) ? false : true;
	if(m_nNameCount == 0) bOptGroup = false;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nNameCount);
		if(bret)
		{
			for(int i = 0; i < m_nNameCount; i++)
			{
				RETURN_IFNULL(m_ppNames[i]);
				bret = m_ppNames[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}

	bOptGroup = (m_ppDescriptions == NULL) ? false : true;
	if(m_nDescriptionCount == 0) bOptGroup = false;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nDescriptionCount);
		if(bret)
		{
			for(int i = 0; i < m_nDescriptionCount; i++)
			{
				RETURN_IFNULL(m_ppDescriptions[i]);
				bret = m_ppDescriptions[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}

	bOptGroup = (m_pGeographicPosition == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = m_pGeographicPosition->WriteBitstream(writer);
	}

	bOptGroup = (m_pPostalAddress == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = m_pPostalAddress->WriteBitstream(writer);
	}

	return bret;
}

bool PlaceType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETEPP(m_ppNames, m_nNameCount);
	SAFE_DELETEPP(m_ppDescriptions, m_nDescriptionCount);
	SAFE_DELETE(m_pGeographicPosition);
	SAFE_DELETE(m_pPostalAddress);
	
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

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nNameCount = (int)val;
			if(m_nNameCount > 0)
			{
				m_ppNames = new TextualType*[m_nNameCount];
				RETURN_IFNULL(m_ppNames);
				ZEROP(m_ppNames, m_nNameCount);
				for(int i = 0; i < m_nNameCount; i++)
				{
					TextualType *name = new TextualType();
					RETURN_IFNULL(name);
					bret = name->ReadBitstream(reader);
					if(!bret) return false;
					m_ppNames[i] = name;
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
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nDescriptionCount = (int)val;
			if(m_nDescriptionCount > 0)
			{
				m_ppDescriptions = new TextualType*[m_nDescriptionCount];
				RETURN_IFNULL(m_ppDescriptions);
				ZEROP(m_ppDescriptions, m_nDescriptionCount);
				for(int i = 0; i < m_nDescriptionCount; i++)
				{
					TextualType *desc = new TextualType();
					RETURN_IFNULL(desc);
					bret = desc->ReadBitstream(reader);
					if(!bret) return false;
					m_ppDescriptions[i] = desc;
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
			m_pGeographicPosition = new GeographicPosition_anonymusType();
			BREAK_IFNULL(m_pGeographicPosition);
			bret = m_pGeographicPosition->ReadBitstream(reader);
			if(!bret) break;
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pPostalAddress = new PostalAddress_anonymousType();
			BREAK_IFNULL(m_pPostalAddress);
			bret = m_pPostalAddress->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}

void PlaceType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pId)
	{
		*fout << " id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	if(m_ppNames)
	{
		for(int i = 0; i < m_nNameCount; i++)
		{
			if(!m_ppNames[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_NAME, false);
			m_ppNames[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_NAME);
			*fout << std::endl;
		}
	}

	if(m_ppDescriptions)
	{
		for(int i = 0; i < m_nDescriptionCount; i++)
		{
			if(!m_ppDescriptions[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_PLACE_DESCRIPTION, false);
			m_ppDescriptions[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_PLACE_DESCRIPTION);
			*fout << std::endl;
		}
	}

	if(m_pGeographicPosition)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_GEOGRAPHIC_POSITION, false);
		m_pGeographicPosition->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_GEOGRAPHIC_POSITION);
		*fout << std::endl;
	}

	if(m_pPostalAddress)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_POSTAL_ADDRESS, false);
		m_pPostalAddress->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_POSTAL_ADDRESS);
		*fout << std::endl;
	}
}
