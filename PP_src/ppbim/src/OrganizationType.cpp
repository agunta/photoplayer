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

OrganizationType::OrganizationType()
{
	m_pId = NULL;
	m_nNameCount = 0;
	m_ppNames = NULL;
	m_pAddress = NULL;
	m_pElectronicAddress = NULL;
}

OrganizationType::~OrganizationType()
{
	SAFE_DELETE(m_pId);
	SAFE_DELETEPP(m_ppNames, m_nNameCount);
	SAFE_DELETE(m_pAddress);
	SAFE_DELETE(m_pElectronicAddress);
}

bool OrganizationType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	m_nNameCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_NAME);
	if(m_nNameCount > 0)
	{
		m_ppNames = new TextualType*[m_nNameCount];
		RETURN_IFNULL(m_ppNames);
		ZEROP(m_ppNames, m_nNameCount);

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int nameNum = 0;
		for(int i = 0; i < listlen; i++)
		{
			bool bValid = true;
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_NAME) == 0)
			{
				if(nameNum < m_nNameCount)
				{
					TextualType *name = new TextualType();
					if(!name || !name->BuildFromXML(nextNode))
					{
						SAFE_DELETE(name);
						BimXMLUtil::ReportWarning("Invalid <Name> in <OrganizationType>");
						bValid = false;
					}
					m_ppNames[nameNum++] = name;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
			if(!bValid) return false;
		}
	}

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_ADDRESS);
	if(childNode)
	{
		m_pAddress = new PlaceType();
		if(!m_pAddress || !m_pAddress->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pAddress);
			return false;
		}
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_ELECTRONIC_ADDRESS);
	if(childNode)
	{
		m_pElectronicAddress = new ElectronicAddressType();
		if(!m_pElectronicAddress || !m_pElectronicAddress->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pElectronicAddress);
			return false;
		}
	}
	return true;
}

int OrganizationType::GetNumberOfBits()
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
	if(m_pAddress)
	{
		bitCount += m_pAddress->GetNumberOfBits();
	}
	bitCount++;
	if(m_pElectronicAddress)
	{
		bitCount += m_pElectronicAddress->GetNumberOfBits();
	}
	return bitCount;
}

bool OrganizationType::Validate()
{
	return true;
}

bool OrganizationType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bHasOpt = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bHasOpt);
	if(bret && bHasOpt)
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

	if(bret)
	{
		bOptGroup = (m_pAddress == NULL) ? false : true;
		bret = writer->WriteBool(bOptGroup);
		if(bret && bOptGroup)
		{
			bret = m_pAddress->WriteBitstream(writer);
		}
	}

	if(bret)
	{
		bOptGroup = (m_pElectronicAddress == NULL) ? false : true;
		bret = writer->WriteBool(bOptGroup);
		if(bret && bOptGroup)
		{
			bret = m_pElectronicAddress->WriteBitstream(writer);
		}
	}

	return bret;
}

bool OrganizationType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETEPP(m_ppNames, m_nNameCount);
	SAFE_DELETE(m_pAddress);
	SAFE_DELETE(m_pElectronicAddress);
	
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
					TextualType *elem = new TextualType();
					RETURN_IFNULL(elem);
					bret = elem->ReadBitstream(reader);
					if(!bret) return false;
					m_ppNames[i] = elem;
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
			m_pElectronicAddress = new ElectronicAddressType();
			BREAK_IFNULL(m_pElectronicAddress);
			bret = m_pElectronicAddress->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}

void OrganizationType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << "id=\"";
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

	if(m_pAddress)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_ADDRESS, false);
		m_pAddress->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_ADDRESS);
		*fout << std::endl;
	}

	if(m_pElectronicAddress)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_ELECTRONIC_ADDRESS, true);
		*fout << std::endl;
		m_pElectronicAddress->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_ELECTRONIC_ADDRESS);
		*fout << std::endl;
	}

}
