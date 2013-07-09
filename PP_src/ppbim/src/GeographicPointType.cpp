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

GeographicPointType::GeographicPointType()
{
	m_bHasAltitude = false;
	m_fAltitude = 0.0;
	m_fLatitude = 0.0;
	m_fLongitude = 0.0;
}

GeographicPointType::~GeographicPointType()
{
}

bool GeographicPointType::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}

	wchar_t *attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_ALTITUDE);
	if(attr)
	{
		m_bHasAltitude = true;
		m_fAltitude = _wtof(attr);
	}
	else
	{
		m_bHasAltitude = false;
	}
	attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_LATITUDE);
	if(attr)
	{
		m_fLatitude = _wtof(attr);
	}
	attr = BimXMLUtil::GetAttributeData(node, ATTR_NAME_LONGITUDE);
	if(attr)
	{
		m_fLongitude = _wtof(attr);
	}
	return true;
}

int GeographicPointType::GetNumberOfBits()
{
	int bitCount = 0;
	bitCount++;
	if(m_bHasAltitude) bitCount += 64;
	bitCount += 64;
	bitCount += 64;
	return bitCount;
}

bool GeographicPointType::Validate()
{
	return true;
}

bool GeographicPointType::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	bret = writer->WriteBool(m_bHasAltitude);
	if(bret && m_bHasAltitude)
	{
		bret = writer->WriteDouble(m_fAltitude);
	}
	if(bret)
	{
		bret = writer->WriteDouble(m_fLatitude);
	}
	if(bret)
	{
		bret = writer->WriteDouble(m_fLongitude);
	}
	return bret;
}

bool GeographicPointType::ReadBitstream(BitstreamReader *reader)
{
	bool bret;

	do {
		bret = reader->ReadBool(&m_bHasAltitude);
		if(!bret) break;
		if(m_bHasAltitude)
		{
			bret = reader->ReadDouble(&m_fAltitude);
			if(!bret) break;
		}
		bret = reader->ReadDouble(&m_fLatitude);
		if(!bret) break;
		bret = reader->ReadDouble(&m_fLongitude);
	}
	while(false);

	return bret;

}

void GeographicPointType::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_bHasAltitude)
	{
		*fout << " altitude=\"" << m_fAltitude << '"';
	}

	*fout << " latitude=\"" << m_fLatitude << '"';

	*fout << " longitude=\"" << m_fLongitude << '"';

	*fout << "/>" << std::endl;
}