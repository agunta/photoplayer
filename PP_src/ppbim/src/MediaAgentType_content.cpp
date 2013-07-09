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

MediaAgentType_content::MediaAgentType_content()
{
	m_pRole = NULL;
	m_pAgent = NULL;
	m_pOrganization = NULL;
	m_pPerson = NULL;
}

MediaAgentType_content::~MediaAgentType_content()
{
	if(m_pRole) delete m_pRole;
	if(m_pAgent) delete m_pAgent;
	if(m_pOrganization) delete m_pOrganization;
	if(m_pPerson) delete m_pPerson;
}

bool MediaAgentType_content::BuildFromXML(DOMNode *node)
{
	if(node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE)
	{
		return false;
	}
	DOMNode *childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_ROLE);
	if(childNode)
	{
		m_pRole = new ControlledTermUseType();
		if(!m_pRole || !m_pRole->BuildFromXML(childNode))
		{
			if(m_pRole)
			{
				delete m_pRole;
				m_pRole = NULL;
			}
			return false;
		}
	}

	childNode = BimXMLUtil::GetDOMElement(node, ELEM_NAME_AGENT);
	if(childNode)
	{
		wchar_t *attr = BimXMLUtil::GetAttributeData(childNode, ATTR_NAME_XSI_TYPE);
		if(!attr)
		{
			// AgentType is abstract?
			m_pAgent = new AgentType();
			RETURN_IFNULL(m_pAgent);
			return true;
		}

		int typechoice = -1;
		if(wcscmp(attr, ELEM_UNINAME_ORGANIZATION_TYPE) == 0)
		{
			typechoice = 0;
		}
		else if(wcscmp(attr, ELEM_UNINAME_PERSON_TYPE) == 0)
		{
			typechoice = 1;
		}
		if(typechoice >= 0)
		{
			if(typechoice == 0)
			{
				m_pOrganization = new OrganizationType();
				if(!m_pOrganization->BuildFromXML(childNode))
				{
					if(m_pOrganization)
					{
						delete m_pOrganization;
						m_pOrganization = NULL;
					}
					return false;
				}
			}
			else if(typechoice == 1)
			{
				m_pPerson = new PersonType();
				if(!m_pPerson || !m_pPerson->BuildFromXML(childNode))
				{
					if(m_pPerson)
					{
						delete m_pPerson;
						m_pPerson = NULL;
					}
					return false;
				}
			}
		}
	}
	return true;
}

int MediaAgentType_content::GetNumberOfBits()
{
	int bitCount = 0;
	if(m_pRole)
	{
		bitCount += m_pRole->GetNumberOfBits();
	}
	bitCount++;
	if(m_pOrganization || m_pPerson)
	{
		bitCount++;
		if(m_pOrganization)
		{
			bitCount += m_pOrganization->GetNumberOfBits();
		}
		else if(m_pPerson)
		{
			bitCount += m_pPerson->GetNumberOfBits();
		}
	}
	else if(m_pAgent)
	{
		bitCount += m_pAgent->GetNumberOfBits();
	}
	return bitCount;
}

bool MediaAgentType_content::Validate()
{
	return true;
}

bool MediaAgentType_content::WriteBitstream(BitstreamWriter *writer)
{
	bool bret;
	RETURN_IFNULL(m_pRole);
	bret = m_pRole->WriteBitstream(writer);
	bool bTypeCast = (m_pPerson == NULL && m_pOrganization == NULL) ? false : true;
	if(bret)
	{
		bret = writer->WriteBool(bTypeCast);
	}
	if(bret && bTypeCast)
	{
		int nChoice = (m_pOrganization != NULL) ? 0 : 1;
		bret = writer->WriteBits(nChoice, 1);
		if(nChoice == 0)
		{
			bret = m_pOrganization->WriteBitstream(writer);
		}
		else
		{
			bret = m_pPerson->WriteBitstream(writer);
		}
	}
	else if(bret)
	{
		if(m_pAgent)
		{
			bret = m_pAgent->WriteBitstream(writer);
		}
		else
		{
			bret = false;
		}
	}
	return bret;
}

bool MediaAgentType_content::ReadBitstream(BitstreamReader *reader)
{
	SAFE_DELETE(m_pRole);
	SAFE_DELETE(m_pOrganization);
	SAFE_DELETE(m_pPerson);
	SAFE_DELETE(m_pAgent);
	
	bool bret = true;

	do {
		m_pRole = new ControlledTermUseType();
		RETURN_IFNULL(m_pRole);
		bret = m_pRole->ReadBitstream(reader);
		if(!bret) break;
		bool bTypeCast;
		bret = reader->ReadBool(&bTypeCast);
		if(!bret) break;
		if(bTypeCast)
		{
			int nChoice;
			bret = reader->ReadBits(&nChoice, 1);
			if(nChoice == 0)
			{
				m_pOrganization = new OrganizationType();
				RETURN_IFNULL(m_pOrganization);
				bret = m_pOrganization->ReadBitstream(reader);
			}
			else
			{
				m_pPerson = new PersonType();
				RETURN_IFNULL(m_pPerson);
				bret = m_pPerson->ReadBitstream(reader);
			}
		}
		else
		{
			m_pAgent = new AgentType();
			RETURN_IFNULL(m_pAgent);
			bret = m_pAgent->ReadBitstream(reader);
		}
	}
	while(false);

	return bret;
}

void MediaAgentType_content::PrintXML(std::ostream *fout, int nIndentLevel)
{
	if(!fout) return;

	if(m_pRole)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, ELEM_NAME_ROLE, false);
		m_pRole->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, ELEM_NAME_ROLE);
		*fout << std::endl;
	}
	if(!m_pRole && !m_pOrganization & !m_pPerson) return;
	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintStartElement(fout, ELEM_NAME_AGENT, false);
	if(m_pOrganization)
	{
		*fout << " xsi:type=\"OrganizationType\"";
		m_pOrganization->PrintXML(fout, nIndentLevel + 1);
	}
	else if(m_pPerson)
	{
		*fout << " xsi:type=\"PersonType\"";
		m_pPerson->PrintXML(fout, nIndentLevel + 1);
	}
	else if(m_pAgent)
	{
		m_pAgent->PrintXML(fout, nIndentLevel + 1);
	}
	BimXMLUtil::PrintIndent(fout, nIndentLevel);
	BimXMLUtil::PrintEndElement(fout, ELEM_NAME_AGENT);
	*fout << std::endl;
}
