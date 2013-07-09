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

// Top-level class for Item level metadata
ItemMpeg7::ItemMpeg7()
{
	m_pLang = NULL;
	m_pChild0 = NULL;
	m_pDescriptionUnit = NULL;
}
ItemMpeg7::~ItemMpeg7()
{
	if(m_pLang) delete m_pLang;
	if(m_pChild0) delete m_pChild0;
	if(m_pDescriptionUnit) delete m_pDescriptionUnit;
}

bool ItemMpeg7::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	else
	{
		if(!BimXMLUtil::VerifyName(node, ELEM_NAME_MPEG7)) return false;
	}

	/*
	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_XMLNS);
	if(attr)
	{
		// to do: perform the checking of namespace
	}

	attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_XMLNS_XSI);
	if(attr)
	{
		// to do
	}
	*/

	m_pLang= BimXMLUtil::GetAttributeString(node, ATTR_NAME_XML_LANG);

	m_pChild0 = new Mpeg7Type_content();
	if(!m_pChild0 || !m_pChild0->BuildFromXML(node))
	{
		SAFE_DELETE(m_pChild0);
	}

	DOMNode *descNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_DESCRIPTION_UNIT);
	if(descNode)
	{
		m_pDescriptionUnit = new ImageType();
		if(!m_pDescriptionUnit || !m_pDescriptionUnit->BuildFromXML(descNode))
		{
			SAFE_DELETE(m_pDescriptionUnit);
		}
	}

	if(!m_pChild0)
	{
		BimXMLUtil::ReportError("Invalid content of <Mpeg7> element");
		return false;
	}
	if(!m_pDescriptionUnit)
	{
		BimXMLUtil::ReportError("Missing <DescriptionUnit> element");
		return false;
	}
	return true;
}

int ItemMpeg7::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pLang)
	{
		bitCount += m_pLang->GetNumberOfBits();
	}
	if(m_pChild0)
	{
		bitCount += m_pChild0->GetNumberOfBits();
	}
	if(m_pDescriptionUnit)
	{
		bitCount += m_pDescriptionUnit->GetNumberOfBits();
	}
	return bitCount;
}

bool ItemMpeg7::Validate()
{
	return true;
}

bool ItemMpeg7::WriteBitstream(BitstreamWriter *writer)
{

	bool bret = true;

	bool bHasLang = (m_pLang == NULL) ? false : true;
	bret = writer->WriteBool(bHasLang);
	if(bret && bHasLang)
	{
		bret = m_pLang->WriteBitstream(writer);
	}
	if(bret && m_pChild0)
	{
		bret = m_pChild0->WriteBitstream(writer);
	}
	if(bret && m_pDescriptionUnit)
	{
		bret = m_pDescriptionUnit->WriteBitstream(writer);
	}
	return bret;
}

bool ItemMpeg7::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pLang);
	SAFE_DELETE(m_pChild0);
	SAFE_DELETE(m_pDescriptionUnit);
	
	bool bret = true;
	bool bHasLang;
	bret = reader->ReadBool(&bHasLang);
	if(bret && bHasLang)
	{
		m_pLang = new DescString();
		RETURN_IFNULL(m_pLang);
		bret = m_pLang->ReadBitstream(reader);
	}
	if(bret)
	{
		m_pChild0 = new Mpeg7Type_content();
		if(!m_pChild0) return false;
		bret = m_pChild0->ReadBitstream(reader);
	}
	if(bret)
	{
		m_pDescriptionUnit = new ImageType();
		bret = m_pDescriptionUnit->ReadBitstream(reader);;
	}
	return bret;
}

void ItemMpeg7::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_MPEG7, false);
	if(m_pLang && m_pLang->GetData() != NULL)
	{
		*fout << " lang=\"";
		m_pLang->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
	*fout << " xmlns=\"" << MPEG7_ITEM_SCHEMA_NS << "\" xmlns:xsi=\"" << MPEG7_SCHEMA_XMLNS_XSI;
	*fout << "\" xsi:schemaLocation=\"" << MPEG7_ITEM_SCHEMA_LOCATION;
	*fout << "\">" << std::endl;
	if(m_pChild0) m_pChild0->PrintXML(fout, nIndentLevel + 1);
	if(m_pDescriptionUnit)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel + 1);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_DESCRIPTION_UNIT, false);
		*fout << " xsi:type=\"ImageType\"";

		m_pDescriptionUnit->PrintXML(fout, nIndentLevel + 2);
	
		BimXMLUtil::PrintIndent(fout, nIndentLevel + 1);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_DESCRIPTION_UNIT);
		*fout << std::endl;
	}
	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_MPEG7);
}
