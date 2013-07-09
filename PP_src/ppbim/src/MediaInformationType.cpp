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

MediaInformationType::MediaInformationType()
{
	m_pId = NULL;
	m_nProfileCount = 0;
	m_ppMediaProfile = NULL;
}

MediaInformationType::~MediaInformationType()
{
	SAFE_DELETE(m_pId);
	SAFE_DELETEPP(m_ppMediaProfile, m_nProfileCount);
}

bool MediaInformationType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	m_nProfileCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_MEDIA_PROFILE);
	if(m_nProfileCount > 0)
	{
		m_ppMediaProfile = new MediaProfileType*[m_nProfileCount];
		RETURN_IFNULL(m_ppMediaProfile);
		ZEROP(m_ppMediaProfile, m_nProfileCount);

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int profileNum = 0;
		for(int i = 0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_MEDIA_PROFILE) == 0)
			{
				if(profileNum < m_nProfileCount)
				{
					MediaProfileType *profile = new MediaProfileType();
					if(!profile->BuildFromXML(nextNode))
					{
						BimXMLUtil::ReportWarning("Invalid media profile");
					}
					m_ppMediaProfile[profileNum++] = profile;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
		}
	}
	else
	{
		BimXMLUtil::ReportError("missing <MediaProfile> in <MediaInformation>");
		return false;
	}
	return true;
}

void MediaInformationType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pId)
	{
		*fout << " id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	if(m_ppMediaProfile)
	{
		for(int i = 0; i < m_nProfileCount; i++)
		{
			if(!m_ppMediaProfile[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_MEDIA_PROFILE, false);
			m_ppMediaProfile[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_MEDIA_PROFILE);
			*fout << std::endl;
		}
	}
}

int MediaInformationType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	bitCount += BimXMLUtil::GetVLI5BitSize(m_nProfileCount - 1);
	for(int i = 0; i < m_nProfileCount; i++)
	{
		if(m_ppMediaProfile[i]) bitCount += m_ppMediaProfile[i]->GetNumberOfBits();
	}
	return bitCount;
}

bool MediaInformationType::Validate()
{
	return true;
}

bool MediaInformationType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOpt = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pId->WriteBitstream(writer);
	}
	if(m_nProfileCount < 1)
	{
		BimXMLUtil::ReportError("missing <MediaProfile> in <MediaInformation>");
		bret = false;
	}

	if(bret)
	{
		bret = writer->WriteVarLenInt5(m_nProfileCount - 1);
		if(bret)
		{
			for(int i = 0; i < m_nProfileCount; i++)
			{
				RETURN_IFNULL(m_ppMediaProfile[i]);
				bret = m_ppMediaProfile[i]->WriteBitstream(writer);
				if(!bret) break;
			}
		}
	}
	return bret;
}

bool MediaInformationType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETEPP(m_ppMediaProfile, m_nProfileCount);
	
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

		unsigned int val;
		bret = reader->ReadVarLenInt5(&val);
		if(!bret) break;
		m_nProfileCount = (int)val + 1;
		if(m_nProfileCount > 0)
		{
			m_ppMediaProfile = new MediaProfileType*[m_nProfileCount];
			RETURN_IFNULL(m_ppMediaProfile);
			ZEROP(m_ppMediaProfile, m_nProfileCount);
			for(int i = 0; i < m_nProfileCount; i++)
			{
				MediaProfileType *profile = new MediaProfileType();
				RETURN_IFNULL(profile);
				bret = profile->ReadBitstream(reader);
				if(!bret) return false;
				m_ppMediaProfile[i] = profile;
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
