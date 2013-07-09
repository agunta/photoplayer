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

ProfileAndLevelIndication::ProfileAndLevelIndication()
{
	m_nMax = 0;
	m_ppItem = NULL;
}

ProfileAndLevelIndication::~ProfileAndLevelIndication()
{
	SAFE_DELETEPP(m_ppItem, m_nMax);
}

bool ProfileAndLevelIndication::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_nMax = 0;
	DOMNamedNodeMap *map = node->getAttributes();
	if(!map) return false;
	int maplen = map->getLength();
	int i;
	for(i=0; i < maplen; i++)
	{
		DOMNode *nextNode = map->item(i);
		char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
		if(nodeName != NULL && strcmp(nodeName, ATTR_NAME_PROFILEANDLEVEL) == 0)
		{
			m_nMax++;
		}
		if(nodeName) XMLString::release(&nodeName);
	}
	if(m_nMax > 0)
	{
		m_ppItem = new DescAnyURI*[m_nMax];
		if(!m_ppItem) { delete map; return false; }
		int profileNum = 0;
		for(i=0; i < maplen; i++)
		{
			DOMNode *nextNode = map->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ATTR_NAME_PROFILEANDLEVEL) == 0)
			{
				wchar_t *value = (wchar_t*)nextNode->getNodeValue();
				DescAnyURI *uri = new DescAnyURI();
				if(uri && value)
				{
					uri->SetURI(value);
				}
				m_ppItem[profileNum++] = uri;
				if(value) XMLString::release(&value);
			}
			if(nodeName) XMLString::release(&nodeName);
			if(profileNum >= m_nMax) break;
		}
	}
	//delete map;
	return true;
}

int ProfileAndLevelIndication::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount += BimXMLUtil::GetVLI5BitSize(m_nMax);
	for(int i = 0; i < m_nMax; i++)
	{
		if(m_ppItem[i]) bitCount += m_ppItem[i]->GetNumberOfBits();
	}

	return bitCount;
}

bool ProfileAndLevelIndication::Validate()
{
	return true;
}

bool ProfileAndLevelIndication::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bret = writer->WriteVarLenInt5(m_nMax);
	if(bret)
	{
		for(int i = 0; i < m_nMax; i++)
		{
			if(m_ppItem[i])
			{
				bret = m_ppItem[i]->WriteBitstream(writer);
			}
			else
			{
				bret = false;
			}
			if(!bret) break;
		}
	}
	return bret;
}

bool ProfileAndLevelIndication::ReadBitstream(BitstreamReader *reader)
{
	m_nMax = 0;
	SAFE_DELETEPP(m_ppItem, m_nMax);
	
	bool bret = true;

	do {
		unsigned int val;
		bret = reader->ReadVarLenInt5(&val);
		if(bret)
		{
			m_nMax = (int)val;
			if(m_nMax > 0)
			{
				m_ppItem = new DescAnyURI*[m_nMax];
				BREAK_IFNULL(m_ppItem);
			}
			else
			{
				bret = false;
			}
			if(!bret) break;
			for(int i =0; i < m_nMax; i++)
			{
				DescAnyURI *uri = new DescAnyURI();
				BREAK_IFNULL(uri);
				bret = uri->ReadBitstream(reader);
				m_ppItem[i] = uri;
				if(!bret) break;
			}
		}
	}
	while(false);

	return bret;
}

void ProfileAndLevelIndication::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout || !m_ppItem) return;
	for(int i = 0; i < m_nMax; i++)
	{
		if(!m_ppItem[i] || m_ppItem[i]->GetURI() == NULL) continue;
		*fout << " " << ATTR_NAME_PROFILEANDLEVEL << "=\"" << m_ppItem[i]->GetURI() << '"';
	}
}
