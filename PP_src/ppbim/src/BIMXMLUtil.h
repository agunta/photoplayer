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

class DescString;

class BimXMLUtil
{
public:
	static wchar_t* GetAttributeData(DOMNode *node, const char *name);
	
	static DOMNode* GetAttributeNode(DOMNode *node, const char *name);

	static DescString *GetAttributeString(DOMNode *node, const char *name);

	static wchar_t* GetNodeText(DOMNode *node);

	static DOMNode* GetDOMElement(DOMNode *parent, const char *name);

	/* Checks if the given node has the specific name */
	static bool VerifyName(DOMNode *node, const char *name);

	/** Gets the number of elements with given name contained in the given node. */
	static int CountDOMElements(DOMNode *parent, char *name);

	static void ReportError(char *msg);

	static void ReportWarning(char *msg);

	static void PrintIndent(std::ostream *fout, int level);

	static void PrintStartElement(std::ostream *fout, char *element, bool bComplete = true);

	static void PrintEndElement(std::ostream *fout, char *element);

	static int GetVLI5BitSize(int value);

	static bool WriteOptionalDS(PPMpeg7Desc *pdesc, BitstreamWriter *writer);

	/** Prints the XML form of pdesc to fout using indent nIndentLevel, if DS contains any
	  * attributes bComplete should be set to false.
	  */
	static void PrintXMLDS(PPMpeg7Desc *pdesc, std::ostream *fout, int nIndentLevel, char *name, bool bComplete);

	static int ParseIntVector(wchar_t *strdata, int *pVector, int nSize);

	static int ParseIntVectorLength(wchar_t *strdata);

	static bool ParseElementInt(DOMNode *node, int *val);

	/** Compares two wide character strings.
	  * @param wstr1 the first input string
	  * @param wstr2 the second input string
	  * @param bTrimWhiteChars if true white characters are trimmed at beginning
	  *  and end of the stream (default true)
	  * @param bIgnoreCase if true the ignore case comparison is performed (default false)
	  * @return the value 0 is returned if strings are the same.
	  */
	static int CompareStrings(wchar_t *wstr1, wchar_t *wstr2, bool bTrimWhiteChars = true, bool bIgnoreCase = false);

	static bool ReadDecodingMode(BitstreamReader *reader);

	static bool WriteDecodingMode(BitstreamWriter *writer);
};
