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

MediaInstanceType::MediaInstanceType()
{
	m_pId = NULL;
	m_pInstanceIdentifier = NULL;
	m_pLocationDescription = NULL;
	m_pMediaLocator = NULL;

}

MediaInstanceType::~MediaInstanceType()
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pInstanceIdentifier);
	SAFE_DELETE(m_pLocationDescription);
	SAFE_DELETE(m_pMediaLocator);
}

bool MediaInstanceType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pId= BimXMLUtil::GetAttributeString(node, ATTR_NAME_ID);

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_INSTANCE_IDENTIFIER);
	if(childNode)
	{
		m_pInstanceIdentifier = new UniqueIDType();
		if(!m_pInstanceIdentifier || !m_pInstanceIdentifier->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pInstanceIdentifier);
			return false;
		}
	}
	else
	{
		BimXMLUtil::ReportError("missing <InstanceIdentifier> (MediaInstanceType)");
		return false;
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_LOCATION_DESC);
	if(childNode)
	{
		m_pLocationDescription = new TextualType();
		if(!m_pLocationDescription || !m_pLocationDescription->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pLocationDescription);
			return false;
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_MEDIA_LOCATOR);
	if(childNode)
	{
		if(m_pLocationDescription)
		{
			BimXMLUtil::ReportError("invalid content of (MediaInstanceType)");
			return false;
		}
		m_pMediaLocator = new MediaLocatorType();
		if(!m_pMediaLocator || !m_pMediaLocator->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pMediaLocator);
			return false;
		}
	}
	// return false if both m_pLocationDescription and m_pMediaLocator are NULL ?

	return true;
}

void MediaInstanceType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	if(m_pId && m_pId->GetData() != NULL)
	{
		*fout << "id=\"";
		m_pId->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>' << std::endl;

	if(m_pInstanceIdentifier)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_INSTANCE_IDENTIFIER, false);
		m_pInstanceIdentifier->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_INSTANCE_IDENTIFIER);
		*fout << std::endl;
	}
	if(m_pLocationDescription)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_LOCATION_DESC, false);
		m_pLocationDescription->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_LOCATION_DESC);
		*fout << std::endl;
	}
	else if(m_pMediaLocator)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_MEDIA_LOCATOR, false);
		m_pMediaLocator->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_MEDIA_LOCATOR);
		*fout << std::endl;
	}
}

int MediaInstanceType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pId)
	{
		bitCount += m_pId->GetNumberOfBits();
	}
	if(m_pInstanceIdentifier)
	{
		bitCount += m_pInstanceIdentifier->GetNumberOfBits();
	}
	bitCount++;
	if(m_pLocationDescription)
	{
		bitCount += m_pLocationDescription->GetNumberOfBits();
	}
	else if(m_pMediaLocator)
	{
		bitCount += m_pMediaLocator->GetNumberOfBits();
	}
	return bitCount;
}

bool MediaInstanceType::Validate()
{
	return true;
}

bool MediaInstanceType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pInstanceIdentifier);
	if((m_pLocationDescription == NULL) && (m_pMediaLocator == NULL))
	{
		return false;
	}

	bool bHasId = (m_pId == NULL) ? false : true;
	bret = writer->WriteBool(bHasId);
	if(bret && bHasId)
	{
		bret = m_pId->WriteBitstream(writer);
	}

	if(bret)
	{
		bret = m_pInstanceIdentifier->WriteBitstream(writer);
	}

	int nChoice = (m_pLocationDescription != NULL) ? 0 : 1;
	if(bret) {
		bret = writer->WriteBits(nChoice, 1);
	}
	if(bret && (nChoice == 0))
	{
		bret = m_pLocationDescription->WriteBitstream(writer);
	}
	else if(bret)
	{
		bret = m_pMediaLocator->WriteBitstream(writer);
	}

	return bret;
}

bool  MediaInstanceType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pId);
	SAFE_DELETE(m_pInstanceIdentifier);
	SAFE_DELETE(m_pLocationDescription);
	SAFE_DELETE(m_pMediaLocator);
	
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
		m_pInstanceIdentifier = new UniqueIDType();
		BREAK_IFNULL(m_pInstanceIdentifier);
		bret = m_pInstanceIdentifier->ReadBitstream(reader);
		if(!bret) break;

		int nChoice = 0;
		bret = reader->ReadBits(&nChoice, 1);
		if(!bret) break;
		if(nChoice == 0)
		{
			m_pLocationDescription= new TextualType();
			BREAK_IFNULL(m_pLocationDescription);
			bret = m_pLocationDescription->ReadBitstream(reader);
		}
		else
		{
			m_pMediaLocator= new MediaLocatorType();
			BREAK_IFNULL(m_pMediaLocator);
			bret = m_pMediaLocator->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}
