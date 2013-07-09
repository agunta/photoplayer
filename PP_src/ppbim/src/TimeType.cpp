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

TimeType::TimeType()
{
	m_pTimePoint = NULL;
	m_pDuration = NULL;
}

TimeType::~TimeType()
{
	if(m_pTimePoint) delete m_pTimePoint;
	if(m_pDuration) delete m_pDuration;
}

bool TimeType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_TIME_POINT);
	if(childNode)
	{
		m_pTimePoint = new TimePointType();
		if(!m_pTimePoint) return false;
		if(!m_pTimePoint->BuildFromXML(childNode))
		{
			delete m_pTimePoint;
			m_pTimePoint = NULL;
		}
	}
	else
	{
		BimXMLUtil::ReportError("missing element <TimePoint> in element <Data>");
		return false;
	}
	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_DURATION);
	if(childNode)
	{
		wchar_t *data = BimXMLUtil::GetNodeText(childNode);
		if(data)
		{
			m_pDuration = new DescString();
			RETURN_IFNULL(m_pDuration);
			m_pDuration->SetData(data);
		}
	}
	return true;
}

int TimeType::GetNumberOfBits()
{
	int bitCount = 0;
	if(m_pTimePoint)
	{
		bitCount += m_pTimePoint->GetNumberOfBits();
	}
	bitCount++;
	if(m_pDuration)
	{
		bitCount += m_pDuration->GetNumberOfBits();
	}

	return bitCount;
}

bool TimeType::Validate()
{
	return true;
}

bool TimeType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pTimePoint);
	bret = m_pTimePoint->WriteBitstream(writer);
	bool bOpt = (m_pDuration == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bOpt);
	}
	if(bret && bOpt)
	{
		bret = m_pDuration->WriteBitstream(writer);
	}
	return bret;
}

bool TimeType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pTimePoint);
	SAFE_DELETE(m_pDuration);
	
	bool bret = true;

	do {
		m_pTimePoint = new TimePointType();
		BREAK_IFNULL(m_pTimePoint);
		bret = m_pTimePoint->ReadBitstream(reader);
		if(!bret) break;

		bool bOpt;
		bret = reader->ReadBool(&bOpt);
		if(!bret) break;
		if(bOpt)
		{
			m_pDuration = new DescString();
			BREAK_IFNULL(m_pDuration);
			bret = m_pDuration->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}
void TimeType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pTimePoint)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_TIME_POINT, true);
		m_pTimePoint->PrintXML(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_TIME_POINT);
		*fout << std::endl;
	}

	if(m_pDuration)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_DURATION, true);
		m_pDuration->PrintXML(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_DURATION);
		*fout << std::endl;
	}
}