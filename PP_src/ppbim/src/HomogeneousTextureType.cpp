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

#define TEXTURE_LIST_COUNT 30
#define TEXTURE_ITEM_LENGTH 8

HomogeneousTextureType::HomogeneousTextureType()
{
	m_nAverage = 0;
	m_nStandardDeviation = 0;
	m_pEnergy = NULL;
	m_pEnergyDeviation = NULL;
}

HomogeneousTextureType::~HomogeneousTextureType()
{
	if(m_pEnergy) delete m_pEnergy;
	if(m_pEnergyDeviation) delete m_pEnergyDeviation;
}

bool HomogeneousTextureType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_AVERAGE);
	if(childNode)
	{
		wchar_t *data = BimXMLUtil::GetNodeText(childNode);
		if(data)
		{
			m_nAverage = _wtoi(data);
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (HomogeneousTextureType)");
		return false;
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_STANDARD_DEVIATION);
	if(childNode)
	{
		wchar_t *data = BimXMLUtil::GetNodeText(childNode);
		if(data)
		{
			m_nStandardDeviation = _wtoi(data);
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (HomogeneousTextureType)");
		return false;
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_ENERGY);
	if(childNode)
	{
		m_pEnergy = new ValueListType(TEXTURE_LIST_COUNT, TEXTURE_ITEM_LENGTH,15);
		if(!m_pEnergy || !m_pEnergy->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pEnergy);
			return false;
		}
	}
	else
	{
		BimXMLUtil::ReportError("invalid (HomogeneousTextureType)");
		return false;
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_ENERGY_DEVIATION);
	if(childNode)
	{
		m_pEnergyDeviation = new ValueListType(TEXTURE_LIST_COUNT, TEXTURE_ITEM_LENGTH, 15);
		if(!m_pEnergyDeviation || !m_pEnergyDeviation->BuildFromXML(childNode))
		{
			SAFE_DELETE(m_pEnergyDeviation);
			return false;
		}
	}

	return true;
}

void HomogeneousTextureType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_AVERAGE);
	*fout << m_nAverage;
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_AVERAGE);
	*fout << std::endl;

	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_STANDARD_DEVIATION);
	*fout << m_nStandardDeviation;
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_STANDARD_DEVIATION);
	*fout << std::endl;

	if(m_pEnergy)
	{
		BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pEnergy, fout, nIndentLevel + 1,ELEM_NAME_ENERGY , true);
		/*BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_ENERGY);
		*fout << std::endl;
		BimXMLUtil::PrintIndent(fout, nIndentLevel + 1);
		m_pEnergy->PrintXML(fout, nIndentLevel + 1);
		*fout << std::endl;
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_ENERGY);
		*fout << std::endl;*/
	}
	if(m_pEnergyDeviation)
	{
		BimXMLUtil::PrintXMLDS((PPMpeg7Desc*)m_pEnergyDeviation, fout, nIndentLevel + 1,ELEM_NAME_ENERGY_DEVIATION , true);
		/*BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_ENERGY_DEVIATION);
		*fout << std::endl;
		BimXMLUtil::PrintIndent(fout, nIndentLevel + 1);
		m_pEnergyDeviation->PrintXML(fout, nIndentLevel + 1);
		*fout << std::endl;
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_ENERGY_DEVIATION);
		*fout << std::endl;*/
	}
}

int HomogeneousTextureType::GetNumberOfBits()
{
	int bitCount = 8 + 8;
	if(m_pEnergy)
	{
		bitCount += m_pEnergy->GetNumberOfBits();
	}
	bitCount++;
	if(m_pEnergyDeviation)
	{
		bitCount += m_pEnergyDeviation->GetNumberOfBits();
	}
	return bitCount;
}

bool HomogeneousTextureType::Validate()
{
	return true;
}

bool HomogeneousTextureType::WriteBitstream(BitstreamWriter *writer)
{
	RETURN_IFNULL(m_pEnergy);

	bool bret;

	bret = writer->WriteBits(m_nAverage, 8);

	if(bret)
	{
		bret = writer->WriteBits(m_nStandardDeviation, 8);
	}

	if(bret)
	{
		bret = m_pEnergy->WriteBitstream(writer);
	}

	if(bret)
	{
		bret = BimXMLUtil::WriteOptionalDS((PPMpeg7Desc*)m_pEnergyDeviation, writer);
	}

	return bret;
}

bool HomogeneousTextureType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pEnergy);
	SAFE_DELETE(m_pEnergyDeviation);

	bool bret;
	do {
		bret = reader->ReadBits(&m_nAverage, 8);
		if(!bret) break;

		bret = reader->ReadBits(&m_nStandardDeviation, 8);
		if(!bret) break;

		m_pEnergy = new ValueListType(TEXTURE_LIST_COUNT, TEXTURE_ITEM_LENGTH, 15);
		BREAK_IFNULL(m_pEnergy);
		bret = m_pEnergy->ReadBitstream(reader);
		if(!bret) break;

		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pEnergyDeviation = new ValueListType(TEXTURE_LIST_COUNT, TEXTURE_ITEM_LENGTH, 15);
			BREAK_IFNULL(m_pEnergyDeviation);
			bret = m_pEnergyDeviation->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}
