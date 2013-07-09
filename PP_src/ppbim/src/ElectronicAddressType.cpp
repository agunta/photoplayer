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


ElectronicAddressType::ElectronicAddressType()
{
	m_nTelephoneCount = 0;
	m_nFaxCount = 0;
	m_nEmailCount = 0;
	m_nUrlCount = 0;

	m_ppTelephone = NULL;
	m_ppFax = NULL;
	m_ppEmail = NULL;
	m_ppUrl = NULL;

}

ElectronicAddressType::~ElectronicAddressType()
{
	SAFE_DELETEPP(m_ppTelephone, m_nTelephoneCount);
	SAFE_DELETEPP(m_ppFax, m_nFaxCount);
	SAFE_DELETEPP(m_ppEmail, m_nEmailCount);
	SAFE_DELETEPP(m_ppUrl, m_nUrlCount);
}

bool ElectronicAddressType::BuildFromXML(DOMNode *node)
{
	m_nTelephoneCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_TELEPHONE);
	m_nFaxCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_FAX);
	m_nEmailCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_EMAIL);
	m_nUrlCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_URL);
	int totalCount = m_nTelephoneCount + m_nFaxCount + m_nEmailCount + m_nUrlCount;
	if(totalCount > 0)
	{
		if(m_nTelephoneCount > 0)
		{
			m_ppTelephone = new DescString*[m_nTelephoneCount];
			if(!m_ppTelephone) return false;
			ZEROP(m_ppTelephone, m_nTelephoneCount);
		}
		if(m_nFaxCount > 0)
		{
			m_ppFax = new DescString_typecast*[m_nFaxCount];
			if(!m_ppFax) return false;
			ZEROP(m_ppFax, m_nFaxCount);
		}
		if(m_nEmailCount > 0)
		{
			m_ppEmail = new DescString_typecast*[m_nEmailCount];
			if(!m_ppEmail) return false;
			ZEROP(m_ppEmail, m_nEmailCount);
		}
		if(m_nUrlCount > 0)
		{
			m_ppUrl = new DescAnyURI_typecast*[m_nUrlCount];
			if(!m_ppUrl) return false;
			ZEROP(m_ppUrl, m_nUrlCount);
		}

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int teleNum = 0;
		int faxNum = 0;
		int emailNum = 0;
		int urlNum = 0;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(!nodeName) continue;
			if(strcmp(nodeName, ELEM_NAME_TELEPHONE) == 0)
			{
				if(teleNum < m_nTelephoneCount)
				{
					DescString *elem = new DescString();
					elem->BuildFromXML(nextNode);
					m_ppTelephone[teleNum++] = elem;
				}
			}
			else if(strcmp(nodeName, ELEM_NAME_FAX) == 0)
			{
				if(faxNum < m_nFaxCount)
				{
					DescString_typecast *elem = new DescString_typecast();
					elem->BuildFromXML(nextNode);
					m_ppFax[faxNum++] = elem;
				}
			}
			else if(strcmp(nodeName, ELEM_NAME_EMAIL) == 0)
			{
				if(emailNum < m_nEmailCount)
				{
					DescString_typecast *elem = new DescString_typecast();
					elem->BuildFromXML(nextNode);
					m_ppEmail[emailNum++] = elem;
				}
			}
			else if(strcmp(nodeName, ELEM_NAME_URL) == 0)
			{
				if(urlNum < m_nUrlCount)
				{
					DescAnyURI_typecast *elem = new DescAnyURI_typecast();
					elem->BuildFromXML(nextNode);
					m_ppUrl[urlNum++] = elem;
				}
			}
			XMLString::release(&nodeName);
		}
	}

	return true;
}

int ElectronicAddressType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_ppTelephone != NULL && m_nTelephoneCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nTelephoneCount);
		for(int i = 0; i < m_nTelephoneCount; i++)
		{
			if(m_ppTelephone[i]) bitCount += m_ppTelephone[i]->GetNumberOfBits();
		}
	}
	bitCount++;
	if(m_ppFax != NULL && m_nFaxCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nFaxCount);
		for(int i = 0; i < m_nFaxCount; i++)
		{
			if(m_ppFax[i]) bitCount += m_ppFax[i]->GetNumberOfBits();
		}
	}
	bitCount++;
	if(m_ppEmail != NULL && m_nEmailCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nEmailCount);
		for(int i = 0; i < m_nEmailCount; i++)
		{
			if(m_ppEmail[i]) bitCount += m_ppEmail[i]->GetNumberOfBits();
		}
	}
	bitCount++;
	if(m_ppUrl != NULL && m_nUrlCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nUrlCount);
		for(int i = 0; i < m_nUrlCount; i++)
		{
			// Fixed: bitCount++;    // TypeCast in BiM

			if(m_ppUrl[i]) bitCount += m_ppUrl[i]->GetNumberOfBits();
		}
	}
	return bitCount;
}

bool ElectronicAddressType::Validate()
{
	return true;
}

bool ElectronicAddressType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOptGroup = (m_ppTelephone == NULL) ? false : true;
	if(m_nTelephoneCount == 0) bOptGroup = false;
	bret = writer->WriteBool(bOptGroup);
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nTelephoneCount);
		if(bret)
		{
			for(int i = 0; i < m_nTelephoneCount; i++)
			{
				RETURN_IFNULL(m_ppTelephone[i]);
				bret = m_ppTelephone[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}

	bOptGroup = (m_ppFax == NULL) ? false : true;
	if(m_nFaxCount == 0) bOptGroup = false;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nFaxCount);
		if(bret)
		{
			for(int i = 0; i < m_nFaxCount; i++)
			{
				RETURN_IFNULL(m_ppFax[i]);
				bret = m_ppFax[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}

	bOptGroup = (m_ppEmail == NULL) ? false : true;
	if(m_nEmailCount == 0) bOptGroup = false;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nEmailCount);
		if(bret)
		{
			for(int i = 0; i < m_nEmailCount; i++)
			{
				RETURN_IFNULL(m_ppEmail[i]);
				bret = m_ppEmail[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}

	bOptGroup = (m_ppUrl == NULL) ? false : true;
	if(m_nUrlCount == 0) bOptGroup = false;
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(m_nUrlCount);
		if(bret)
		{
			for(int i = 0; i < m_nUrlCount; i++)
			{
				RETURN_IFNULL(m_ppUrl[i]);

				// FIXED: bret = writer->WriteBits(0, 1);   // TypeCast in BiM

				bret = m_ppUrl[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	return bret;
}

bool ElectronicAddressType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETEPP(m_ppTelephone, m_nTelephoneCount);
	SAFE_DELETEPP(m_ppFax, m_nFaxCount);
	SAFE_DELETEPP(m_ppEmail, m_nEmailCount);
	SAFE_DELETEPP(m_ppUrl, m_nUrlCount);
	
	bool bret;

	do {
		bool bOpt;

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nTelephoneCount = (int)val;
			if(m_nTelephoneCount > 0)
			{
				m_ppTelephone = new DescString*[m_nTelephoneCount];
				RETURN_IFNULL(m_ppTelephone);
				ZEROP(m_ppTelephone, m_nTelephoneCount);
				for(int i = 0; i < m_nTelephoneCount; i++)
				{
					DescString *elem = new DescString();
					RETURN_IFNULL(elem);
					bret = elem->ReadBitstream(reader);
					if(!bret) return false;
					m_ppTelephone[i] = elem;
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
			m_nFaxCount = (int)val;
			if(m_nFaxCount > 0)
			{
				m_ppFax = new DescString_typecast*[m_nFaxCount];
				RETURN_IFNULL(m_ppFax);
				ZEROP(m_ppFax, m_nFaxCount);
				for(int i = 0; i < m_nFaxCount; i++)
				{
					DescString_typecast *elem = new DescString_typecast();
					RETURN_IFNULL(elem);
					bret = elem->ReadBitstream(reader);
					if(!bret) return false;
					m_ppFax[i] = elem;
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
			m_nEmailCount = (int)val;
			if(m_nEmailCount > 0)
			{
				m_ppEmail = new DescString_typecast*[m_nEmailCount];
				RETURN_IFNULL(m_ppEmail);
				ZEROP(m_ppEmail, m_nEmailCount);
				for(int i = 0; i < m_nEmailCount; i++)
				{
					DescString_typecast *elem = new DescString_typecast();
					RETURN_IFNULL(elem);
					bret = elem->ReadBitstream(reader);
					if(!bret) return false;
					m_ppEmail[i] = elem;
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
			m_nUrlCount = (int)val;
			if(m_nUrlCount > 0)
			{
				m_ppUrl = new DescAnyURI_typecast*[m_nUrlCount];
				RETURN_IFNULL(m_ppUrl);
				ZEROP(m_ppUrl, m_nUrlCount);
				for(int i = 0; i < m_nUrlCount; i++)
				{
					DescAnyURI_typecast *elem = new DescAnyURI_typecast();
					RETURN_IFNULL(elem);

					// FIXED: int val;
					// bret = reader->ReadBits(&val, 1);  //TypeCast in BiM
					if(!bret) return false;

					bret = elem->ReadBitstream(reader);
					if(!bret) return false;
					m_ppUrl[i] = elem;
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

void ElectronicAddressType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	
	if(m_ppTelephone)
	{
		for(int i = 0; i < m_nTelephoneCount; i++)
		{
			if(!m_ppTelephone[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_TELEPHONE, true);
			m_ppTelephone[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_TELEPHONE);
			*fout << std::endl;
		}
	}

	if(m_ppFax)
	{
		for(int i = 0; i < m_nFaxCount; i++)
		{
			if(!m_ppFax[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_FAX, false);
			m_ppFax[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_FAX);
			*fout << std::endl;
		}
	}

	if(m_ppEmail)
	{
		for(int i = 0; i < m_nEmailCount; i++)
		{
			if(!m_ppEmail[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_EMAIL, false);
			m_ppEmail[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_EMAIL);
			*fout << std::endl;
		}
	}

	if(m_ppUrl)
	{
		for(int i = 0; i < m_nUrlCount; i++)
		{
			if(!m_ppUrl[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_URL, false);
			m_ppUrl[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_URL);
			*fout << std::endl;
		}
	}
}
