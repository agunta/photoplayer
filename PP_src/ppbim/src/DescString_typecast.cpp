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

#define STRING_TYPECAST_COUNT 22
static char* STRING_TYPECAST_NAMES[STRING_TYPECAST_COUNT] =
	{	"normalizedString",
		"token",
		"NMTOKEN",
		"NMTOKENS",
		"termAliasReferenceType",
		"Name",
		"NCName",
		"ENTITY",
		"ENTITIES",
		"ID",
		"IDREF",
		"IDREFS",
		"language",
		"TextualBaseType",
		"NameComponentType",
		"TextualType",
		"TitleType",
		"basicDurationType",
		"durationType",
		"basicTimePointType",
		"timePointType",
		"mimeType",
};


int getSubtype(wchar_t *wattr);

DescString_typecast::DescString_typecast()
{
	m_bTypecast = false;
	m_nSubtype = -1;
	m_pValue = NULL;
	m_pValueTextual = NULL;
	m_pValueUID = NULL;
}

DescString_typecast::~DescString_typecast()
{
	SAFE_DELETE(m_pValue);
	SAFE_DELETE(m_pValueTextual);
	SAFE_DELETE(m_pValueUID);

}

bool DescString_typecast::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_XSI_TYPE);
	if(!attr)
	{
		m_bTypecast = false;
		m_pValue = new DescString();
		RETURN_IFNULL(m_pValue);
		if(!m_pValue->BuildFromXML(node)) return false;
		return true;
	}
	m_nSubtype = GetSubtype(attr);

	if(m_nSubtype >= 0)
	{
		m_bTypecast = true;
		if(m_nSubtype == 0
			|| m_nSubtype == 1
			|| m_nSubtype == 2
			|| m_nSubtype == 3
			|| m_nSubtype == 4
			|| m_nSubtype == 5
			|| m_nSubtype == 6
			|| m_nSubtype == 7
			|| m_nSubtype == 8
			|| m_nSubtype == 9
			|| m_nSubtype == 10
			|| m_nSubtype == 11
			|| m_nSubtype == 12
			|| m_nSubtype == 18
			|| m_nSubtype == 19
			|| m_nSubtype == 20
		)
		{
			m_pValue = new DescString();
			if(!m_pValue->BuildFromXML(node))
			{
				SAFE_DELETE(m_pValue);
				return false;
			}
			return true;
		}
		else if(m_nSubtype == 13
			|| m_nSubtype == 14
			|| m_nSubtype == 15
			|| m_nSubtype == 16
			)
		{
			// here are TextualBaseType, NameComponentType, TextualType, TitleType - other values are basic strings
			m_pValueTextual = new TextualType();
			if(!m_pValueTextual || !m_pValueTextual->BuildFromXML(node))
			{
				SAFE_DELETE(m_pValueTextual);
				return false;
			}
			return true;
		}
		else if(m_nSubtype == 17)
		{
			// UniqueID
			m_pValueUID = new UniqueIDType();
			if(!m_pValueUID || !m_pValueUID->BuildFromXML(node))
			{
				SAFE_DELETE(m_pValueUID);
				return false;
			}
			return true;
		}
	}
	return false;
}

void DescString_typecast::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_bTypecast && (m_nSubtype >= 0 && m_nSubtype < STRING_TYPECAST_COUNT))
	{
		*fout << ' ' << ATTR_NAME_XSI_TYPE << "=\"";
		*fout << STRING_TYPECAST_NAMES[m_nSubtype];
		*fout << '"';
	}
	if(m_pValueUID)
	{
		m_pValueUID->PrintXML(fout, nIndentLevel);
	}
	else if(m_pValueTextual)
	{
		m_pValueTextual->PrintXML(fout, nIndentLevel);
	}
	else
	{
		*fout << '>';
		if(m_pValue)
		{
			m_pValue->PrintXML(fout, nIndentLevel);
		}
	}
}

int DescString_typecast::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_pValue && !m_bTypecast)
	{
		bitCount += m_pValue->GetNumberOfBits();
	}
	else if(m_bTypecast)
	{
		bitCount += 5;
		if(m_pValue)
		{
			bitCount += m_pValue->GetNumberOfBits();
		}
		else if(m_pValueTextual)
		{
			bitCount += m_pValueTextual->GetNumberOfBits();
		}
		else if(m_pValueUID)
		{
			bitCount += m_pValueUID->GetNumberOfBits();
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0; // error
	}

	return bitCount;
}

bool DescString_typecast::Validate()
{
	// not implemented
	return false;
}

bool DescString_typecast::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	if((m_pValue == NULL) && (m_pValueTextual == NULL) && (m_pValueUID == NULL) ) return false;
	if(m_bTypecast && (m_nSubtype < 0 || m_nSubtype > 20)) return false;

	bret = writer->WriteBool(m_bTypecast);
	if(bret && !m_bTypecast)
	{
		m_pValue->WriteBitstream(writer);
	}
	else if(bret)
	{
		bret = writer->WriteBits(m_nSubtype, 5);
		if(bret && m_pValue)
		{
			bret = m_pValue->WriteBitstream(writer);
		}
		else if(bret&& m_pValueTextual)
		{
			bret = m_pValueTextual->WriteBitstream(writer);
		}
		else if(bret&& m_pValueUID)
		{
			bret = m_pValueUID->WriteBitstream(writer);
		}
		else
		{
			bret = false;
		}
	}
	return bret;
}

bool DescString_typecast::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pValue);
	SAFE_DELETE(m_pValueTextual);
	
	bool bret = true;

	do {
		bret = reader->ReadBool(&m_bTypecast);
		if(!bret) break;
		if(m_bTypecast)
		{
			bret = reader->ReadBits(&m_nSubtype, 5);
			if(m_nSubtype == 0
				|| m_nSubtype == 1
				|| m_nSubtype == 2
				|| m_nSubtype == 3
				|| m_nSubtype == 4
				|| m_nSubtype == 5
				|| m_nSubtype == 6
				|| m_nSubtype == 7
				|| m_nSubtype == 8
				|| m_nSubtype == 9
				|| m_nSubtype == 10
				|| m_nSubtype == 11
				|| m_nSubtype == 12
				|| m_nSubtype == 18
				|| m_nSubtype == 19
				|| m_nSubtype == 20
				)
			{
				m_pValue = new DescString();
				RETURN_IFNULL(m_pValue);
				if(!m_pValue->ReadBitstream(reader))
				{
					SAFE_DELETE(m_pValue);
					bret = false;
					break;
				}
			}
			else if(m_nSubtype == 13
				|| m_nSubtype == 14
				|| m_nSubtype == 15
				|| m_nSubtype == 16
				)
			{
				// here are TextualBaseType, NameComponentType, TextualType, TitleType - other values are basic strings
				m_pValueTextual = new TextualType();
				if(!m_pValueTextual || !m_pValueTextual->ReadBitstream(reader))
				{
					SAFE_DELETE(m_pValueTextual);
					bret = false;
					break;
				}
			}
			else if(m_nSubtype == 17)
			{
				m_pValueUID = new UniqueIDType();
				if(!m_pValueUID || !m_pValueUID->ReadBitstream(reader))
				{
					SAFE_DELETE(m_pValueUID);
					bret = false;
					break;
				}
			}
			else
			{
				bret = false;
			}
		}
		else
		{
			m_pValue = new DescString();
			RETURN_IFNULL(m_pValue);
			if(!m_pValue->ReadBitstream(reader))
			{
				SAFE_DELETE(m_pValue);
				bret = false;
			}
		}
	}
	while(false);

	return bret;
}

int DescString_typecast::GetSubtype(wchar_t *attr)
{
	int subtype = -1;

	if(wcscmp(attr, ELEM_UNINAME_NORMALIZEDSTRING) == 0)
	{
		subtype = 0;
	}
	else if(wcscmp(attr, ELEM_UNINAME_TOKEN) == 0)
	{
		subtype = 1;
	}
	else if(wcscmp(attr, ELEM_UNINAME_NMTOKEN) == 0)
	{
		subtype = 2;
	}
	else if(wcscmp(attr, ELEM_UNINAME_NMTOKENS) == 0)
	{
		subtype = 3;
	}
	else if(wcscmp(attr, ELEM_UNINAME_TERMALIASREF) == 0)
	{
		subtype = 4;
	}
	else if(wcscmp(attr, ELEM_UNINAME_NAME) == 0)
	{
		subtype = 5;
	}
	else if(wcscmp(attr, ELEM_UNINAME_NCNAME) == 0)
	{
		subtype = 6;
	}
	else if(wcscmp(attr, ELEM_UNINAME_ENTITY) == 0)
	{
		subtype = 7;
	}
	else if(wcscmp(attr, ELEM_UNINAME_ENTITIES) == 0)
	{
		subtype = 8;
	}
	else if(wcscmp(attr, ELEM_UNINAME_ID) == 0)
	{
		subtype = 9;
	}
	else if(wcscmp(attr, ELEM_UNINAME_IDREF) == 0)
	{
		subtype = 10;
	}
	else if(wcscmp(attr, ELEM_UNINAME_IDREFS) == 0)
	{
		subtype = 11;
	}
	else if(wcscmp(attr, ELEM_UNINAME_LANGUAGE) == 0)
	{
		subtype = 12;
	}
	else if(wcscmp(attr, ELEM_UNINAME_TEXTUALBASETYPE) == 0)
	{
		subtype = 13;
	}
	else if(wcscmp(attr, ELEM_UNINAME_NAMECOMPONENTTYPE) == 0)
	{
		subtype = 14;
	}
	else if(wcscmp(attr, ELEM_UNINAME_TEXTUALTYPE) == 0)
	{
		subtype = 15;
	}
	else if(wcscmp(attr, ELEM_UNINAME_TITLETYPE) == 0)
	{
		subtype = 16;
	}
	else if(wcscmp(attr, ELEM_UNINAME_UNIQUEIDTYPE) == 0)
	{
		subtype = 17;
	}
	else if(wcscmp(attr, ELEM_UNINAME_BASICTIMEPOINTTYPE) == 0)
	{
		subtype = 18;
	}
	else if(wcscmp(attr, ELEM_UNINAME_TIMEPOINTTYPE) == 0)
	{
		subtype = 19;
	}

	return subtype;
}
