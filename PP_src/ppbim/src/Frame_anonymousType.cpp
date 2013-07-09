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


Frame_anonymousType::Frame_anonymousType()
{
	m_bHasHeight = false;
	m_bHasWidth = false;
}

Frame_anonymousType::~Frame_anonymousType()
{
}

bool Frame_anonymousType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	m_bHasHeight = false;
	m_bHasWidth = false;

	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_HEIGHT);
	if(attr)
	{
		m_bHasHeight = true;
		m_nHeight = _wtoi(attr);
		if(m_nHeight < 0)
		{
			BimXMLUtil::ReportError("invalid attribute (height)");
			return false;
		}
	}
	attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_WIDTH);
	if(attr)
	{
		m_bHasWidth = true;
		m_nWidth = _wtoi(attr);
		if(m_nWidth < 0)
		{
			BimXMLUtil::ReportError("invalid attribute (width)");
			return false;
		}
	}
	return true;
}

void Frame_anonymousType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	
	if(m_bHasHeight)
	{
		*fout << " height=\"" << m_nHeight << '"';
	}
	if(m_bHasWidth)
	{
		*fout << " width=\"" << m_nWidth << '"';
	}
	*fout << "/>";
}

int Frame_anonymousType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_bHasHeight)
	{
		int val = m_nHeight;
		if(val < 0) val = -val;
		bitCount += 1 + BimXMLUtil::GetVLI5BitSize(val);
	}
	bitCount++;
	if(m_bHasWidth)
	{
		int val = m_nWidth;
		if(val < 0) val = -val;
		bitCount += 1 + BimXMLUtil::GetVLI5BitSize(val);
	}

	return bitCount;
}

bool Frame_anonymousType::Validate()
{
	return true;
}

bool Frame_anonymousType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;

	bret = writer->WriteBool(m_bHasHeight);
	if(bret && m_bHasHeight)
	{
		if(m_nHeight < 0) return false;
		//bret = writer->WriteVarLenInt5(m_nHeight);
		bret = writer->WriteIntegerVLI5(m_nHeight);
	}

	if(bret)
	{
		bret = writer->WriteBool(m_bHasWidth);
	}
	if(bret && m_bHasWidth)
	{
		if(m_nWidth < 0) return false;
		//bret = writer->WriteVarLenInt5(m_nWidth);
		bret = writer->WriteIntegerVLI5(m_nWidth);
	}

	return bret;
}

bool Frame_anonymousType::ReadBitstream(BitstreamReader *reader)
{
	bool bret;

	m_bHasHeight = false;
	m_bHasWidth = false;

	do {
		bret = reader->ReadBool(&m_bHasHeight);
		if(!bret) break;
		if(m_bHasHeight)
		{
			bret = reader->ReadIntegerVLI5(&m_nHeight);
		}

		bret = reader->ReadBool(&m_bHasWidth);
		if(!bret) break;
		if(m_bHasWidth)
		{
			bret = reader->ReadIntegerVLI5(&m_nWidth);
		}
	}
	while(false);

	return bret;
}