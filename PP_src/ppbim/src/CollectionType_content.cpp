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

CollectionType_content::CollectionType_content()
{
	m_pCreationInfo = NULL;
	m_nTextCount = 0;
	m_ppTextAnnotation = NULL;

}

CollectionType_content::~CollectionType_content()
{
	if(m_pCreationInfo) delete m_pCreationInfo;
	if(m_ppTextAnnotation)
	{
		for(int i = 0; i< m_nTextCount; i++)
		{
			if(m_ppTextAnnotation[i]) delete m_ppTextAnnotation[i];
		}
		delete [] m_ppTextAnnotation;
	}
}

bool CollectionType_content::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_CREATION_INFORMATION);
	if(childNode)
	{
		m_pCreationInfo = new ColCreationInformationType();
		if(!m_pCreationInfo) return false;
		if(!m_pCreationInfo->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pCreationInfo);
		}
	}

	m_nTextCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_TEXT_ANNOTATION);
	if(m_nTextCount > 0)
	{
		m_ppTextAnnotation = new ColTextAnnotationType*[m_nTextCount];
		if(!m_ppTextAnnotation) return false;
		int i;
		for(i = 0; i< m_nTextCount; i++) m_ppTextAnnotation[i] = 0;

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int textNum = 0;
		for(i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_TEXT_ANNOTATION) == 0)
			{
				if(textNum < m_nTextCount)
				{
					ColTextAnnotationType *text = new ColTextAnnotationType();
					if(!text->BuildFromXML(nextNode))
					{
						BimXMLUtil::ReportWarning("Invalid text annotation");
					}
					m_ppTextAnnotation[textNum++] = text;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
			if(textNum >= m_nTextCount) break;
		}
	}
	return true;
}

int CollectionType_content::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pCreationInfo)
	{
		bitCount += m_pCreationInfo->GetNumberOfBits();
	}
	bitCount++;
	if(m_nTextCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nTextCount);
		for(int i = 0; i < m_nTextCount; i++)
		{
			if(m_ppTextAnnotation[i]) bitCount += m_ppTextAnnotation[i]->GetNumberOfBits();
		}
	}
	return bitCount;
}

bool CollectionType_content::Validate()
{
	return true;
}

bool CollectionType_content::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOpt = (m_pCreationInfo == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pCreationInfo->WriteBitstream(writer);
	}
	bOpt = (m_ppTextAnnotation == NULL) ? false : true;
	if(m_nTextCount == 0) bOpt = false;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = writer->WriteVarLenInt5(m_nTextCount);
		if(bret)
		{
			for(int i = 0; i < m_nTextCount; i++)
			{
				RETURN_IFNULL(m_ppTextAnnotation[i]);
				bret = m_ppTextAnnotation[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	return bret;
}

bool CollectionType_content::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pCreationInfo);
	SAFE_DELETEPP(m_ppTextAnnotation, m_nTextCount);
	
	bool bret = true;

	do {
		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pCreationInfo = new ColCreationInformationType();
			BREAK_IFNULL(m_pCreationInfo);
			bret = m_pCreationInfo->ReadBitstream(reader);
			if(!bret) break;
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nTextCount = (int)val;
			if(m_nTextCount > 0)
			{
				m_ppTextAnnotation = new ColTextAnnotationType*[m_nTextCount];
				RETURN_IFNULL(m_ppTextAnnotation);
				ZEROP(m_ppTextAnnotation, m_nTextCount);
				for(int i = 0; i < m_nTextCount; i++)
				{
					ColTextAnnotationType *annot = new ColTextAnnotationType();
					RETURN_IFNULL(annot);
					bret = annot->ReadBitstream(reader);
					if(!bret) return false;
					m_ppTextAnnotation[i] = annot;
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

void CollectionType_content::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pCreationInfo)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CREATION_INFORMATION, false);
		m_pCreationInfo->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CREATION_INFORMATION);
		*fout << std::endl;
	}
	
	if(m_ppTextAnnotation)
	{
		for(int i = 0; i < m_nTextCount; i++)
		{
			if(!m_ppTextAnnotation[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_TEXT_ANNOTATION, false);
			m_ppTextAnnotation[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_TEXT_ANNOTATION);
			*fout << std::endl;
		}
	}
}
