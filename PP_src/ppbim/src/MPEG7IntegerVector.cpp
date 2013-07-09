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

MPEG7IntegerVector::MPEG7IntegerVector()
{
	m_nFormatLength = 20;
	m_nMax = 0;
	m_pItems = NULL;
}

MPEG7IntegerVector::~MPEG7IntegerVector()
{
	if(m_pItems) delete m_pItems;
}

bool MPEG7IntegerVector::BuildFromXML(DOMNode *node)
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
		if(m_nMax <= 0)
		{
			XMLString::release(&data);
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

void MPEG7IntegerVector::PrintXML(std::ostream *fout, int nIndentLevel)
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

int MPEG7IntegerVector::GetNumberOfBits()
{
	int bitCount;

	bitCount = BimXMLUtil::GetVLI5BitSize(m_nMax);
	for(int i = 0; i < m_nMax; i++)
	{
		int val = m_pItems[i];
		if(val < 0) val = -val;
		bitCount += 1 + BimXMLUtil::GetVLI5BitSize(val);
	}
	return bitCount;
}

bool MPEG7IntegerVector::Validate()
{
	return true;
}

bool MPEG7IntegerVector::WriteBitstream(BitstreamWriter *writer)
{
	if(!m_pItems) return false;
	bool bret;
	bret = writer->WriteVarLenInt5(m_nMax);
	for(int i = 0; i < m_nMax; i++)
	{
		bret = writer->WriteIntegerVLI5(m_pItems[i]);
		if(!bret) break;
	}
	return bret;
}

bool MPEG7IntegerVector::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pItems);
	m_nMax = 0;
	bool bret;

	do
	{
		unsigned int val;
		bret = reader->ReadVarLenInt5(&val);
		if(!bret) break;
		m_nMax = (int)val;
		if(m_nMax == 0) break;
		m_pItems = new int[m_nMax];
		RETURN_IFNULL(m_pItems);
		for(int i = 0; i < m_nMax; i++)
		{
			bret = reader->ReadIntegerVLI5(&m_pItems[i]);
			if(!bret) break;
		}
	}
	while(false);

	return bret;
}
