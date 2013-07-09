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

ContentCollectionType::ContentCollectionType(bool bDescUnit)
{
	m_bDescriptionUnit = bDescUnit;

	m_pAttributes = NULL;
	m_pContent = NULL;

	m_nReferenceCount = 0;
	m_nCollectionCount = 0;
	m_ppReferences = NULL;
	m_ppCollections = NULL;
}

ContentCollectionType::~ContentCollectionType()
{
	if(m_pAttributes) delete m_pAttributes;
	if(m_pContent) delete m_pContent;

	if(m_ppReferences)
	{
		for(int i = 0; i < m_nReferenceCount; i++)
		{
			if(m_ppReferences[i]) delete m_ppReferences[i];
		}
		delete [] m_ppReferences;
	}
	if(m_ppCollections)
	{
		for(int i = 0; i < m_nCollectionCount; i++)
		{
			if(m_ppCollections[i]) delete m_ppCollections[i];
		}
		delete [] m_ppCollections;
	}
}

bool ContentCollectionType::BuildFromXML(DOMNode *node)
{
	m_pAttributes = new CollectionType_attributes();
	if(!m_pAttributes) return false;
	if(node->hasAttributes())
	{
		if(!m_pAttributes->BuildFromXML(node))
		{
			delete m_pAttributes;
			m_pAttributes = NULL;
		}
	}
	m_pContent = new CollectionType_content();
	if(!m_pContent) return false;
	if(!m_pContent->BuildFromXML(node))
	{
		delete m_pContent;
		m_pContent = NULL;
		return false;
	}
	m_nReferenceCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_CONTENT_REF);
	m_nCollectionCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_CONTENT_COLLECTION);
	if(m_nReferenceCount > 0 || m_nCollectionCount > 0)
	{
		int i;
		if(m_nReferenceCount > 0)
		{
			m_ppReferences = new ReferenceType*[m_nReferenceCount];
			if(!m_ppReferences) return false;
			for(i = 0; i< m_nReferenceCount; i++) m_ppReferences[i] = 0;
		}
		if(m_nCollectionCount > 0)
		{
			m_ppCollections = new ContentCollectionType*[m_nCollectionCount];
			if(!m_ppCollections) return false;
			for(i = 0; i< m_nCollectionCount; i++) m_ppCollections[i] = 0;
		}

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int referenceNum = 0;
		int collectionNum = 0;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_CONTENT_REF) == 0)
			{
				if(referenceNum < m_nReferenceCount)
				{
					ReferenceType *reference = new ReferenceType();
					reference->BuildFromXML(nextNode);
					m_ppReferences[referenceNum++] = reference;
				}
			}
			else if( nodeName != NULL && strcmp(nodeName, ELEM_NAME_CONTENT_COLLECTION) == 0)
			{
				if(collectionNum < m_nCollectionCount)
				{
					ContentCollectionType *col = new ContentCollectionType(false);
					col->BuildFromXML(nextNode);
					m_ppCollections[collectionNum++] = col;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
		}
	}


	return true;
}

int ContentCollectionType::GetNumberOfBits()
{
	int bitCount = 0;
	if(m_pAttributes)
	{
		bitCount += m_pAttributes->GetNumberOfBits();
	}
	if(m_pContent)
	{
		bitCount += m_pContent->GetNumberOfBits();
	}
	bitCount++;
	if(m_nReferenceCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nReferenceCount);
		for(int i = 0; i < m_nReferenceCount; i++)
		{
			if(m_ppReferences[i]) bitCount += m_ppReferences[i]->GetNumberOfBits();
		}
	}
	bitCount++;
	if(m_nCollectionCount > 0)
	{
		bitCount += BimXMLUtil::GetVLI5BitSize(m_nCollectionCount);
		for(int i = 0; i < m_nCollectionCount; i++)
		{
			if(m_ppCollections[i]) bitCount += m_ppCollections[i]->GetNumberOfBits();
		}
	}
	return bitCount;
}

bool ContentCollectionType::Validate()
{
	return true;
}

bool ContentCollectionType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;

	RETURN_IFNULL(m_pAttributes);
	RETURN_IFNULL(m_pContent);
	bret = m_pAttributes->WriteBitstream(writer);
	if(bret)
	{
		bret = m_pContent->WriteBitstream(writer);
	}

	if(bret)
	{
		bool bOpt = (m_ppReferences == NULL) ? false : true;
		if(m_nReferenceCount == 0) bOpt = false;
		bret = writer->WriteBool(bOpt);
		if(bret && bOpt)
		{
			bret = writer->WriteVarLenInt5(m_nReferenceCount);
			if(bret)
			{
				for(int i = 0; i < m_nReferenceCount; i++)
				{
					RETURN_IFNULL(m_ppReferences[i]);
					bret = m_ppReferences[i]->WriteBitstream(writer);
					if(!bret) break;
				}
			}
		}
	}

	if(bret)
	{
		bool bOpt = (m_ppCollections == NULL) ? false : true;
		if(m_nCollectionCount == 0) bOpt = false;
		bret = writer->WriteBool(bOpt);
		if(bret && bOpt)
		{
			bret = writer->WriteVarLenInt5(m_nCollectionCount);
			if(bret)
			{
				for(int i = 0; i < m_nCollectionCount; i++)
				{
					RETURN_IFNULL(m_ppCollections[i]);
					bret = m_ppCollections[i]->WriteBitstream(writer);
					if(!bret) break;
				}
			}
		}
	}

	return bret;
}

bool ContentCollectionType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pAttributes);
	SAFE_DELETE(m_pContent);
	SAFE_DELETEPP(m_ppReferences, m_nReferenceCount);
	SAFE_DELETEPP(m_ppCollections, m_nCollectionCount);
	
	bool bret = true;

	do {
		m_pAttributes = new CollectionType_attributes();
		BREAK_IFNULL(m_pAttributes);
		bret = m_pAttributes->ReadBitstream(reader);
		if(!bret) break;

		m_pContent = new CollectionType_content();
		BREAK_IFNULL(m_pContent);
		bret = m_pContent->ReadBitstream(reader);
		if(!bret) break;

		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nReferenceCount = (int)val;
			if(m_nReferenceCount > 0)
			{
				m_ppReferences = new ReferenceType*[m_nReferenceCount];
				RETURN_IFNULL(m_ppReferences);
				ZEROP(m_ppReferences, m_nReferenceCount);
				for(int i = 0; i < m_nReferenceCount; i++)
				{
					ReferenceType *ref = new ReferenceType();
					RETURN_IFNULL(ref);
					bret = ref->ReadBitstream(reader);
					if(!bret) return false;
					m_ppReferences[i] = ref;
				}
			}
			else
			{
				bret = false;
				break;
			}
		}

		bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			unsigned int val;
			bret = reader->ReadVarLenInt5(&val);
			if(!bret) break;
			m_nCollectionCount = (int)val;
			if(m_nCollectionCount > 0)
			{
				m_ppCollections = new ContentCollectionType*[m_nCollectionCount];
				RETURN_IFNULL(m_ppCollections);
				ZEROP(m_ppCollections, m_nCollectionCount);
				for(int i = 0; i < m_nCollectionCount; i++)
				{
					ContentCollectionType *col = new ContentCollectionType(false);
					RETURN_IFNULL(col);
					bret = col->ReadBitstream(reader);
					if(!bret) return false;
					m_ppCollections[i] = col;
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

void ContentCollectionType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pAttributes != NULL)
	{
		m_pAttributes->PrintXML(fout, nIndentLevel);
	}
	*fout << '>' << std::endl;
	if(m_pContent)
	{
		m_pContent->PrintXML(fout, nIndentLevel);
	}
	if(m_ppReferences)
	{
		for(int i = 0; i < m_nReferenceCount; i++)
		{
			if(!m_ppReferences[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CONTENT_REF, false);
			m_ppReferences[i]->PrintXML(fout, nIndentLevel);
			*fout << "/>" << std::endl;
		}
	}

	if(m_ppCollections)
	{
		for(int i = 0; i < m_nCollectionCount; i++)
		{
			if(!m_ppCollections[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CONTENT_COLLECTION, false);
			m_ppCollections[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CONTENT_COLLECTION);
			*fout << std::endl;
		}
	}

}
