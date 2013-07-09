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

#define NONNEGINT_TYPECAST_COUNT 11
static char* NONNEGINT_TYPECAST_NAMES[NONNEGINT_TYPECAST_COUNT] =
	{	"positiveInteger",
		"unsignedLong",
		"unsignedInt",
		"unsignedShort",
		"unsignedByte",
		"unsigned1",
		"unsigned12",
		"unsigned3",
		"unsigned5",
		"unsigned6",
		"unsigned8",
};


NonNegativeInteger_typecast::NonNegativeInteger_typecast()
{
	m_bTypecast = false;
	m_nSubtype = -1;
	m_pIntValue = NULL;
}

NonNegativeInteger_typecast::~NonNegativeInteger_typecast()
{
	SAFE_DELETE(m_pIntValue);
}

bool NonNegativeInteger_typecast::BuildFromXML(DOMNode *node)
{
	SAFE_DELETE(m_pIntValue);
	m_bTypecast = false;

	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_XSI_TYPE);
	if(attr)
	{
		m_nSubtype = GetSubtype(attr);
		if(m_nSubtype >= 0)
		{
			m_bTypecast = true;
		}
		else { return false; }

	}
	// we assume that the data is validated by the schema
	m_pIntValue = new NonNegativeInteger();
	RETURN_IFNULL(m_pIntValue);
	if(!m_pIntValue->BuildFromXML(node)) return false;
	return true;
}

void NonNegativeInteger_typecast::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_bTypecast && (m_nSubtype >= 0 && m_nSubtype < NONNEGINT_TYPECAST_COUNT))
	{
		*fout << ' ' << ATTR_NAME_XSI_TYPE << "=\"";
		*fout << NONNEGINT_TYPECAST_NAMES[m_nSubtype];
		*fout << '"';
	}
	*fout << '>';
	if(m_pIntValue)
	{
		m_pIntValue->PrintXML(fout, nIndentLevel);
	}
}

int NonNegativeInteger_typecast::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_bTypecast)
	{
		bitCount += 4;
	}
	if(m_bTypecast)
	{
		if(m_pIntValue && (m_nSubtype == 0 || m_nSubtype == 1 || m_nSubtype == 2))
		{
			bitCount += m_pIntValue->GetNumberOfBits();
		}
		else if(m_nSubtype > 2 && m_nSubtype < NONNEGINT_TYPECAST_COUNT)
		{
			bitCount += GetSubtypeBits(m_nSubtype);
		}
		else
		{
			// error
			bitCount = 0;
		}
	}
	else if(m_pIntValue)
	{
		bitCount += m_pIntValue->GetNumberOfBits();
	}
	else
	{
		//error
		bitCount = 0;
	}
	return bitCount;

}

bool NonNegativeInteger_typecast::Validate()
{
	// not implemented
	return false;
}

bool NonNegativeInteger_typecast::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	if(m_pIntValue == NULL) return false;
	if(m_bTypecast && (m_nSubtype < 0 || m_nSubtype >= NONNEGINT_TYPECAST_COUNT)) return false;

	bret = writer->WriteBool(m_bTypecast);
	if(bret && m_bTypecast)
	{
		bret = writer->WriteBits(m_nSubtype, 4);
		if(!bret) return false;
		if(m_pIntValue && (m_nSubtype == 0 || m_nSubtype == 1 || m_nSubtype == 2))
		{
			m_pIntValue->WriteBitstream(writer);
		}
		else if(m_nSubtype > 2 && m_nSubtype < NONNEGINT_TYPECAST_COUNT)
		{
			int nBits = GetSubtypeBits(m_nSubtype);
			if(nBits > 0)
			{
				writer->WriteBits(m_pIntValue->m_nValue, nBits);
			}
			else { bret = false; }
		}
		else { bret = false; }
	}
	else if(bret)
	{
		bret = m_pIntValue->WriteBitstream(writer);
	}
	return bret;
}

bool NonNegativeInteger_typecast::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pIntValue);
	bool bret = true;

	m_pIntValue = new NonNegativeInteger();
	RETURN_IFNULL(m_pIntValue);

	do {
		bret = reader->ReadBool(&m_bTypecast);
		if(!bret) break;
		if(m_bTypecast)
		{
			int nVal = 0;
			bret = reader->ReadBits(&m_nSubtype, 4);
			if(m_nSubtype >= NONNEGINT_TYPECAST_COUNT)
			{
				bret = false;
				break;
			}
			else if(m_nSubtype == 0 || m_nSubtype == 1  || m_nSubtype == 2)
			{
				if(!m_pIntValue->ReadBitstream(reader))
				{
					bret = false; break;
				}
			}
			else if(m_nSubtype > 2 && m_nSubtype < NONNEGINT_TYPECAST_COUNT)
			{
				int nBits = GetSubtypeBits(m_nSubtype);
				bret = reader->ReadBits(&nVal, nBits);
				if(!bret) break;
				m_pIntValue->m_nValue = nVal;
			}
			else
			{
				bret = false; break;
			}
		}
		else
		{
			if(!m_pIntValue->ReadBitstream(reader))
			{
				bret = false; break;
			}
		}
	}
	while(false);

	return bret;
}

int NonNegativeInteger_typecast::GetSubtype(wchar_t *wattr)
{
	if(wattr == NULL) return -1;
	int i;
	int nSubType = -1;
	char strAttr[32];
	int nLen = (int) wcslen(wattr);
	if(nLen > 0 && nLen < 32)
	{
		sprintf(strAttr, "%S", wattr);
		for(i = 0; i < NONNEGINT_TYPECAST_COUNT; i++)
		{
			if(strcmp(strAttr, NONNEGINT_TYPECAST_NAMES[i]) == 0)
			{
				nSubType = i;
				break;
			}
		}
	}
	return nSubType;
}

int NonNegativeInteger_typecast::GetSubtypeBits(int nSubType)
{
	int nBits = 0;
	switch(nSubType)
	{
	case 3:
		nBits = 16;
		break;
	case 4:
		nBits = 8;
		break;
	case 5:
		nBits = 1;
		break;
	case 6:
		nBits = 12;
		break;
	case 7:
		nBits = 3;
		break;
	case 8:
		nBits = 5;
		break;
	case 9:
		nBits = 6;
		break;
	case 10:
		nBits = 8;
		break;
	default:
		break;
	}
	return nBits;
}
