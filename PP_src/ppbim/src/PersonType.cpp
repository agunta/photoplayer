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

PersonType::PersonType()
{
	m_pId = NULL;
	m_pName = NULL;
	m_nAffiliationCount = 0;
	m_nElectronicAddressCount = 0;
	m_ppAffiliation = NULL;
	m_pAddress = NULL;
	m_ppElectronicAddress = NULL;
}

PersonType::~PersonType()
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pName);
	SAFE_DELETEPP(m_ppAffiliation, m_nAffiliationCount);
	SAFE_DELETE(m_pAddress);
	SAFE_DELETEPP(m_ppElectronicAddress, m_nElectronicAddressCount);
}

bool PersonType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_NAME);
	if(childNode)
	{
		m_pName = new PersonNameType();
		if(!m_pName || !m_pName->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pName);
			return false;
		}
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_ADDRESS);
	if(childNode)
	{
		m_pAddress = new PlaceType();
		if(!m_pAddress || !m_pAddress->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pAddress);
			return false;
		}
	}
	m_nAffiliationCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_AFFILIATION);
	m_nElectronicAddressCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_ELECTRONIC_ADDRESS);
	int totalCount = m_nAffiliationCount + m_nElectronicAddressCount;
	if(totalCount > 0)
	{
		if(m_nAffiliationCount > 0)
		{
			m_ppAffiliation = new Affiliation_anonymousType*[m_nAffiliationCount];
			if(!m_ppAffiliation) return false;
			ZEROP(m_ppAffiliation, m_nAffiliationCount);
		}
		if(m_nElectronicAddressCount > 0)
		{
			m_ppElectronicAddress = new ElectronicAddressType*[m_nElectronicAddressCount];
			if(!m_ppElectronicAddress) return false;
			ZEROP(m_ppElectronicAddress, m_nElectronicAddressCount);
		}
		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int affiliationNum = 0;
		int eaddressNum = 0;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(!nodeName) continue;
			if(strcmp(nodeName, ELEM_NAME_AFFILIATION) == 0)
			{
				if(affiliationNum < m_nAffiliationCount)
				{
					Affiliation_anonymousType *elem = new Affiliation_anonymousType();
					elem->BuildFromXML(nextNode);
					m_ppAffiliation[affiliationNum++] = elem;
				}
			}
			else if(strcmp(nodeName, ELEM_NAME_ELECTRONIC_ADDRESS) == 0)
			{
				if(eaddressNum < m_nElectronicAddressCount)
				{
					ElectronicAddressType *elem = new ElectronicAddressType();
					elem->BuildFromXML(nextNode);
					m_ppElectronicAddress[eaddressNum++] = elem;
				}
			}
			XMLString::release(&nodeName);
		}
	}
	return true;
}

int PersonType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	if(m_pName)
	{
		bitCount += m_pName->GetNumberOfBits();
	}
	bitCount++;
	if(m_nAffiliationCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nAffiliationCount);
		for(int i = 0; i < m_nAffiliationCount; i++)
		{
			if(m_ppAffiliation[i]) bitCount += m_ppAffiliation[i]->GetNumberOfBits();
		}
	}
	bitCount++;
	if(m_pAddress)
	{
		bitCount += m_pAddress->GetNumberOfBits();
	}
	bitCount++;
	if(m_nElectronicAddressCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nElectronicAddressCount);
		for(int i = 0; i < m_nElectronicAddressCount; i++)
		{
			if(m_ppElectronicAddress[i]) bitCount += m_ppElectronicAddress[i]->GetNumberOfBits();
		}
	}

	return bitCount;
}

bool PersonType::Validate()
{
	if(m_pName == NULL) return false;
	return true;
}

bool PersonType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pName);
	bool bHasId = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bHasId);
	if(bret && bHasId)
	{
		bret = m_pId->WriteBitstream(writer);
	}
	if(bret)
	{
		bret = m_pName->WriteBitstream(writer);
	}
	bool bOptGroup = (m_ppAffiliation == NULL) ? false : true;
	if(m_nAffiliationCount == 0) bOptGroup = false;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nAffiliationCount);
		if(bret)
		{
			for(int i = 0; i < m_nAffiliationCount; i++)
			{
				RETURN_IFNULL(m_ppAffiliation[i]);
				bret = m_ppAffiliation[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	if(bret)
	{
		bOptGroup = (m_pAddress == NULL) ? false : true;
		bret = writer->WriteBool(bOptGroup);
		if(bret && bOptGroup)
		{
			bret = m_pAddress->WriteBitstream(writer);
		}
	}
	bOptGroup = (m_ppElectronicAddress == NULL) ? false : true;
	if(m_nElectronicAddressCount == 0) bOptGroup = false;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nElectronicAddressCount);
		if(bret)
		{
			for(int i = 0; i < m_nElectronicAddressCount; i++)
			{
				RETURN_IFNULL(m_ppElectronicAddress[i]);
				bret = m_ppElectronicAddress[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	return bret;
}

bool PersonType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pName);
	SAFE_DELETEPP(m_ppAffiliation, m_nAffiliationCount);
	SAFE_DELETE(m_pAddress);
	SAFE_DELETEPP(m_ppElectronicAddress, m_nElectronicAddressCount);
	
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
		m_pName = new PersonNameType();
		BREAK_IFNULL(m_pName);
		bret = m_pName->ReadBitstream(reader);
		if(!bret) break;

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nAffiliationCount = (int)val;
			if(m_nAffiliationCount > 0)
			{
				m_ppAffiliation = new Affiliation_anonymousType*[m_nAffiliationCount];
				RETURN_IFNULL(m_ppAffiliation);
				ZEROP(m_ppAffiliation, m_nAffiliationCount);
				for(int i = 0; i < m_nAffiliationCount; i++)
				{
					Affiliation_anonymousType *affiliation = new Affiliation_anonymousType();
					RETURN_IFNULL(affiliation);
					bret = affiliation->ReadBitstream(reader);
					if(!bret) return false;
					m_ppAffiliation[i] = affiliation;
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
			m_pAddress = new PlaceType();
			BREAK_IFNULL(m_pAddress);
			bret = m_pAddress->ReadBitstream(reader);
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nElectronicAddressCount = (int)val;
			if(m_nElectronicAddressCount > 0)
			{
				m_ppElectronicAddress = new ElectronicAddressType*[m_nElectronicAddressCount];
				RETURN_IFNULL(m_ppElectronicAddress);
				ZEROP(m_ppElectronicAddress, m_nElectronicAddressCount);
				for(int i = 0; i < m_nElectronicAddressCount; i++)
				{
					ElectronicAddressType *eaddress = new ElectronicAddressType();
					RETURN_IFNULL(eaddress);
					bret = eaddress->ReadBitstream(reader);
					if(!bret) return false;
					m_ppElectronicAddress[i] = eaddress;
				}
			}
			else
			{
				bret = false;
				break;
			}
		}
	}
	while(false);

	return bret;
}

void PersonType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << "id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;
	
	if(m_pName)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_NAME, false);
		m_pName->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_NAME);
		*fout << std::endl;
	}

	if(m_ppAffiliation)
	{
		for(int i = 0; i < m_nAffiliationCount; i++)
		{
			if(!m_ppAffiliation[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_AFFILIATION, true);
			*fout << std::endl;
			m_ppAffiliation[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_AFFILIATION);
			*fout << std::endl;
		}
	}

	if(m_pAddress)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_ADDRESS, false);
		m_pAddress->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_ADDRESS);
		*fout << std::endl;
	}

	if(m_ppElectronicAddress)
	{
		for(int i = 0; i < m_nElectronicAddressCount; i++)
		{
			if(!m_ppElectronicAddress[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_ELECTRONIC_ADDRESS, true);
			*fout << std::endl;
			m_ppElectronicAddress[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_ELECTRONIC_ADDRESS);
			*fout << std::endl;
		}
	}
}
