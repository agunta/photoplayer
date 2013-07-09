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

// ppbim_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../src/collectionbim.h"
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/internal/EndOfEntityException.hpp>
#include <iostream>
#include <fstream>
#include <io.h>

#include <xercesc/dom/DOMError.hpp>
#include "ppdomerrorhandler.h"

//#define PPBIM_ITEM_TEST

// Define BIM_BYTE_0X1F to insert byte 0x1F at the beginning of bim stream.
// Comment this to get bit stream corresponding to SDL.

static char*                    gXmlFile               = 0;
static bool                     gDoNamespaces          = true;
static bool                     gDoSchema              = true;
static bool                     gSchemaFullChecking    = false;
static bool                     gDoCreate              = false;


//static XercesDOMParser::ValSchemes    gValScheme       = XercesDOMParser::Val_Auto;
static XercesDOMParser::ValSchemes    gValScheme       = XercesDOMParser::Val_Always;

bool testXML(char *buf, int len);
bool xml_to_bim(char *inFile, char *outFile);
bool bim_to_xml(char *inFile, char *outFile);

int main(int argc, char* argv[])
{

	bool bConverter = true;
	if(bConverter)
	{
		if(argc < 3)
		{
			printf("Usage:\n%s input_file output_file\n", argv[0]);
			return 0;
		}
		bool bXMLToBim = false;
		char *inFile = argv[1];
		char *outFile = argv[2];
		FILE *file1 = fopen(inFile, "rb");
		if(!file1)
		{
			printf("File open error: %s\n", inFile);
			return -1;
		}
		char fbuf[16];
		fbuf[0] = 0;
		fread(fbuf, 1, 16, file1);
		if(testXML(fbuf, 16))
		{
			bXMLToBim = true;
		}
		fclose(file1);

		if(bXMLToBim)
		{
			printf("Converting XML data to Bim\n", inFile);
			xml_to_bim(inFile, outFile);
		}
		else
		{
			printf("Converting Bim data to XML\n", inFile);
			bim_to_xml(inFile, outFile);
		}


		return 0;
	}




	// development testing
	if(argc > 1)
	{
		gXmlFile = argv[1];

	}
	else
	{
		return -1;
	}

	try
	{
		XMLPlatformUtils::Initialize();
	}
	//catch (const XMLException& toCatch)
	catch (...)
	{
		printf("XML Platform initialization failed\n");
		return -1;
	}
    // Instantiate the DOM parser.
	XercesDOMParser *parser = new XercesDOMParser();
    parser->setValidationScheme(gValScheme);
    parser->setDoNamespaces(gDoNamespaces);
    parser->setDoSchema(gDoSchema);
    parser->setValidationSchemaFullChecking(gSchemaFullChecking);
    parser->setCreateEntityReferenceNodes(gDoCreate);

    PPDOMErrorHandler errorHandler;  //= new DOMTreeErrorReporter();
    parser->setErrorHandler(&errorHandler);

	bool bError = false;
	try
    {
        parser->parse(gXmlFile);
    }
    catch (const OutOfMemoryException&)
    {
		bError = true;
	}
    catch (...)
    {
		bError = true;
	}
	if(errorHandler.getSawErrors())
	{
		bError = true;
	}
	if(!bError)
	{
		bool bresult = true;;
		//ColMpeg7 *mpegcol = new ColMpeg7();
		//ColMpeg7 *mpegcol2 = new ColMpeg7();
		ItemMpeg7 *mpegcol = new ItemMpeg7();
		ItemMpeg7 *mpegcol2 = new ItemMpeg7();
		do {
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *domdoc = parser->getDocument();
			if(!domdoc)
			{
				BimXMLUtil::ReportError("Missing or invalid XML document");
				break;
			}
			bresult = mpegcol->BuildFromXML((DOMNode*)domdoc->getDocumentElement());
			if(!bresult)
			{
				BimXMLUtil::ReportError("Building the Collection DOM tree failed");
				break;
			}
			int bufsize = mpegcol->GetNumberOfBits();
			bufsize = (bufsize + 7)/8;
			if(bufsize <= 0)
			{
				BimXMLUtil::ReportError("Calculation of the BiM buffer size failed");
				break;
			}
			unsigned char *buf;
			buf = (unsigned char*)malloc(bufsize);
			if(!buf)
			{
				BimXMLUtil::ReportError("Out of memory error");
				break;
			}
			BitstreamWriter *writer = new BitstreamWriter(bufsize, buf);
			bresult = mpegcol->WriteBitstream(writer);
			delete writer;
			if(!bresult)
			{
				BimXMLUtil::ReportError("BiM writing failed");
				free(buf);
				break;
			}
			BitstreamReader *reader = new BitstreamReader(buf, bufsize);
			bresult = mpegcol2->ReadBitstream(reader);
			delete reader;
			free(buf);
			if(!bresult)
			{
				BimXMLUtil::ReportError("BiM reading failed");
				break;
			}

			//std::ostream *fout = &std::cout;
			std::ostream *fout = new std::ofstream("fileout.txt");
			if(fout)
			{
				mpegcol2->PrintXML(fout, 0);
				fout->flush();
				delete fout;
			}
		}while(false);
		if(mpegcol) delete mpegcol;
		if(mpegcol2) delete mpegcol2;
	}

	delete parser;

	XMLPlatformUtils::Terminate();
	return 0;
}

bool testXML(char *buf, int len)
{
	if(buf == NULL) return false;
	int buflen = (int)strlen("<?xml");
	for(int i = 0; i < (len - buflen); i++)
	{
		if(strncmp("<?xml", buf + i, buflen) == 0)
		{
			return true;
		}
	}
	buflen = (int)strlen("<Mpeg7");
	for(int i = 0; i < (len - buflen); i++)
	{
		if(strncmp("<Mpeg7", buf + i, buflen) == 0)
		{
			return true;
		}
	}
	return false;
}


bool xml_to_bim(char *inFile, char *outFile)
{
	bool bret = true;
	try
	{
		XMLPlatformUtils::Initialize();
	}
	//catch (const XMLException& toCatch)
	catch (const XMLException& toCatch)
	{
		const XMLCh* msg = toCatch.getMessage();
		wprintf(L"Exception: %s\n", msg);
		return false;
	}
    // Instantiate the DOM parser.
	XercesDOMParser *parser = new XercesDOMParser();
    parser->setValidationScheme(gValScheme);
    parser->setDoNamespaces(gDoNamespaces);
    parser->setDoSchema(gDoSchema);
    parser->setValidationSchemaFullChecking(gSchemaFullChecking);
    parser->setCreateEntityReferenceNodes(gDoCreate);

    PPDOMErrorHandler errorHandler;  //= new DOMTreeErrorReporter();
    parser->setErrorHandler(&errorHandler);

	bool bError = false;
	try
    {
        parser->parse(inFile);
    }
    catch (const OutOfMemoryException&)
    {
		bError = true;
	}
    catch (...)
    {
		bError = true;
	}
	if(errorHandler.getSawErrors())
	{
		bError = true;
	}

	if(!bError)
	{
		bool bresult = true;;
#ifdef PPBIM_ITEM_TEST
		ItemMpeg7 *mpegcol = new ItemMpeg7();
#else
		ColMpeg7 *mpegcol = new ColMpeg7();
#endif
		do {
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *domdoc = parser->getDocument();
			if(!domdoc)
			{
				BimXMLUtil::ReportError("Missing or invalid XML document");
				bresult = false;
				break;
			}
			bresult = mpegcol->BuildFromXML((DOMNode*)domdoc->getDocumentElement());
			if(!bresult)
			{
				BimXMLUtil::ReportError("Building the Collection DOM tree failed");
				break;
			}
			int bufsize = mpegcol->GetNumberOfBits();

			bufsize = (bufsize + 7)/8;
			if(bufsize <= 0)
			{
				BimXMLUtil::ReportError("Calculation of the BiM buffer size failed");
				bresult = false;
				break;
			}
#ifdef BIM_BYTE_0X1F
			bufsize += 1;
#endif
			unsigned char *buf;
			buf = (unsigned char*)malloc(bufsize);
			if(!buf)
			{
				BimXMLUtil::ReportError("Out of memory error");
				bresult = false;
				break;
			}
			BitstreamWriter *writer = new BitstreamWriter(bufsize, buf);

#ifdef BIM_BYTE_0X1F
			bresult = BimXMLUtil::WriteDecodingMode(writer);
#endif

			if(bresult)
			{
				bresult = mpegcol->WriteBitstream(writer);
			}
			delete writer;
			if(!bresult)
			{
				BimXMLUtil::ReportError("BiM writing failed");
				break;
			}
			FILE *ofile = fopen(outFile, "wb");
			if(!ofile)
			{
				bresult = false;
				break;
			}
			fwrite(buf, bufsize, 1, ofile);
			fclose(ofile);
		}
		while(false);
		bret = bresult;

		if(mpegcol) delete mpegcol;
	}
	if(bError) bret = false;
	delete parser;

	XMLPlatformUtils::Terminate();
	return bret;
}

bool bim_to_xml(char *inFile, char *outFile)
{
	bool bresult = true;
#ifdef PPBIM_ITEM_TEST
		ItemMpeg7 *mpegcol = new ItemMpeg7();
#else
		ColMpeg7 *mpegcol = new ColMpeg7();
#endif
	BitstreamReader *reader = NULL;
	unsigned char *buf = NULL;;

	do
	{
		FILE *ifile = fopen(inFile, "rb");
		if(!ifile)
		{
			BimXMLUtil::ReportError("input file error");
			bresult = false;
			break;
		}
		int ifd = _fileno(ifile);
		int bufsize = _filelength( ifd );

		buf = (unsigned char*)malloc(bufsize);
		reader = new BitstreamReader(buf, bufsize);
		if(!buf || !mpegcol || !reader)
		{
			BimXMLUtil::ReportError("Out of memory error");
			fclose(ifile);
			bresult = false;
			break;
		}
		fread(buf, bufsize, 1, ifile);
		fclose(ifile);

#ifdef BIM_BYTE_0X1F
		bresult = BimXMLUtil::ReadDecodingMode(reader);
#endif

		if(bresult)
		{
			bresult = mpegcol->ReadBitstream(reader);
		}
	
		if(!bresult)
		{
			BimXMLUtil::ReportError("BiM reading failed");
			break;
		}

		//std::ostream *fout = &std::cout;
		std::ostream *fout = new std::ofstream(outFile);
		if(fout)
		{
			*fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
			mpegcol->PrintXML(fout, 0);
			fout->flush();
			delete fout;
		}
	}while(false);

	if(mpegcol) delete mpegcol;
	if(reader) delete reader;
	if(buf) delete buf;

	return bresult;
}
