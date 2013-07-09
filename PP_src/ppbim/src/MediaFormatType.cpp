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

MediaFormatType::MediaFormatType()
{
	m_pContent = NULL;
	m_pFileFormat = NULL;
	m_pFileSize = NULL;
	m_pVisualCoding = NULL;
}

MediaFormatType::~MediaFormatType()
{
	SAFE_DELETE(m_pContent);
	SAFE_DELETE(m_pFileFormat);
	SAFE_DELETE(m_pFileSize);
	SAFE_DELETE(m_pVisualCoding);
}

bool MediaFormatType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_CONTENT);
	if(childNode)
	{
		m_pContent = new ControlledTermUseType();
		if(!m_pContent || !m_pContent->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pContent);
			return false;
		}
	}
	else
	{
		return false;
		BimXMLUtil::ReportError("Missing <Content> in <MediaFormat>");
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_FILE_FORMAT);
	if(childNode)
	{
		m_pFileFormat = new ControlledTermUseType();
		if(!m_pFileFormat || !m_pFileFormat->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pFileFormat);
			return false;
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_FILE_SIZE);
	if(childNode)
	{
		m_pFileSize = new NonNegativeInteger_typecast();
		if(!m_pFileSize || !m_pFileSize->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pFileSize);
			return false;
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_VISUAL_CODING);
	if(childNode)
	{
		m_pVisualCoding = new VisualCoding_anonymousType();
		if(!m_pVisualCoding || !m_pVisualCoding->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pVisualCoding);
			return false;
		}
	}

	return true;
}

void MediaFormatType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pContent)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_CONTENT, false);
		m_pContent->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_CONTENT);
		*fout << std::endl;
	}
	if(m_pFileFormat)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_FILE_FORMAT, false);
		m_pFileFormat->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_FILE_FORMAT);
		*fout << std::endl;
	}
	if(m_pFileSize)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_FILE_SIZE, false);
		m_pFileSize->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_FILE_SIZE);
		*fout << std::endl;
	}
	if(m_pVisualCoding)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_VISUAL_CODING);
		*fout << std::endl;
		m_pVisualCoding->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_VISUAL_CODING);
		*fout << std::endl;
	}
}

int MediaFormatType::GetNumberOfBits()
{
	int bitCount = 0;
	if(m_pContent)
	{
		bitCount += m_pContent->GetNumberOfBits();
	}

	bitCount++;
	if(m_pFileFormat)
	{
		bitCount += m_pFileFormat->GetNumberOfBits();
	}

	bitCount++;
	if(m_pFileSize)
	{
		bitCount += m_pFileSize->GetNumberOfBits();
	}

	bitCount++;
	if(m_pVisualCoding)
	{
		bitCount += m_pVisualCoding->GetNumberOfBits();
	}
	return bitCount;
}

bool MediaFormatType::Validate()
{
	return true;
}

bool MediaFormatType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pContent);
	bret = m_pContent->WriteBitstream(writer);

	bool bOpt = (m_pFileFormat == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = m_pFileFormat->WriteBitstream(writer);
	}	

	bOpt = (m_pFileSize == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = m_pFileSize->WriteBitstream(writer);
	}	

	bOpt = (m_pVisualCoding == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = m_pVisualCoding->WriteBitstream(writer);
	}	

	return bret;
}

bool MediaFormatType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pContent);
	SAFE_DELETE(m_pFileFormat);
	SAFE_DELETE(m_pFileSize);
	SAFE_DELETE(m_pVisualCoding);
	
	bool bret = true;

	do {
		m_pContent = new ControlledTermUseType();
		RETURN_IFNULL(m_pContent);
		bret = m_pContent->ReadBitstream(reader);
		if(!bret) break;

		bool bOpt;

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pFileFormat = new ControlledTermUseType();
			RETURN_IFNULL(m_pFileFormat);
			bret = m_pFileFormat->ReadBitstream(reader);
			if(!bret) break;
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pFileSize = new NonNegativeInteger_typecast();
			RETURN_IFNULL(m_pFileSize);
			bret = m_pFileSize->ReadBitstream(reader);
			if(!bret) break;
		}

		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pVisualCoding = new VisualCoding_anonymousType();
			RETURN_IFNULL(m_pVisualCoding);
			bret = m_pVisualCoding->ReadBitstream(reader);
			if(!bret) break;
		}
	}
	while(false);

	return bret;
}