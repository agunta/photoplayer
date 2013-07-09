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

static char* ANYURI_TYPECAST_NAME = "termURIReferenceType";

DescAnyURI_typecast::DescAnyURI_typecast()
{
	m_pUri = NULL;
	m_bTypecast = false;
}

DescAnyURI_typecast::~DescAnyURI_typecast()
{
	SAFE_DELETE(m_pUri);
}

bool DescAnyURI_typecast::BuildFromXML(DOMNode *node)
{
	SAFE_DELETE(m_pUri);
	m_bTypecast = false;

	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_XSI_TYPE);
	if(attr)
	{
		m_bTypecast = true;
	}
	m_pUri = new DescAnyURI();
	RETURN_IFNULL(m_pUri);
	if(!m_pUri->BuildFromXML(node)) return false;
	return true;
}

void DescAnyURI_typecast::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	if(m_bTypecast)
	{
		*fout << ' ' << ATTR_NAME_XSI_TYPE << "=\"";
		*fout << ANYURI_TYPECAST_NAME;
		*fout << '"';
	}
	*fout << '>';
	if(m_pUri)
	{
		m_pUri->PrintXML(fout, nIndentLevel);
	}
}

int DescAnyURI_typecast::GetNumberOfBits()
{
	if(!m_pUri) return 0;
	int bitCount = 0;

	bitCount++;
	bitCount += m_pUri->GetNumberOfBits();

	return bitCount;
}

bool DescAnyURI_typecast::Validate()
{
	return false;
}

bool DescAnyURI_typecast::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	if(m_pUri == NULL) return false;

	bret = writer->WriteBool(m_bTypecast);
	if(bret)
	{
		m_pUri->WriteBitstream(writer);
	}
	return bret;
}

bool DescAnyURI_typecast::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pUri);
	
	bool bret = true;

	do {
		bret = reader->ReadBool(&m_bTypecast);
		if(!bret) break;
		m_pUri = new DescAnyURI();
		RETURN_IFNULL(m_pUri);
		if(!m_pUri->ReadBitstream(reader))
		{
			SAFE_DELETE(m_pUri);
			bret = false;
		}
	}
	while(false);

	return bret;
}
