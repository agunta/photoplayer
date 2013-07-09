///////////////////////////////////////////////////////////////////////////////
//
// This software module was originally developed by
//
// NEC Corp., Akio Yamada, Eiji Kasutani
// (contributing organizations names)
//
// in the course of development of the MPEG-7 Experimentation Model.
//
// This software module is an implementation of a part of one or more MPEG-7
// Experimentation Model tools as specified by the MPEG-7 Requirements.
//
// ISO/IEC gives users of MPEG-7 free license to this software module or
// modifications thereof for use in hardware or software products claiming
// conformance to MPEG-7.
//
// Those intending to use this software module in hardware or software products
// are advised that its use may infringe existing patents. The original
// developer of this software module and his/her company, the subsequent
// editors and their companies, and ISO/IEC have no liability for use of this
// software module or modifications thereof in an implementation.
//
// Copyright is not released for non MPEG-7 conforming products. The
// organizations named above retain full right to use the code for their own
// purpose, assign or donate the code to a third party and inhibit third parties
// from using the code for non MPEG-7 conforming products.
//
// Copyright (c) 1998-1999.
//
// This notice must be included in all copies or derivative works.
//
// Applicable File Name:  ColorHistExtraction.h
//

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

#ifndef __COLOR_LAYOUT_EXTRACTION_H__
#define __COLOR_LAYOUT_EXTRACTION_H__
//#include "BasicArchitecture/XMInterfaces.h"                                   /* Naming Convention */
//#include "BasicArchitecture/XMObjects.h"                                      /* Naming Convention */

//#include "Descriptors/Descriptors.h"
//#include "DescriptionSchemes/DescriptionSchemes.h"
//#include "Media/Media.h"

namespace XM
{


// Forward Declarations:
//class ColorLayoutExtractionTool;

//=============================================================================
class ColorLayoutExtractionTool
{
public:
	// Null constructor
	ColorLayoutExtractionTool();
	~ColorLayoutExtractionTool();

	bool SupportsPush(void);
	bool SupportsPull(void);

	//virtual int SetSourceMedia(MultiMediaInterfaceABC *media);

/*	virtual unsigned long ExtractFeature(ColorLayoutDescriptor* aDesc,
	             I_ExtractionInterfaceABC::ExtractionResult& aResult);*/

        // start modification for version 3.0
	unsigned long InitExtracting(void);
        // end modification for version 3.0
	unsigned long StartExtracting(int NumberOfYCoeff, int NumberOfCCoeff, ImageIO *pImg);
	unsigned long StartExtracting( ImageIO *pImg){ return StartExtracting(6, 3, pImg); };
	unsigned long PostExtracting(void);

	// KW: retrieving the description data
	int GetNumberOfYCoeff() { return m_NumberOfYCoeff; };
	int GetNumberOfCCoeff() { return m_NumberOfYCoeff; };
	int* GetYCoeff() { return m_YCoeff; };
	int* GetCbCoeff() { return m_CbCoeff; };
	int* GetCrCoeff() { return m_CrCoeff; };

/*	"C" void pixeltocolorhist(TPixel *res, TPixel *in,TCoor x, TCoor y);*/

private:
        // start modification for version 3.0
	//virtual ~ColorLayoutExtractionTool();
        // end modification for version 3.0

	//MultiMediaInterfaceABC* m_Media;
	unsigned long m_FrameCnt;

	// helper functions and variables
	double c[8][8]; /* transform coefficients */
	unsigned char zigzag_scan[64]; /* Zig-Zag scan pattern  */
	void CreateSmallImage(MomVop *src, short small_img[3][64]);
	void init_fdct();
	inline void fdct(short *block);
 	inline int quant_ydc(int i);
	inline int quant_cdc(int i);
	inline int quant_ac(int i);
	int m_NumberOfYCoeff;
	int m_NumberOfCCoeff;

    int m_YCoeff[64];
	int m_CbCoeff[64];
	int m_CrCoeff[64];
};

};

//extern "C" void pixeltocolorhist(TPixel *res, TPixel *in,TCoor x, TCoor y);


#endif //__COLOR_LAYOUT_EXTRACTION_H__
