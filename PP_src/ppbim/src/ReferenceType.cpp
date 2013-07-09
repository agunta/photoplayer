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

ReferenceType::ReferenceType()
{
	m_pHref = NULL;
	m_nImgId = -1;
}

ReferenceType::~ReferenceType()
{
	if(m_pHref) delete m_pHref;
}

bool ReferenceType::BuildFromXML(DOMNode *node)
{
	bool bNotSupported = false; // to do ...
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	m_pHref= BimXMLUtil::GetAttributeString(node, ATTR_NAME_HREF);

	if(m_pHref == NULL)
	{
		wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_IDREF);
		if(attr) bNotSupported = true;
		if(attr == NULL)
		{
			attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_XPATH);
			if(attr) bNotSupported = true;
		}
	}
	if(bNotSupported)
	{
		// test
		BimXMLUtil::ReportWarning("Not supported attribute of element <ContentRef> occured (idref or xpath)");
		return false;
	}
	if(m_pHref)
	{
		m_nImgId = GetItemId(m_pHref->GetData());
	}

	return true;
}

int ReferenceType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pHref)
	{
		bitCount += m_pHref->GetNumberOfBits();
	}
	return bitCount;
}

bool ReferenceType::Validate()
{
	return true;
}

bool ReferenceType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bool bHasOpt = (m_pHref == NULL) ? false : true;
	bret = writer->WriteBool(bHasOpt);
	if(bret && bHasOpt)
	{
		bret = m_pHref->WriteBitstream(writer);
	}
	return bret;
}

bool ReferenceType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pHref);
	m_nImgId = -1;
	
	bool bret;
	do {
		bool bHasOpt;
		bret = reader->ReadBool(&bHasOpt);
		if(!bret) break;
		if(bHasOpt)
		{
			m_pHref = new DescString();
			RETURN_IFNULL(m_pHref);
			bret = m_pHref->ReadBitstream(reader);
			if(!bret) break;
			m_nImgId = GetItemId(m_pHref->GetData());
		}
	}
	while(false);
	
	return bret;
}

int ReferenceType::GetItemId(wchar_t *data)
{
	int i = -1;
	// simple parsing of strings in the form "#ffp(track_ID=1)"
	if(data)
	{
		int len = (int)wcslen(data);
		wchar_t *strp = data;
		while(*strp == L' ')
		{
			strp++;
			len--;
			if(len == 0) return i;
		}
		if(wcsncmp(strp, L"#ffp(", 5) != 0 || len <= 5) return i;
		strp += 5;
		len -= 5;
		while(*strp == L' ')
		{
			strp++;
			len--;
			if(len == 0) return i;
		}
		if(wcsncmp(strp, L"track_ID", 8) != 0 || len <= 8) return i;
		strp += 8;
		len -= 8;
		while(*strp == L' ')
		{
			strp++;
			len--;
			if(len == 0) return i;
		}
		if(*strp != L'=' || len <= 1) return i;
		strp ++;
		len --;
		while(*strp == L' ')
		{
			strp++;
			len--;
			if(len == 0) return i;
		}
		int numlen = 0;
		wchar_t *num_bigin = strp;
		while(*strp >= L'0' && *strp <= L'9')
		{
			strp++;
			numlen++;
			len--;
			if(len == 0) break;
		}
		if(numlen > 0)
		{
			wchar_t *numstr = (wchar_t*)malloc((numlen + 1) * sizeof(wchar_t));
			if(!numstr) return i;
			wmemcpy(numstr, num_bigin, numlen);
			*(numstr + numlen) = 0;
			i = _wtoi(numstr);
			free(numstr);
		}
	}
	return i;
}

void ReferenceType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	if(m_pHref)
	{
		*fout << " href=\"";
		m_pHref->PrintXML(fout, nIndentLevel);
		*fout << '"';
	}
}
