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

static char* g_chIndent = "  ";

/** Gets the value of a named attribute of a node.
  * @return the atrribute value or NULL if not exist.
  */

wchar_t* BimXMLUtil::GetAttributeData(DOMNode *node, const char *name)
{
	wchar_t *attrData = NULL;
	if(name != NULL && node->hasAttributes())
	{
		DOMNamedNodeMap *attr = node->getAttributes();
		int attrlen = attr->getLength();
		char *attrName = NULL;
		for(int i=0; i < attrlen; i++)
		{
			DOMNode *attrNode = attr->item(i);
			attrName = XMLString::transcode(attrNode->getNodeName()); 
			if(attrName != NULL && strcmp(attrName, name) == 0)
			{
				attrData = (wchar_t*)attrNode->getNodeValue();
				break;
			}
			if(attrName) XMLString::release(&attrName);
		}
		if(attrName) XMLString::release(&attrName);
	}
	return attrData;
}

DescString *BimXMLUtil::GetAttributeString(DOMNode *node, const char *name)
{
	DescString *pString = NULL;
	wchar_t *attr = BimXMLUtil::GetAttributeData(node, name);
	if(attr)
	{
		pString = new DescString();
		if(pString) pString->SetData(attr);
	}
	return pString;
}

DOMNode* BimXMLUtil::GetAttributeNode(DOMNode *node, const char *name)
{
	DOMNode *attrNode = NULL;
	if(name != NULL && node->hasAttributes())
	{
		DOMNamedNodeMap *attr = node->getAttributes();
		int attrlen = attr->getLength();
		char *attrName = NULL;
		for(int i=0; i < attrlen; i++)
		{
			DOMNode *nextNode = attr->item(i);
			attrName = XMLString::transcode(nextNode->getNodeName()); 
			if(attrName != NULL && strcmp(attrName, name) == 0)
			{
				attrNode = nextNode;
				break;
			}
			if(attrName) XMLString::release(&attrName);
		}
		if(attrName) XMLString::release(&attrName);
	}

	return attrNode;
}

wchar_t* BimXMLUtil::GetNodeText(DOMNode *node)
{
	wchar_t *outText = NULL;
	if(node->hasChildNodes())
	{
		DOMNodeList *list = node->getChildNodes();
		int listlen = list->getLength();
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			if(nextNode->getNodeType() == DOMNode::TEXT_NODE)
			{
				outText = (wchar_t*)nextNode->getNodeValue(); 
				break;
			}
		}
	}
	return outText;
}
	
DOMNode* BimXMLUtil::GetDOMElement(DOMNode *parent, const char *name)
{
	DOMNode *outNode = NULL;
	if(name != NULL && parent->hasChildNodes())
	{
		DOMNodeList *list = parent->getChildNodes();
		int listlen = list->getLength();
		char *nodeName = NULL;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, name) == 0)
			{
				outNode = nextNode;
				break;
			}
			if(nodeName) XMLString::release(&nodeName);
		}
		if(nodeName) XMLString::release(&nodeName);
	}

	return outNode;
}

bool BimXMLUtil::VerifyName(DOMNode *node, const char *name)
{
	bool bTheSame = false;
	if(node != NULL && name != NULL)
	{
		char *elemName = XMLString::transcode( node->getNodeName());
		if(strcmp(elemName, name) == 0)
		{
			bTheSame = true;
		}
		XMLString::release(&elemName);
	}
	return bTheSame;

}

int BimXMLUtil::CountDOMElements(DOMNode *parent, char *name)
{
	int nodeCnt = 0;
	if(name != NULL && parent->hasChildNodes())
	{
		DOMNodeList *list = parent->getChildNodes();
		int listlen = list->getLength();
		char *nodeName = NULL;
		for(int i=0; i < listlen; i++)
		{
			DOMNode *nextNode = list->item(i);
			nodeName = XMLString::transcode(nextNode->getNodeName()); 
			if(nodeName != NULL && strcmp(nodeName, name) == 0)
			{
				nodeCnt++;
			}
			if(nodeName) XMLString::release(&nodeName);
		}
	}
	return nodeCnt;
}


void BimXMLUtil::ReportError(char *msg)
{
	if(msg)
	{
		printf("Error: %s\n", msg);
	}
}

void BimXMLUtil::ReportWarning(char *msg)
{
	if(msg)
	{
		printf("Warning: %s\n", msg);
	}
}

void BimXMLUtil::PrintIndent(std::ostream *fout, int level)
{
	for(int i=0; i < level; i++)
	{
		*fout << g_chIndent;
	}
}

void BimXMLUtil::PrintStartElement(std::ostream *fout, char *element, bool bComplete)
{
	if(!element) return;
	*fout << '<' << element;
	if(bComplete)
	{
		*fout << '>';
	}
}

void BimXMLUtil::PrintEndElement(std::ostream *fout, char *element)
{
	if(!element) return;
	*fout << "</" << element << '>';
}


/** The number of bits of the value coded as vluimsbf5.
*/
int BimXMLUtil::GetVLI5BitSize(int value)
{
	// the value should be > 0
	assert(value >= 0);
	if(value < 0) return 0;
	int test_length = value;
	int num_chunks = 0;
	while(test_length){
		num_chunks++;
		test_length>>=4;
	}
	if(num_chunks == 0) num_chunks = 1;
	return (num_chunks * 4 + num_chunks);
}

bool BimXMLUtil::WriteOptionalDS(PPMpeg7Desc *pdesc, BitstreamWriter *writer)
{
	bool bOpt = (pdesc == NULL) ? false : true;
	bool bret = writer->WriteBool(bOpt);
	if(bret && bOpt)
	{
		bret = pdesc->WriteBitstream(writer);
	}
	return bret;
}

void BimXMLUtil::PrintXMLDS(PPMpeg7Desc *pdesc, std::ostream *fout, int nIndentLevel, char* elname, bool bComplete)
{
	if(pdesc && fout && elname)
	{
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintStartElement(fout, elname, bComplete);
		if(bComplete)
		{
			*fout << std::endl;
		}
		// Note: pdesc if not NULL should print new line at the end
		if(pdesc) pdesc->PrintXML(fout, nIndentLevel + 1);
		BimXMLUtil::PrintIndent(fout, nIndentLevel);
		BimXMLUtil::PrintEndElement(fout, elname);
		*fout << std::endl;
	}
}

int BimXMLUtil::ParseIntVector(wchar_t *strdata, int *pVector, int nSize)
{
	if(nSize == 0) return 0;
	if(!strdata || !pVector) return 0;

	int nRet = 0;
	wchar_t *pstr = strdata;
	int nLen = (int)wcslen(strdata);
	int nPos = 0;
	wchar_t cnum[32];
	int i = 0;
	for(i = 0; i< nSize; i++)
	{
		// skip spaces and line-ending characters
		while(*pstr == L' ' || *pstr == 10 || *pstr == 13)
		{
			nPos++;
			pstr++;
		}
		int nSign = 1;
		if(*pstr == L'+')
		{
			nPos++;
			pstr++;
		}
		else if(*pstr == L'-')
		{
			nSign = -1;
			nPos++;
			pstr++;
		}

		int nNumPos = 0;
		while(*pstr >= L'0' && *pstr <= L'9')
		{
			// this number is too big.
			if(nNumPos >= 31) return 0;;
			cnum[nNumPos++] = *pstr;
			nPos++;
			pstr++;
		}
		cnum[nNumPos] = 0;
		if(nNumPos > 0)
		{
			pVector[i] = _wtoi(cnum) * nSign;
			nRet++;
		}
		/*else
		{
			break;
		}*/
		if(nPos >= nLen) break;

		if(*pstr != L' ' && *pstr != 10 && *pstr != 13 && *pstr != 0)
		{
			// invalid character
			if(i < (nSize-1)) return 0;
		}
	}
	return nRet;
}

int BimXMLUtil::ParseIntVectorLength(wchar_t *strdata)
{
	if(!strdata) return false;

	wchar_t *pstr = strdata;
	int nLen = (int)wcslen(pstr);
	int nPos = 0;
	wchar_t cnum[32];
	int i = 0;
	while(true)
	{
		// skip spaces and line-ending characters
		while(*pstr == L' ' || *pstr == 10 || *pstr == 13)
		{
			nPos++;
			pstr++;
		}
		if(nPos >= nLen) break;

		if(*pstr == L'+')
		{
			nPos++;
			pstr++;
		}
		else if(*pstr == L'-')
		{
			nPos++;
			pstr++;
		}

		int nNumPos = 0;
		while(*pstr >= L'0' && *pstr <= L'9')
		{
			// this number is too big.
			if(nNumPos >= 31)
			{
				i = -1;
				break;
			}
			cnum[nNumPos++] = *pstr;
			nPos++;
			pstr++;
		}
		if(i < 0) break;
		cnum[nNumPos] = 0;
		if(nNumPos > 0)
		{
			i++;
		}
		else
		{
			i = -1;
			break;
		}
		if(nPos >= nLen) break;
		if(*pstr != L' ' && *pstr != 10 && *pstr != 13 && *pstr != 0)
		{
			// invalid character
			i = -1;
			break;
		}
	}
	return i;
}

bool BimXMLUtil::ParseElementInt(DOMNode *node, int *val)
{
	bool bret = true;
 	if(node)
	{
		wchar_t *data = BimXMLUtil::GetNodeText(node);
		if(data)
		{
			*val = _wtoi(data);
			//XMLString::release(&data);
		}
		else
		{
			bret = false;
		}
	}
	else
	{
		bret = false;
	}
	if(!bret) *val = 0;
	return bret;
}

int BimXMLUtil::CompareStrings(wchar_t *wstr1, wchar_t *wstr2, bool bTrimWhiteChars, bool bIgnoreCase)
{
	// to do: test the method
	// to do: bIgnoreCase is not implemented
	if(wstr1 == NULL || wstr2 == NULL) return -1;
	if(!bTrimWhiteChars && !bIgnoreCase)
	{
		return wcscmp(wstr1, wstr2);
	}
	int firstStart = 0;
	int firstEnd = (int)wcslen(wstr1) - 1;
	int secondStart = 0;
	int secondEnd = (int)wcslen(wstr2) - 1;
	if(firstStart >= firstEnd) return -1;
	if(secondStart >= secondEnd) return -1;
	for( ; firstStart < firstEnd;)
	{
		wchar_t ch1 = wstr1[firstStart];
		// skipping space new line chars and tab
		if( ch1 == L' ' || ch1 == 0x0D || ch1 == 0x0A || ch1 == 0x09)
		{
			firstStart++;
		}
		else
		{
			break;
		}
	}
	for( ; firstEnd > firstStart;)
	{
		wchar_t ch1 = wstr1[firstEnd];
		// skipping space new line chars and tab
		if( ch1 == L' ' || ch1 == 0x0D || ch1 == 0x0A || ch1 == 0x09)
		{
			firstEnd--;
		}
		else
		{
			break;
		}
	}
	for( ; secondStart < secondEnd;)
	{
		wchar_t ch1 = wstr2[secondStart];
		// skipping space new line chars and tab
		if( ch1 == L' ' || ch1 == 0x0D || ch1 == 0x0A || ch1 == 0x09)
		{
			secondStart++;
		}
		else
		{
			break;
		}
	}
	for( ; secondEnd > secondStart;)
	{
		wchar_t ch1 = wstr2[secondEnd];
		// skipping space new line chars and tab
		if( ch1 == L' ' || ch1 == 0x0D || ch1 == 0x0A || ch1 == 0x09)
		{
			secondEnd--;
		}
		else
		{
			break;
		}
	}
	if((firstEnd - firstStart) != (secondEnd - secondStart))
	{
		return -1;
	}
	for(; (firstStart <= firstEnd) &&  (secondStart <= secondEnd); )
	{
		if(wstr1[firstStart++] != wstr2[secondStart++]) return -1;
	}
	return 0;
}

bool BimXMLUtil::ReadDecodingMode(BitstreamReader *reader)
{
	bool bresult = true;
#ifdef BIM_BYTE_0X1F
	if(!reader) return false;
	int decoding_mode = 0;
	bresult = reader->ReadBits(&decoding_mode, 8);
	if(decoding_mode != 0x0000001F)
	{
		bresult = false;
	}
#endif
	return bresult;
}

bool BimXMLUtil::WriteDecodingMode(BitstreamWriter *writer)
{
	bool bresult = true;
#ifdef BIM_BYTE_0X1F
	if(!writer) return false;
	bresult = writer->WriteBits(0x0000001F, 8);
#endif
	return bresult;
}
