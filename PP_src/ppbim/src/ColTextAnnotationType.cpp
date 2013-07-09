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
#include <vector>

ColTextAnnotationType::ColTextAnnotationType()
{
	m_pLang = NULL;
	m_nFreeTextCount = 0;;
	m_nKeywordCount = 0;;
	m_pTypeTab = NULL;
	m_ppKeywords = NULL;
	m_ppFreeTexts = NULL;

}

ColTextAnnotationType::~ColTextAnnotationType()
{
	SAFE_DELETE(m_pLang);
	SAFE_DELETEPP(m_ppFreeTexts, m_nFreeTextCount);
	SAFE_DELETEPP(m_ppKeywords, m_nKeywordCount);
	SAFE_DELETE(m_pTypeTab);
}

bool ColTextAnnotationType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pLang = BimXMLUtil::GetAttributeString(node, ATTR_NAME_XML_LANG);

	m_nFreeTextCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_FREE_TEXT);
	m_nKeywordCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_KEYWORD_ANNOTATION);
	if(m_nFreeTextCount > 0 || m_nKeywordCount > 0)
	{
		int i;
		if(m_nFreeTextCount > 0)
		{
			m_ppFreeTexts = new TextualType*[m_nFreeTextCount];
			if(!m_ppFreeTexts) return false;
			for(i = 0; i< m_nFreeTextCount; i++) m_ppFreeTexts[i] = NULL;
		}
		if(m_nKeywordCount > 0)
		{
			m_ppKeywords = new ColKeywordAnnotationType*[m_nKeywordCount];
			if(!m_ppKeywords) return false;
			for(i = 0; i< m_nKeywordCount; i++) m_ppKeywords[i] = NULL;
		}

		m_pTypeTab = new int[m_nFreeTextCount + m_nKeywordCount];
		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int ftextNum = 0;
		int keywordNum = 0;
		int totalTexts = 0;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_FREE_TEXT) == 0)
			{
				if(ftextNum < m_nFreeTextCount)
				{
					TextualType *ftext = new TextualType();
					ftext->BuildFromXML(nextNode);
					m_ppFreeTexts[ftextNum++] = ftext;
					m_pTypeTab[totalTexts++] = ANNOTATION_FREE_TEXT;
				}
			}
			else if( nodeName != NULL && strcmp(nodeName, ELEM_NAME_KEYWORD_ANNOTATION) == 0)
			{
				if(keywordNum < m_nKeywordCount)
				{
					ColKeywordAnnotationType *keyword = new ColKeywordAnnotationType();
					keyword->BuildFromXML(nextNode);
					m_ppKeywords[keywordNum++] = keyword;
					m_pTypeTab[totalTexts++] = ANNOTATION_KEYWORD;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
		}
	}
	return true;
}

int ColTextAnnotationType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pLang)
	{
		bitCount += m_pLang->GetNumberOfBits();
	}
	bool bOpt = false;
	if(m_ppFreeTexts || m_ppKeywords)
	{
		bOpt = true;
	}
	int totalCount = m_nFreeTextCount + m_nKeywordCount;

	if(totalCount == 0)
	{
		bOpt = false;
	}
	if(bOpt)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(totalCount - 1);
		int freeNum=0;
		int keywordNum=0;
		for(int i = 0; i < totalCount; i++)
		{
			bitCount += 1;
			switch (m_pTypeTab[i])
			{
			case ANNOTATION_FREE_TEXT:
				bitCount += m_ppFreeTexts[freeNum++]->GetNumberOfBits();
				break;
			case ANNOTATION_KEYWORD:
				bitCount += m_ppKeywords[keywordNum++]->GetNumberOfBits();
				break;
			default:
				break;
			}
		}
	}

	return bitCount;
}

bool ColTextAnnotationType::Validate()
{
	return true;
}

bool ColTextAnnotationType::WriteBitstream(BitstreamWriter *writer)
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
	if(m_ppFreeTexts == NULL && m_ppKeywords == NULL)
	{
		bret = false;
	}
	int totalCount = m_nFreeTextCount + m_nKeywordCount;

	if(totalCount == 0)
	{
		bret = false;
	}
	if(bret)
	{
		assert(totalCount > 0);
		bret = writer->WriteVarLenInt5(totalCount - 1);
		int freeNum=0;
		int keywordNum=0;
		for(int i = 0; i < totalCount; i++)
		{
			if(!bret) break;
			bret = writer->WriteBits(m_pTypeTab[i], 1);
			if(!bret) break;
			switch (m_pTypeTab[i])
			{
			case ANNOTATION_FREE_TEXT:
				bret = m_ppFreeTexts[freeNum++]->WriteBitstream(writer);
				break;
			case ANNOTATION_KEYWORD:
				bret = m_ppKeywords[keywordNum++]->WriteBitstream(writer);
				break;
			default:
				break;
			}
		}
	}

	return bret;
}

bool ColTextAnnotationType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pLang);
	SAFE_DELETE(m_pTypeTab);
	SAFE_DELETEPP(m_ppFreeTexts, m_nFreeTextCount);
	SAFE_DELETEPP(m_ppKeywords, m_nKeywordCount);
	
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

		unsigned int val;
		bret = reader->ReadVarLenInt5(&val);
		if(!bret) break;
		int totalCount = (int)val + 1;
		if(totalCount > 0)
		{
			std::vector <TextualType*> freeV;
			std::vector <ColKeywordAnnotationType*> keywordV;
			std::vector <int> typeV;
			for(int i = 0; i < totalCount; i++)
			{
				int nType;
				TextualType *pFreeData;
				ColKeywordAnnotationType *pKeyData;
				bret = reader->ReadBits(&nType, 1);
				if(!bret) break;
				typeV.push_back(nType);
				switch (nType)
				{
				case ANNOTATION_FREE_TEXT:
					pFreeData = new TextualType();
					RETURN_IFNULL(pFreeData);
					bret = pFreeData->ReadBitstream(reader);
					if(!bret) break;
					freeV.push_back(pFreeData);
					break;
				case ANNOTATION_KEYWORD:
					pKeyData = new ColKeywordAnnotationType();
					RETURN_IFNULL(pKeyData);
					bret = pKeyData->ReadBitstream(reader);
					keywordV.push_back(pKeyData);
					break;
				default:
					break;
				}
				if(!bret) break;
			}
			m_nFreeTextCount=(int)freeV.size();
			m_nKeywordCount=(int)keywordV.size();

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

			if(m_nFreeTextCount > 0)
			{
				m_ppFreeTexts = new TextualType*[m_nFreeTextCount];
				ZEROP(m_ppFreeTexts, m_nFreeTextCount);
			}
			if(m_nKeywordCount > 0)
			{
				m_ppKeywords = new ColKeywordAnnotationType*[m_nKeywordCount];
				ZEROP(m_ppKeywords, m_nKeywordCount);
			}

			int freeNum=0;
			int keywordNum=0;
			std::vector <TextualType*>::iterator free_Iter = freeV.begin();
			std::vector <ColKeywordAnnotationType*>::iterator keyword_Iter = keywordV.begin();
			for(int i = 0; i < totalCount; i++)
			{
				if(m_pTypeTab[i] == ANNOTATION_FREE_TEXT)
				{
					TextualType *text = *free_Iter;
					m_ppFreeTexts[freeNum++] = text;
					free_Iter++;
				}
				else if(m_pTypeTab[i] == ANNOTATION_KEYWORD)
				{
					ColKeywordAnnotationType *keyword = *keyword_Iter;
					m_ppKeywords[keywordNum++] = keyword;
					keyword_Iter++;
				}
			}
		}
	}
	while(false);

	return bret;

}

void ColTextAnnotationType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pLang && m_pLang->GetData() != NULL)
	{
		*fout << "lang=\"";
		m_pLang->PrintXML(fout, nIndentLevel);
	}
	*fout << '>' << std::endl;

	int totalCount = m_nFreeTextCount + m_nKeywordCount;
	if(totalCount > 0 && m_pTypeTab != NULL)
	{
		int freeNum=0;
		int keywordNum=0;
		for(int i = 0; i < totalCount; i++)
		{
			int nType = m_pTypeTab[i];
			switch (nType)
			{
			case ANNOTATION_FREE_TEXT:
				if(freeNum < m_nFreeTextCount)
				{
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintStartElement(fout, ELEM_NAME_FREE_TEXT, false);
					if(m_ppFreeTexts[freeNum]) m_ppFreeTexts[freeNum++]->PrintXML(fout, nIndentLevel + 1);
					BimXMLUtil::PrintEndElement(fout, ELEM_NAME_FREE_TEXT);
					*fout << std::endl;
				}
				break;
			case ANNOTATION_KEYWORD:
				if(keywordNum < m_nKeywordCount)
				{
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintStartElement(fout, ELEM_NAME_KEYWORD_ANNOTATION, false);
					if(m_ppKeywords[keywordNum]) m_ppKeywords[keywordNum++]->PrintXML(fout, nIndentLevel + 1);
					BimXMLUtil::PrintIndent(fout, nIndentLevel);
					BimXMLUtil::PrintEndElement(fout, ELEM_NAME_KEYWORD_ANNOTATION);
					*fout << std::endl;
				}
				break;
			default:
				break;
			}
		}
	}
}
