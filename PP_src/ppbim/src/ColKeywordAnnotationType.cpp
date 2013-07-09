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

ColKeywordAnnotationType::ColKeywordAnnotationType()
{
	m_pLang = NULL;
	m_nKeywordCount = 0;
	m_ppKeyword = NULL;
}

ColKeywordAnnotationType::~ColKeywordAnnotationType()
{
	SAFE_DELETE(m_pLang);
	SAFE_DELETEPP(m_ppKeyword, m_nKeywordCount);
}

bool ColKeywordAnnotationType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pLang =  BimXMLUtil::GetAttributeString(node, ATTR_NAME_XML_LANG);

	m_nKeywordCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_KEYWORD);
	if(m_nKeywordCount > 0)
	{
		m_ppKeyword = new ColKeyword_anonymousType*[m_nKeywordCount];
		if(!m_ppKeyword) return false;
		int i;
		for(i = 0; i< m_nKeywordCount; i++) m_ppKeyword[i] = 0;

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int keywordNum = 0;
		for(i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_KEYWORD) == 0)
			{
				if(keywordNum < m_nKeywordCount)
				{
					ColKeyword_anonymousType *keyword = new ColKeyword_anonymousType();
					if(!keyword->BuildFromXML(nextNode))
					{
						BimXMLUtil::ReportWarning("Invalid keyword");
					}
					m_ppKeyword[keywordNum++] = keyword;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
		}
	}
	return true;
}

int ColKeywordAnnotationType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pLang)
	{
		bitCount += m_pLang->GetNumberOfBits();
	}
	bitCount += BimXMLUtil::GetVLI5BitSize(m_nKeywordCount - 1);
	for(int i = 0; i < m_nKeywordCount; i++)
	{
		if(m_ppKeyword[i]) bitCount += m_ppKeyword[i]->GetNumberOfBits();
	}
	return bitCount;
}

bool ColKeywordAnnotationType::Validate()
{
	return true;
}

bool ColKeywordAnnotationType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOpt = (m_pLang == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pLang->WriteBitstream(writer);
	}
	if(bret)
	{
		assert(m_nKeywordCount > 0);
		bret = writer->WriteVarLenInt5(m_nKeywordCount - 1);
		if(bret)
		{
			for(int i = 0; i < m_nKeywordCount; i++)
			{
				RETURN_IFNULL(m_ppKeyword[i]);
				bret = m_ppKeyword[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	return bret;
}

bool ColKeywordAnnotationType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pLang);
	SAFE_DELETEPP(m_ppKeyword, m_nKeywordCount);
	
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
		m_nKeywordCount = (int)val + 1;
		if(m_nKeywordCount > 0)
		{
			m_ppKeyword = new ColKeyword_anonymousType*[m_nKeywordCount];
			RETURN_IFNULL(m_ppKeyword);
			ZEROP(m_ppKeyword, m_nKeywordCount);
			for(int i = 0; i < m_nKeywordCount; i++)
			{
				ColKeyword_anonymousType *keyword = new ColKeyword_anonymousType();
				RETURN_IFNULL(keyword);
				bret = keyword->ReadBitstream(reader);
				if(!bret) return false;
				m_ppKeyword[i] = keyword;
			}
		}
		else
		{
			bret = false;
			break;
		}
	}
	while(false);

	return bret;
}

void ColKeywordAnnotationType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pLang)
	{
		*fout << ' ' << ATTR_NAME_XML_LANG << "=\"";
		m_pLang->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	if(m_ppKeyword)
	{
		for(int i = 0; i < m_nKeywordCount; i++)
		{
			if(!m_ppKeyword[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_KEYWORD, false);
			m_ppKeyword[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_KEYWORD);
			*fout << std::endl;
		}
	}
}
