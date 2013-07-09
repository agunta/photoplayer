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


CSValues_anonymousType::CSValues_anonymousType()
{
	m_nFormatLength = 20;
	m_nMax = 0;
	m_pItems = NULL;
}

CSValues_anonymousType::~CSValues_anonymousType()
{
	SAFE_DELETE(m_pItems);
}

bool CSValues_anonymousType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	m_nMax = 0;
	wchar_t *data = BimXMLUtil::GetNodeText(node);
	if(data)
	{
		m_nMax = BimXMLUtil::ParseIntVectorLength(data);
		if(m_nMax <= 0 || m_nMax > 256)
		{
			//XMLString::release(&data);
			return false;
		}
	
		m_pItems = new int[m_nMax];
		int nItemsCount = 0;
		if(m_pItems)
		{
			nItemsCount = BimXMLUtil::ParseIntVector(data, m_pItems, m_nMax);
		}
		//XMLString::release(&data);
		if(nItemsCount != m_nMax)
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

void CSValues_anonymousType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!m_pItems) return;
	for(int i = 0; i < m_nMax; i++)
	{
		if(i > 0)
		{
			if((m_nFormatLength != 0 ) && (i%m_nFormatLength == 0))
			{
				*fout << std::endl;
				BimXMLUtil::PrintIndent(fout, nIndentLevel);
			}
			else
			{
				*fout << ' ';
			}
		}
		*fout << m_pItems[i];
	}
}

int CSValues_anonymousType::GetNumberOfBits()
{
	int bitCount = 0;

	if(m_nMax < 1) return 0;
	bitCount += 8;
	bitCount += m_nMax * 8;
	return bitCount;
}

bool CSValues_anonymousType::Validate()
{
	return true;
}

bool CSValues_anonymousType::WriteBitstream(BitstreamWriter *writer)
{
	if(!m_pItems || m_nMax < 1) return false;
	bool bret;
	if(m_nMax <= 0) return false;
	bret = writer->WriteBits(m_nMax - 1, 8);
	for(int i = 0; i < m_nMax; i++)
	{
		bret = writer->WriteBits(m_pItems[i], 8);
		if(!bret) break;
	}
	return bret;
}

bool CSValues_anonymousType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pItems);
	m_nMax = 0;
	bool bret;

	do
	{
		bret = reader->ReadBits(&m_nMax, 8);
		if(!bret) break;
		m_nMax++;
		//if(m_nMax == 0) break;
		m_pItems = new int[m_nMax];
		RETURN_IFNULL(m_pItems);
		for(int i = 0; i < m_nMax; i++)
		{
			bret = reader->ReadBits(&m_pItems[i], 8);
			if(!bret) break;
		}
	}
	while(false);

	return bret;
}
