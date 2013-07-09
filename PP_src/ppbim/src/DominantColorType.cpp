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

DominantColorType::DominantColorType()
{
	m_nSpatialCoherency = 0;
	m_nValueCount = 0;
	m_ppValues = NULL;
}

DominantColorType::~DominantColorType()
{
	SAFE_DELETE(m_ppValues);
}

bool DominantColorType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_SPATIAL_COHERENCY);
	if(childNode)
	{
		wchar_t *data = BimXMLUtil::GetNodeText(childNode);
		if(data)
		{
			m_nSpatialCoherency = _wtoi(data);
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (DominantColor)");
		return false;
	}

	m_nValueCount = BimXMLUtil::CountDOMElements(node, ELEM_NAME_VALUE);
	if(m_nValueCount > 0)
	{
		m_ppValues = new DominantColorValueType*[m_nValueCount];
		RETURN_IFNULL(m_ppValues);
		ZEROP(m_ppValues, m_nValueCount);

		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		int valueNum = 0;
		for(int i = 0; i < listlen; i++)
		{
			bool bValid = true;
			DOMNode *nextNode = list->item(i);
			char *nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, ELEM_NAME_VALUE) == 0)
			{
				if(valueNum < m_nValueCount)
				{
					DominantColorValueType *value = new DominantColorValueType();
					if(!value || !value->BuildFromXML(nextNode))
					{
						SAFE_DELETE(value);
						BimXMLUtil::ReportWarning("Invalid <Value> in <DominantColorType>");
						bValid = false;
					}
					m_ppValues[valueNum++] = value;
				}
			}
			if(nodeName) XMLString::release(&nodeName);
			if(!bValid) return false;
		}
	}

	return true;
}

void DominantColorType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_SPATIAL_COHERENCY);
	*fout << m_nSpatialCoherency;
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_SPATIAL_COHERENCY);
	*fout << std::endl;

	if(m_ppValues)
	{
		for(int i = 0; i < m_nValueCount; i++)
		{
			if(!m_ppValues[i]) continue;
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintStartElement(fout, ELEM_NAME_VALUE);
			*fout << std::endl;
			m_ppValues[i]->PrintXML(fout, nIndentLevel + 1);
			BimXMLUtil::PrintIndent(fout, nIndentLevel);
			BimXMLUtil::PrintEndElement(fout, ELEM_NAME_VALUE);
			*fout << std::endl;
		}
	}

}

int DominantColorType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount += 5;
	bitCount += 3;
	if(m_ppValues)
	{
		for(int i = 0; i < m_nValueCount; i++)
		{
			if(m_ppValues[i]) bitCount += m_ppValues[i]->GetNumberOfBits();
		}
	}

	return bitCount;
}

bool DominantColorType::Validate()
{
	return true;
}

bool DominantColorType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;

	bret = writer->WriteBits(m_nSpatialCoherency, 5);

	if((m_nValueCount < 1) || (m_nValueCount > 8)) bret = false;
	if(bret)
	{
		bret = writer->WriteBits(m_nValueCount - 1, 3);
	}

	if(bret)
	{
		for(int i = 0; i < m_nValueCount; i++)
		{
			RETURN_IFNULL(m_ppValues[i]);
			bret = m_ppValues[i]->WriteBitstream(writer);
			if(!bret) break;
		}
	}

	return bret;
}

bool DominantColorType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETEPP(m_ppValues, m_nValueCount);

	bool bret;
	do {
		bret = reader->ReadBits(&m_nSpatialCoherency, 5);
		if(!bret) break;

		bret = reader->ReadBits(&m_nValueCount, 3);
		if(!bret) break;
		m_nValueCount++;
		m_ppValues = new DominantColorValueType*[m_nValueCount];
		RETURN_IFNULL(m_ppValues);
		ZEROP(m_ppValues, m_nValueCount);
		for(int i = 0; i < m_nValueCount; i++)
		{
			DominantColorValueType *elem = new DominantColorValueType();
			RETURN_IFNULL(elem);
			bret = elem->ReadBitstream(reader);
			if(!bret) return false;
			m_ppValues[i] = elem;
		}
	}
	while(false);

	return bret;
}
