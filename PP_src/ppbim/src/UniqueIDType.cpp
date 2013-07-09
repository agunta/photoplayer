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

UniqueIDType::UniqueIDType()
{
	m_bHasEncoding = false;
	m_pType = NULL;
	m_pValue = NULL;
}

UniqueIDType::~UniqueIDType()
{
	SAFE_DELETE(m_pType);
	SAFE_DELETE(m_pValue);
}

bool UniqueIDType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_ENCODING);
	m_bHasEncoding = false;
	if(attr)
	{
		m_bHasEncoding = true;
		if(wcscmp(attr, L"base16") == 0)
		{
			m_nEncoding = ENCODING_BASE16;
		}
		else if(wcscmp(attr, L"base64") == 0)
		{
			m_nEncoding = ENCODING_BASE64;
		}
		else if(wcscmp(attr, L"text") == 0)
		{
			// default is not saved
			m_bHasEncoding = false;
			m_nEncoding = ENCODING_TEXT;
		}
		else
		{
			m_bHasEncoding = false;
			BimXMLUtil::ReportWarning("Unknown encoding of UniqueID type");
			//BimXMLUtil::ReportWarning(attr);
		}
	}

	attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_TYPE);
	if(attr)
	{
		// Test: default is URI ?
		if(wcscmp(attr, L"URI") != 0)
		{
			m_pType = new DescString();
			RETURN_IFNULL(m_pType);
			m_pType->SetData(attr);
		}
	}

	m_pValue = new DescString();
	RETURN_IFNULL(m_pValue);
	m_pValue->BuildFromXML(node);

	return true;
}

void UniqueIDType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_bHasEncoding)
	{
		*fout << " encoding=\"";
		switch(m_nEncoding)
		{
		case ENCODING_BASE16:
			*fout <<"base16";
			break;
		case ENCODING_BASE64:
			*fout <<"base64";
			break;
		case ENCODING_TEXT:
			*fout <<"text";
			break;
		default:
			break;
		}
		*fout << '"';
	}
	if(m_pType)
	{
		*fout << " type=\"";
		m_pType->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << '>';

	if(m_pValue)
	{
		m_pValue->PrintXML(fout, nIndentLevel);
	}
}

int UniqueIDType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_bHasEncoding)
	{
		bitCount +=2;
	}

	bitCount++;
	if(m_pType)
	{
		bitCount += m_pType->GetNumberOfBits();
	}

	if(m_pValue)
	{
		bitCount += m_pValue->GetNumberOfBits();
	}

	return bitCount;
}

bool UniqueIDType::Validate()
{
	return true;
}

bool UniqueIDType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pValue);

	bret = writer->WriteBool(m_bHasEncoding);
	if(bret && m_bHasEncoding)
	{
		bret = writer->WriteBits(m_nEncoding, 2);
	}

	bool bOpt = (m_pType == NULL) ? false : true;
	bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = m_pType->WriteBitstream(writer);
	}

	if(bret)
	{
		bret = m_pValue->WriteBitstream(writer);
	}

	return bret;
}

bool UniqueIDType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pType);
	SAFE_DELETE(m_pValue);
	
	bool bret = true;

	do {
		bret = reader->ReadBool(&m_bHasEncoding);
		if(!bret) break;
		if(m_bHasEncoding)
		{
			m_nEncoding = 0;
			reader->ReadBits(&m_nEncoding, 2);
			if(!bret) break;
		}

		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pType = new DescString();
			BREAK_IFNULL(m_pType);
			bret = m_pType->ReadBitstream(reader);
			if(!bret) break;
		}

		m_pValue = new DescString();
		BREAK_IFNULL(m_pValue);
		bret = m_pValue->ReadBitstream(reader);
	}
	while(false);

	return bret;
}
