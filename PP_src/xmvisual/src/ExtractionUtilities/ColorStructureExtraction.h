////////////////////////////////////////////////////////////////////////
//
// ColorStructureExtraction.h
//
// This software module was originally developed by
//
// Jim Errico, Sharp Laboratories of America, Camas, WA
// Peter van Beek, Sharp Laboratories of America, Camas, WA
// 
//
// in the course of development of the MPEG-7 standard (ISO/IEC 15938). 
// This software module is an implementation of a part of one or more 
// MPEG-7 tools as specified by the MPEG-7 standard (ISO/IEC 15938).
// ISO/IEC gives users of the MPEG-7 standard free license to this 
// software module or modifications thereof for use in hardware or 
// software products claiming conformance to the MPEG-7 standard. 
// Those intending to use this software module in hardware or software 
// products are advised that its use may infringe existing patents. 
// The original developer of this software module and his/her company, 
// the subsequent editors and their companies, and ISO/IEC have no 
// liability for use of this software module or modifications thereof 
// in an application. No license to this software is granted for non 
// MPEG-7 conforming products. 
// Sharp Laboratories of America retains full right to use the software
// module for their own purpose, assign or donate the software module 
// to a third party and to inhibit third parties from using the software 
// module for non MPEG-7 conforming products. 
//
// Copyright (c) 2000
//
// This copyright notice must be included in all copies or derivative 
// works of this software module.
//
////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Parts of this software module were developed by
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
// infringe existing patents. The developer of this software module and
// his/her company, the subsequent editors and their companies, and
// ISO/IEC have no liability for use of this software module or
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

#ifndef __COLORSTRUCTUREEXTRACTION_H__
#define __COLORSTRUCTUREEXTRACTION_H__

//#include "BasicArchitecture/XMInterfaces.h"   /* Naming Convention */
//#include "BasicArchitecture/XMObjects.h"      /* Naming Convention */

//#include "Descriptors/Descriptors.h"
//#include "DescriptionSchemes/DescriptionSchemes.h"
//#include "Media/Media.h"

namespace XM
{

//=============================================================================
class ColorStructureExtractionTool
{
public:
	// Null constructor
	ColorStructureExtractionTool();
	virtual ~ColorStructureExtractionTool();

	// Don't know what this does
	virtual bool SupportsPush(void);
	// Don't know what this does
	virtual bool SupportsPull(void);

	virtual unsigned long InitExtracting(void);
	virtual unsigned long StartExtracting(int tsize, ImageIO *pImage);
	virtual unsigned long PostExtracting(void);

	virtual int DownQuantHMMD(int nNewSize);

	static int BuildColorQuantTable(int iColorQuantSpace = 3);

	unsigned long GetSize(void);
	unsigned long SetSize(unsigned long size);
	unsigned long GetElement(unsigned long index);
	unsigned long SetElement(unsigned long index, int value);

private:

	static int GetColorQuantSpace(int size);
	static int GetBinSize(int iColorQuantSpace);

	// Single pixel/bin operations
	static void RGB2HMMD(int R, int G, int B, int &H, int &S, int &D);
	static int QuantHMMD(int H, int S, int D, int iColorQuantSpace);
	static int TransformBinIndex(int iOrig, int iOrigColorQuantSpace, 
											int iNewColorQuantSpace);

	// Full descriptor operations
	int QuantAmplNonLinear(unsigned long OldNorm);
	int ConvertAmplLinear(unsigned long NewNorm);
	int UnifyBins(unsigned long Norm, int nNewSize);

	// descriptor values and data size (copied from Color Structure Descriptor)
	unsigned long *m_Data;
    unsigned long m_Size;

	// Three ways to transform color quant spaces:
	// 1) hard-coded transform table (define PRINT_TRANSFORM_TABLE to generate)
	// 2) build transform table from colorspace table dynamically
	// 3) build transform table using QuantHMMD() repeatedly (very inefficient)
	static int BuildTransformTable(int iOrigColorQuantSpace, int nNewSize);

	enum { NUM_COLOR_QUANT_SPACE = 4, 
				 MAX_SUB_SPACE = 5, 
				 BASE_QUANT_SPACE = 256,
				 BASE_QUANT_SPACE_INDEX = 3 };

	static const int diffThresh[NUM_COLOR_QUANT_SPACE][MAX_SUB_SPACE+1];
	static const int nHueLevels[NUM_COLOR_QUANT_SPACE][MAX_SUB_SPACE];
	static const int nSumLevels[NUM_COLOR_QUANT_SPACE][MAX_SUB_SPACE];
	static const int nCumLevels[NUM_COLOR_QUANT_SPACE][MAX_SUB_SPACE];
	static unsigned char *colorQuantTable[NUM_COLOR_QUANT_SPACE];
	static unsigned char *colorQuantTransform[NUM_COLOR_QUANT_SPACE]
											 [NUM_COLOR_QUANT_SPACE];

	static const double amplThresh[];
	static const int nAmplLevels[];

}; // End class

}; // End namespace


#endif //__COLORSTRUCTUREEXTRACTION_H__
