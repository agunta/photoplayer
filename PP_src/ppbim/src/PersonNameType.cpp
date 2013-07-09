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
#include <vector>

PersonNameType::PersonNameType()
{
	m_pLang = NULL;
	m_pTypeTab = NULL;
	m_nFamilyNameCount = 0;
	m_nGivenNameCount = 0;
	m_nLinkingNameCount = 0;
	m_nNumerationCount = 0;
	m_nSalutationCount = 0;
	m_nTitleCount = 0;
	m_ppFamilyName = NULL;
	m_ppGivenName = NULL;
	m_ppLinkingName = NULL;
	m_ppNumeration = NULL;
	m_ppSalutation = NULL;
	m_ppTitle = NULL;
}

PersonNameType::~PersonNameType()
{
	if(m_pLang) delete m_pLang;
	if(m_pTypeTab) delete m_pTypeTab;
	if(m_ppFamilyName)
	{
		for(int i = 0; i < m_nFamilyNameCount; i++)
		{
			if(m_ppFamilyName[i]) delete m_ppFamilyName[i];
		}
		delete [] m_ppFamilyName;
	}
	if(m_ppGivenName)
	{
		for(int i = 0; i < m_nGivenNameCount; i++)
		{
			if(m_ppGivenName[i]) delete m_ppGivenName[i];
		}
		delete [] m_ppGivenName;
	}
	if(m_ppLinkingName)
	{
		for(int i = 0; i < m_nLinkingNameCount; i++)
		{
			if(m_ppLinkingName[i]) delete m_ppLinkingName[i];
		}
		delete [] m_ppLinkingName;
	}
	if(m_ppNumeration)
	{
		for(int i = 0; i < m_nNumerationCount; i++)
		{
			if(m_ppNumeration[i]) delete m_ppNumeration[i];
		}
		delete [] m_ppNumeration;
	}
	if(m_ppSalutation)
	{
		for(int i = 0; i < m_nSalutationCount; i++)
		{
			if(m_ppSalutation[i]) delete m_ppSalutation[i];
		}
		delete [] m_ppSalutation;
	}
	if(m_ppTitle)
	{
		for(int i = 0; i < m_nTitleCount; i++)
		{
			if(m_ppTitle[i]) delete m_ppTitle[i];
		}
		delete [] m_ppTitle;
	}
}

bool PersonNameType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pLang= BimXMLUtil::GetAttributeString(node, ATTR_NAME_XML_LANG);

	m_nFamilyNameCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_FAMILY_NAME);
	m_nGivenNameCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_GIVEN_NAME);
	m_nLinkingNameCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_LINKING_NAME);
	m_nNumerationCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_NUMERATION);
	m_nSalutationCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_SALUTATION);
	m_nTitleCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_TITLE);
	int totalCount = m_nFamilyNameCount + m_nGivenNameCount + m_nLinkingNameCount
				+ m_nNumerationCount + m_nSalutationCount + m_nTitleCount;
	if(totalCount > 0)
	{
		int i;
		if(m_nFamilyNameCount > 0)
		{
			m_ppFamilyName = new TextualType*[m_nFamilyNameCount];
			if(!m_ppFamilyName) return false;
			for(i = 0; i< m_nFamilyNameCount; i++) m_ppFamilyName[i] = NULL;
		}
		if(m_nGivenNameCount > 0)
		{
			m_ppGivenName = new TextualType*[m_nGivenNameCount];
			if(!m_ppGivenName) return false;
			for(i = 0; i< m_nGivenNameCount; i++) m_ppGivenName[i] = NULL;
		}
		if(m_nLinkingNameCount > 0)
		{
			m_ppLinkingName = new TextualType*[m_nLinkingNameCount];
			if(!m_ppLinkingName) return false;
			for(i = 0; i< m_nLinkingNameCount; i++) m_ppLinkingName[i] = NULL;
		}
		if(m_nNumerationCount > 0)
		{
			m_ppNumeration = new DescString_typecast*[m_nNumerationCount];
			if(!m_ppNumeration) return false;
			for(i = 0; i< m_nNumerationCount; i++) m_ppNumeration[i] = NULL;
		}
		if(m_nSalutationCount > 0)
		{
			m_ppSalutation = new TextualType*[m_nSalutationCount];
			if(!m_ppSalutation) return false;
			for(i = 0; i< m_nSalutationCount; i++) m_ppSalutation[i] = NULL;
		}
		if(m_nTitleCount > 0)
		{
			m_ppTitle = new TextualType*[m_nTitleCount];
			if(!m_ppTitle) return false;
			for(i = 0; i< m_nTitleCount; i++) m_ppTitle[i] = NULL;
		}

		m_pTypeTab = new int[totalCount];
		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int familyNum = 0;
		int givenNum = 0;
		int linkingNum = 0;
		int numerationNum = 0;
		int salutationNum = 0;
		int titleNum = 0;
		int totalNum = 0;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_FAMILY_NAME) == 0)
			{
				if(familyNum < m_nFamilyNameCount)
				{
					TextualType *name = new TextualType();
					name->BuildFromXML(nextNode);
					m_ppFamilyName[familyNum++] = name;
					m_pTypeTab[totalNum++] = PERSON_FAMILY_NAME;
				}
			}
			else if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_GIVEN_NAME) == 0)
			{
				if(givenNum < m_nGivenNameCount)
				{
					TextualType *name = new TextualType();
					name->BuildFromXML(nextNode);
					m_ppGivenName[givenNum++] = name;
					m_pTypeTab[totalNum++] = PERSON_GIVEN_NAME;
				}
			}
			else if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_LINKING_NAME) == 0)
			{
				if(linkingNum < m_nLinkingNameCount)
				{
					TextualType *name = new TextualType();
					name->BuildFromXML(nextNode);
					m_ppGivenName[linkingNum++] = name;
					m_pTypeTab[totalNum++] = PERSON_LINKING_NAME;
				}
			}
			else if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_NUMERATION) == 0)
			{
				if(numerationNum < m_nNumerationCount)
				{
					//wchar_t *data = BimXMLUtil::GetNodeText(nextNode);
					DescString_typecast *name = new DescString_typecast();
					name->BuildFromXML(nextNode);
					m_ppNumeration[numerationNum++] = name;
					m_pTypeTab[totalNum++] = PERSON_NUMERATION;
				}
			}
			else if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_SALUTATION) == 0)
			{
				if(salutationNum < m_nSalutationCount)
				{
					TextualType *name = new TextualType();
					name->BuildFromXML(nextNode);
					m_ppSalutation[salutationNum++] = name;
					m_pTypeTab[totalNum++] = PERSON_SALUTATION;
				}
			}
			else if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_TITLE) == 0)
			{
				if(titleNum < m_nTitleCount)
				{
					TextualType *name = new TextualType();
					name->BuildFromXML(nextNode);
					m_ppTitle[titleNum++] = name;
					m_pTypeTab[totalNum++] = PERSON_TITLE;
				}
			}
			if(totalNum >= totalCount) break;
		}
	}
	return true;
}

int PersonNameType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pLang)
	{
		bitCount += m_pLang->GetNumberOfBits();
	}
	bitCount++;
	bool bOpt = false;
	if(m_ppFamilyName || m_ppGivenName || m_ppLinkingName || m_ppNumeration
		|| m_ppSalutation || m_ppTitle)
	{
		bOpt = true;
	}
	int totalCount = m_nFamilyNameCount + m_nGivenNameCount + m_nLinkingNameCount
				+ m_nNumerationCount + m_nSalutationCount + m_nTitleCount;

	if(totalCount == 0)
	{
		bOpt = false;
	}
	if(bOpt)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(totalCount);
		int familyNum=0;
		int givenNum=0;
		int linkingNum=0;
		int numerationNum=0;
		int salutationNum=0;
		int titleNum=0;
		for(int i = 0; i < totalCount; i++)
		{
			bitCount += 3;
			switch (m_pTypeTab[i])
			{
			case PERSON_FAMILY_NAME:
				bitCount += m_ppFamilyName[familyNum++]->GetNumberOfBits();
				break;
			case PERSON_GIVEN_NAME:
				bitCount += m_ppGivenName[givenNum++]->GetNumberOfBits();
				break;
			case PERSON_LINKING_NAME:
				bitCount += m_ppLinkingName[linkingNum++]->GetNumberOfBits();
				break;
			case PERSON_NUMERATION:
				bitCount += m_ppNumeration[numerationNum++]->GetNumberOfBits();
				break;
			case PERSON_SALUTATION:
				bitCount += m_ppSalutation[salutationNum++]->GetNumberOfBits();
				break;
			case PERSON_TITLE:
				bitCount += m_ppTitle[titleNum++]->GetNumberOfBits();
				break;
			default:
				break;
			}
		}
	}

	return bitCount;
}

bool PersonNameType::Validate()
{
	return true;
}

bool PersonNameType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret = true;
	bool bHasLang = (m_pLang == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bHasLang);
	}
	if(bret && bHasLang)
	{
		bret = m_pLang->WriteBitstream(writer);
	}

	bool bOptGroup = false;
	int totalCount = m_nFamilyNameCount + m_nGivenNameCount + m_nLinkingNameCount
				+ m_nNumerationCount + m_nSalutationCount + m_nTitleCount;

	if(totalCount > 0)
	{
		bOptGroup = true;
	}
	if(bret)
	{
		bret = writer->WriteBool(bOptGroup);
	}
	if(bret && bOptGroup)
	{
		bret = writer->WriteVarLenInt5(totalCount);
		int familyNum=0;
		int givenNum=0;
		int linkingNum=0;
		int numerationNum=0;
		int salutationNum=0;
		int titleNum=0;
		for(int i = 0; i < totalCount; i++)
		{
			if(!bret) break;
			bret = writer->WriteBits(m_pTypeTab[i], 3);
			if(!bret) break;
			switch (m_pTypeTab[i])
			{
			case PERSON_FAMILY_NAME:
				bret = m_ppFamilyName[familyNum++]->WriteBitstream(writer);
				break;
			case PERSON_GIVEN_NAME:
				bret = m_ppGivenName[givenNum++]->WriteBitstream(writer);
				break;
			case PERSON_LINKING_NAME:
				bret = m_ppLinkingName[linkingNum++]->WriteBitstream(writer);
				break;
			case PERSON_NUMERATION:
				bret = m_ppNumeration[numerationNum++]->WriteBitstream(writer);
				break;
			case PERSON_SALUTATION:
				bret = m_ppSalutation[salutationNum++]->WriteBitstream(writer);
				break;
			case PERSON_TITLE:
				bret = m_ppTitle[titleNum++]->WriteBitstream(writer);
				break;
			default:
				break;
			}
		}
	}

	return bret;
}

bool PersonNameType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pLang);
	SAFE_DELETE(m_pTypeTab);
	SAFE_DELETEPP(m_ppFamilyName, m_nFamilyNameCount);
	SAFE_DELETEPP(m_ppGivenName, m_nGivenNameCount);
	SAFE_DELETEPP(m_ppLinkingName, m_nLinkingNameCount);
	SAFE_DELETEPP(m_ppNumeration, m_nNumerationCount);
	SAFE_DELETEPP(m_ppSalutation, m_nSalutationCount);
	SAFE_DELETEPP(m_ppTitle, m_nTitleCount);
	
	bool bret = true;

	do {
		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pLang = new DescString();
			BREAK_IFNULL(m_pLang);
			bret = m_pLang->ReadBitstream(reader);
			if(!bret) break;
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			int totalCount = (int)val;
			if(totalCount > 0)
			{
				std::vector <TextualType*> fNameV;
				std::vector <TextualType*> gNameV;
				std::vector <TextualType*> lNameV;
				std::vector <DescString_typecast*> numerV;
				std::vector <TextualType*> salutV;
				std::vector <TextualType*> titleV;
				std::vector <int> typeV;
				for(int i = 0; i < totalCount; i++)
				{
					int nType;
					TextualType *pData;
					DescString_typecast *pNumData;
					bret = reader->ReadBits(&nType, 3);
					if(!bret) break;
					typeV.push_back(nType);
					switch (nType)
					{
					case PERSON_FAMILY_NAME:
						pData = new TextualType();
						RETURN_IFNULL(pData);
						bret = pData->ReadBitstream(reader);
						if(!bret) break;
						fNameV.push_back(pData);
						break;
					case PERSON_GIVEN_NAME:
						pData = new TextualType();
						RETURN_IFNULL(pData);
						bret = pData->ReadBitstream(reader);
						gNameV.push_back(pData);
						break;
					case PERSON_LINKING_NAME:
						pData = new TextualType();
						RETURN_IFNULL(pData);
						bret = pData->ReadBitstream(reader);
						lNameV.push_back(pData);
						break;
					case PERSON_NUMERATION:
						pNumData = new DescString_typecast();
						RETURN_IFNULL(pNumData);
						bret = pNumData->ReadBitstream(reader);
						numerV.push_back(pNumData);
						break;
					case PERSON_SALUTATION:
						pData = new TextualType();
						RETURN_IFNULL(pData);
						bret = pData->ReadBitstream(reader);
						salutV.push_back(pData);
						break;
					case PERSON_TITLE:
						pData = new TextualType();
						RETURN_IFNULL(pData);
						bret = pData->ReadBitstream(reader);
						titleV.push_back(pData);
						break;
					default:
						break;
					}
					if(!bret) break;
				}
				m_nFamilyNameCount=(int)fNameV.size();
				m_nGivenNameCount=(int)gNameV.size();
				m_nLinkingNameCount=(int)lNameV.size();
				m_nNumerationCount=(int)numerV.size();
				m_nSalutationCount=(int)salutV.size();
				m_nTitleCount=(int)titleV.size();

				m_pTypeTab = new int[totalCount];
				std::vector <int>::iterator type_Iter;
				int typeNum = 0;
				for(type_Iter = typeV.begin(); type_Iter != typeV.end(); type_Iter++)
				{
					if(typeNum >= totalCount)
					{
						bret = false;
						break;
					}
					m_pTypeTab[typeNum++] = *type_Iter;
				}
				if(typeNum != totalCount) bret = false;
				if(!bret) break;

				if(m_nFamilyNameCount > 0)
				{
					m_ppFamilyName = new TextualType*[m_nFamilyNameCount];
					ZEROP(m_ppFamilyName, m_nFamilyNameCount);
				}
				if(m_nGivenNameCount > 0)
				{
					m_ppGivenName = new TextualType*[m_nGivenNameCount];
					ZEROP(m_ppGivenName, m_nGivenNameCount);
				}
				if(m_nLinkingNameCount > 0)
				{
					m_ppLinkingName = new TextualType*[m_nLinkingNameCount];
					ZEROP(m_ppLinkingName, m_nLinkingNameCount);
				}
				if(m_nNumerationCount > 0)
				{
					m_ppNumeration = new DescString_typecast*[m_nNumerationCount];
					ZEROP(m_ppNumeration, m_nNumerationCount);
				}
				if(m_nSalutationCount > 0)
				{
					m_ppSalutation = new TextualType*[m_nSalutationCount];
					ZEROP(m_ppSalutation, m_nSalutationCount);
				}
				if(m_nTitleCount > 0)
				{
					m_ppTitle = new TextualType*[m_nTitleCount];
					ZEROP(m_ppTitle, m_nTitleCount);
				}

				int familyNum=0;
				int givenNum=0;
				int linkingNum=0;
				int numerationNum=0;
				int salutationNum=0;
				int titleNum=0;
				std::vector <TextualType*>::iterator fName_Iter = fNameV.begin();
				std::vector <TextualType*>::iterator gName_Iter = gNameV.begin();
				std::vector <TextualType*>::iterator lName_Iter = lNameV.begin();
				std::vector <DescString_typecast*>::iterator numer_Iter = numerV.begin();
				std::vector <TextualType*>::iterator salut_Iter = salutV.begin();
				std::vector <TextualType*>::iterator title_Iter = titleV.begin();
				for(int i = 0; i < totalCount; i++)
				{
					if(m_pTypeTab[i] == PERSON_FAMILY_NAME)
					{
						TextualType *name = *fName_Iter;
						m_ppFamilyName[familyNum++] = name;
						fName_Iter++;
					}
					else if(m_pTypeTab[i] == PERSON_GIVEN_NAME)
					{
						TextualType *name = *gName_Iter;
						m_ppGivenName[givenNum++] = name;
						gName_Iter++;
					}
					else if(m_pTypeTab[i] == PERSON_LINKING_NAME)
					{
						TextualType *name = *lName_Iter;
						m_ppLinkingName[linkingNum++] = name;
						lName_Iter++;
					}
					else if(m_pTypeTab[i] == PERSON_NUMERATION)
					{
						DescString_typecast *name = *numer_Iter;
						m_ppNumeration[numerationNum++] = name;
						numer_Iter++;
					}
					else if(m_pTypeTab[i] == PERSON_SALUTATION)
					{
						TextualType *name = *salut_Iter;
						m_ppLinkingName[salutationNum++] = name;
						salut_Iter++;
					}
					else if(m_pTypeTab[i] == PERSON_TITLE)
					{
						TextualType *name = *title_Iter;
						m_ppTitle[titleNum++] = name;
						title_Iter++;
					}
				}
			}
		}
	}
	while(false);

	return bret;
}

void PersonNameType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pLang && m_pLang->GetData() != NULL)
	{
		*fout << "lang=\"";
		m_pLang->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;
	int totalCount = m_nFamilyNameCount + m_nGivenNameCount + m_nLinkingNameCount
				+ m_nNumerationCount + m_nSalutationCount + m_nTitleCount;
	if(totalCount > 0 && m_pTypeTab != NULL)
	{
		int familyNum=0;
		int givenNum=0;
		int linkingNum=0;
		int numerationNum=0;
		int salutationNum=0;
		int titleNum=0;
		for(int i = 0; i < totalCount; i++)
		{
			int nType = m_pTypeTab[i];
			switch (nType)
			{
			case PERSON_FAMILY_NAME:
				if(familyNum < m_nFamilyNameCount)
				{
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintStartElement(fout, ELEM_NAME_FAMILY_NAME, false);
					if(m_ppFamilyName[familyNum]) m_ppFamilyName[familyNum++]->PrintXML(fout, nIndentLevel + 1);
					BimXMLUtil::PrintEndElement(fout, ELEM_NAME_FAMILY_NAME);
					*fout << std::endl;
				}
				break;
			case PERSON_GIVEN_NAME:
				if(givenNum < m_nGivenNameCount)
				{
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintStartElement(fout, ELEM_NAME_GIVEN_NAME, false);
					if(m_ppGivenName[givenNum]) m_ppGivenName[givenNum++]->PrintXML(fout, nIndentLevel + 1);
					BimXMLUtil::PrintEndElement(fout, ELEM_NAME_GIVEN_NAME);
					*fout << std::endl;
				}
				break;
			case PERSON_LINKING_NAME:
				if(linkingNum < m_nLinkingNameCount)
				{
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintStartElement(fout, ELEM_NAME_LINKING_NAME, false);
					if(m_ppLinkingName[linkingNum]) m_ppLinkingName[linkingNum++]->PrintXML(fout, nIndentLevel + 1);
					BimXMLUtil::PrintEndElement(fout, ELEM_NAME_LINKING_NAME);
					*fout << std::endl;
				}
				break;
			case PERSON_NUMERATION:
				if(numerationNum < m_nNumerationCount)
				{
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintStartElement(fout, ELEM_NAME_NUMERATION, false);
					if(m_ppNumeration[numerationNum]) m_ppNumeration[numerationNum]->PrintXML(fout, nIndentLevel);
					BimXMLUtil::PrintEndElement(fout, ELEM_NAME_NUMERATION);
					*fout << std::endl;
				}
				break;
			case PERSON_SALUTATION:
				if(salutationNum < m_nSalutationCount)
				{
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintStartElement(fout, ELEM_NAME_SALUTATION, false);
					if(m_ppSalutation[salutationNum]) m_ppSalutation[salutationNum++]->PrintXML(fout, nIndentLevel + 1);
					BimXMLUtil::PrintEndElement(fout, ELEM_NAME_SALUTATION);
					*fout << std::endl;
				}
				break;
			case PERSON_TITLE:
				if(titleNum < m_nTitleCount)
				{
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintStartElement(fout, ELEM_NAME_TITLE, false);
					if(m_ppTitle[titleNum]) m_ppTitle[titleNum++]->PrintXML(fout, nIndentLevel + 1);
					BimXMLUtil::PrintEndElement(fout, ELEM_NAME_TITLE);
					*fout << std::endl;
				}
				break;
			default:
				break;
			}
		}
	}
}
