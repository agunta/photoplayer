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

PostalAddress_anonymousType::PostalAddress_anonymousType()
{
	m_pLang = NULL;
	m_nAddressLineCount = 0;
	m_ppAddressLines = NULL;
	m_pPostingIdentifier = NULL;

}

PostalAddress_anonymousType::~PostalAddress_anonymousType()
{
	if(m_pLang) delete m_pLang;
	if(m_ppAddressLines)
	{
		for(int i = 0; i < m_nAddressLineCount; i++)
		{
			if(m_ppAddressLines[i]) delete m_ppAddressLines[i];
		}
		delete [] m_ppAddressLines;
	}
	if(m_pPostingIdentifier) delete m_pPostingIdentifier;
}

bool PostalAddress_anonymousType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	m_pLang= BimXMLUtil::GetAttributeString(node, ATTR_NAME_XML_LANG);

	m_nAddressLineCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_ADDRESS_LINE);
	if(m_nAddressLineCount < 1)
	{
		BimXMLUtil::ReportError("<PostalAddress> element doesn't contain <AddressLine> element");
		return false;
	}

	if(m_nAddressLineCount > 0)
	{
		m_ppAddressLines = new TextualType*[m_nAddressLineCount];
		if(!m_ppAddressLines) return false;
		int i;
		for(i = 0; i< m_nAddressLineCount; i++) m_ppAddressLines[i] = 0;

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int lineNum = 0;
		for(i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_ADDRESS_LINE) == 0)
			{
				if(lineNum < m_nAddressLineCount)
				{
					TextualType *line = new TextualType();
					if(!line->BuildFromXML(nextNode))
					{
						BimXMLUtil::ReportWarning("Invalid <AddressLine>");
					}
					m_ppAddressLines[lineNum++] = line;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
		}
	}
	else
	{
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_POSTING_IDENTIFIER);
	if(childNode)
	{
		m_pPostingIdentifier = new TextualType();
		if(!m_pPostingIdentifier) return false;
		if(!m_pPostingIdentifier->BuildFromXML(childNode))
		{
			delete m_pPostingIdentifier;
			m_pPostingIdentifier = NULL;
		}
	}
	return true;
}

int PostalAddress_anonymousType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pLang)
	{
		bitCount += m_pLang->GetNumberOfBits();
	}
	bitCount += BimXMLUtil::GetVLI5BitSize(m_nAddressLineCount - 1);
	for(int i = 0; i < m_nAddressLineCount; i++)
	{
		if(m_ppAddressLines[i]) bitCount += m_ppAddressLines[i]->GetNumberOfBits();
	}
	bitCount++;
	if(m_pPostingIdentifier)
	{
		bitCount += m_pPostingIdentifier->GetNumberOfBits();
	}
	return bitCount;
}

bool PostalAddress_anonymousType::Validate()
{
	return true;
}

bool PostalAddress_anonymousType::WriteBitstream(BitstreamWriter *writer)
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
		assert(m_nAddressLineCount > 0);
		bret = writer->WriteVarLenInt5(m_nAddressLineCount - 1);
		if(bret)
		{
			for(int i = 0; i < m_nAddressLineCount; i++)
			{
				RETURN_IFNULL(m_ppAddressLines[i]);
				bret = m_ppAddressLines[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	if(bret)
	{
		bOpt = (m_pPostingIdentifier == NULL) ? false : true;
		bret = writer->WriteBool(bOpt);
		if(bret && bOpt)
		{
			bret = m_pPostingIdentifier->WriteBitstream(writer);
		}
	}

	return bret;
}

bool PostalAddress_anonymousType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pLang);
	SAFE_DELETEPP(m_ppAddressLines, m_nAddressLineCount);
	SAFE_DELETE(m_pPostingIdentifier);
	
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
		m_nAddressLineCount = (int)val + 1;
		if(m_nAddressLineCount > 0)
		{
			m_ppAddressLines = new TextualType*[m_nAddressLineCount];
			RETURN_IFNULL(m_ppAddressLines);
			ZEROP(m_ppAddressLines, m_nAddressLineCount);
			for(int i = 0; i < m_nAddressLineCount; i++)
			{
				TextualType *line = new TextualType();
				RETURN_IFNULL(line);
				bret = line->ReadBitstream(reader);
				if(!bret) return false;
				m_ppAddressLines[i] = line;
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
			m_pPostingIdentifier = new TextualType();
			BREAK_IFNULL(m_pPostingIdentifier);
			bret = m_pPostingIdentifier->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}

void PostalAddress_anonymousType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pLang)
	{
		*fout << " lang=\"";
		m_pLang->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	if(m_ppAddressLines)
	{
		for(int i = 0; i < m_nAddressLineCount; i++)
		{
			if(!m_ppAddressLines[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_ADDRESS_LINE, false);
			m_ppAddressLines[i]->PrintXML(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_ADDRESS_LINE);
			*fout << std::endl;
		}
	}

	if(m_pPostingIdentifier)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_POSTING_IDENTIFIER, false);
		m_pPostingIdentifier->PrintXML(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_POSTING_IDENTIFIER);
		*fout << std::endl;
	}


}