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

MediaProfileType::MediaProfileType()
{
	m_pId = NULL;
	m_pMediaFormat = NULL;
	m_pMediaInstance = NULL;
}

MediaProfileType::~MediaProfileType()
{
	if(m_pId) delete m_pId;
	if(m_pMediaFormat) delete m_pMediaFormat;
	if(m_pMediaInstance) delete m_pMediaInstance;
}

bool MediaProfileType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_MEDIA_FORMAT);
	if(childNode)
	{
		m_pMediaFormat = new MediaFormatType();
		if(!m_pMediaFormat || !m_pMediaFormat->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pMediaFormat);
			return false;
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_MEDIA_INSTANCE);
	if(childNode)
	{
		m_pMediaInstance = new MediaInstanceType();
		if(!m_pMediaInstance || !m_pMediaInstance->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pMediaInstance);
			return false;
		}
	}

	return true;
}

void MediaProfileType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << " id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;
	
	if(m_pMediaFormat)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_MEDIA_FORMAT);
		*fout << std::endl;
		m_pMediaFormat->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_MEDIA_FORMAT);
		*fout << std::endl;
	}

	if(m_pMediaInstance)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_MEDIA_INSTANCE, false);
		m_pMediaInstance->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_MEDIA_INSTANCE);
		*fout << std::endl;
	}
}

int MediaProfileType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	bitCount++;
	if(m_pMediaFormat)
	{
		bitCount += m_pMediaFormat->GetNumberOfBits();
	}
	bitCount++;
	if(m_pMediaInstance)
	{
		bitCount += m_pMediaInstance->GetNumberOfBits();
	}
	return bitCount;
}

bool MediaProfileType::Validate()
{
	return true;
}

bool MediaProfileType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bOpt = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pId->WriteBitstream(writer);
	}

	bOpt = (m_pMediaFormat == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = m_pMediaFormat->WriteBitstream(writer);
	}

	bOpt = (m_pMediaInstance == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = m_pMediaInstance->WriteBitstream(writer);
	}
	return bret;
}

bool MediaProfileType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pMediaFormat);
	SAFE_DELETE(m_pMediaInstance);
	
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
		if(!bret) break;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pMediaFormat = new MediaFormatType();
			BREAK_IFNULL(m_pMediaFormat);
			bret = m_pMediaFormat->ReadBitstream(reader);
			if(!bret) break;
		}
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pMediaInstance = new MediaInstanceType();
			BREAK_IFNULL(m_pMediaInstance);
			bret = m_pMediaInstance->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}
