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

TimePointType::TimePointType()
{
	// to do - this type can have various names: LastUpdata, CreationTime ...
	m_pValue = NULL;
}

TimePointType::~TimePointType()
{
	if(m_pValue) delete m_pValue;
}

bool TimePointType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	wchar_t *data = BimXMLUtil::GetNodeText(node);
	if(data)
	{
		m_pValue = new DescString();
		RETURN_IFNULL(m_pValue);
		m_pValue->SetData(data);
	}
	return true;
}

int TimePointType::GetNumberOfBits()
{
	if(m_pValue) return m_pValue->GetNumberOfBits();
	return 0;
}

bool TimePointType::Validate()
{
	if(m_pValue) return m_pValue->Validate();
	return false;
}

bool TimePointType::WriteBitstream(BitstreamWriter *writer)
{
	if(m_pValue) return m_pValue->WriteBitstream(writer);
	return false;
}

bool TimePointType::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pValue);
	m_pValue = new DescString();
	RETURN_IFNULL(m_pValue);
	if(m_pValue) return m_pValue->ReadBitstream(reader);
	return false;
}

void TimePointType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;
	m_pValue->PrintXML(fout, nIndentLevel);
}
