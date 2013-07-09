///////////////////////////////////////////////////////////////////////////////
//
// This software module was originally developed by
//
// TU-Munich, S.Herrmann
// Heinrich-Hertz-Institute (HHI), Bela Makai
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
// Applicable File Name:  ScalableColorExtraction.h
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

#ifndef __SCALCOLOREXTRACTION_H__
#define __SCALCOLOREXTRACTION_H__
//#include "BasicArchitecture/XMInterfaces.h"
//#include "BasicArchitecture/XMObjects.h"
#include "momusys.h"
#include "intra.h"
//#include "Media/Media.h"

//=============================================================================
//Macro for extraction process



namespace XM
{


       const static int scalableColorQuantValues[256][3] =
        {
                {217,9,255},{-71,9,255},{-27,8,127},{-54,9,255},{-8,7,63},{-14,7,63},{-22,7,63},{-29,8,127},
                {-6,6,31},{-13,7,63},{-11,6,31},{-22,7,63},{-9,7,63},{-14,7,63},{-19,7,63},{-22,7,63},
                {0,4,7},{-1,5,15},{0,3,3},{-2,6,31},{1,5,15},{-5,6,31},{0,5,15},{0,7,63},
                {2,5,15},{-2,6,31},{-2,5,15},{0,7,63},{3,5,15},{-5,6,31},{-1,6,31},{4,7,63},
                {0,3,3},{0,3,3},{0,3,3},{-1,5,15},{0,3,3},{0,3,3},{-1,5,15},{-2,5,15},
                {-1,5,15},{-1,4,7},{-1,5,15},{-3,5,15},{-1,5,15},{-2,5,15},{-4,5,15},{-5,5,15},
                {-1,5,15},{0,3,3},{-2,5,15},{-2,5,15},{-2,5,15},{-3,5,15},{-3,5,15},{0,5,15},
                {0,5,15},{0,5,15},{0,5,15},{2,5,15},{-1,5,15},{0,5,15},{3,6,31},{3,5,15},
                {0,2,1},{0,2,1},{0,3,3},{0,4,7},{0,2,1},{0,2,1},{0,3,3},{-1,4,7},
                {-1,4,7},{-1,4,7},{-2,5,15},{-1,5,15},{-2,5,15},{-2,5,15},{-2,5,15},{-1,5,15},
                {0,3,3},{0,2,1},{0,3,3},{-1,4,7},{0,2,1},{0,3,3},{-1,4,7},{-1,5,15},
                {-2,5,15},{-1,4,7},{-2,5,15},{-1,5,15},{-3,5,15},{-3,5,15},{-2,5,15},{0,5,15},
                {0,3,3},{0,3,3},{0,3,3},{-1,4,7},{0,3,3},{0,3,3},{-2,5,15},{-2,5,15},
                {-2,5,15},{-2,4,7},{-2,5,15},{-1,5,15},{-3,5,15},{-3,5,15},{-1,5,15},{0,5,15},
                {1,4,7},{0,3,3},{0,4,7},{-1,4,7},{0,3,3},{0,4,7},{-1,4,7},{0,4,7},
                {-1,4,7},{-1,3,3},{-1,4,7},{0,4,7},{-1,5,15},{0,5,15},{1,5,15},{-1,5,15},
                {0,2,1},{0,2,1},{0,3,3},{0,3,3},{0,2,1},{0,2,1},{0,3,3},{0,3,3},
                {0,2,1},{0,2,1},{0,3,3},{0,4,7},{0,2,1},{0,2,1},{0,3,3},{0,3,3},
                {0,3,3},{0,2,1},{0,3,3},{1,4,7},{0,2,1},{0,3,3},{-1,4,7},{1,4,7},
                {0,3,3},{0,3,3},{0,3,3},{0,4,7},{0,3,3},{0,3,3},{-1,4,7},{0,4,7},
                {0,3,3},{0,2,1},{0,3,3},{0,3,3},{0,2,1},{0,2,1},{0,3,3},{0,3,3},
                {0,3,3},{0,2,1},{0,3,3},{1,4,7},{0,2,1},{0,3,3},{0,4,7},{1,4,7},
                {0,3,3},{0,2,1},{0,3,3},{1,5,15},{0,3,3},{0,3,3},{-1,5,15},{2,5,15},
                {0,3,3},{0,3,3},{0,3,3},{0,4,7},{0,3,3},{0,3,3},{-1,4,7},{1,5,15},
                {0,3,3},{0,2,1},{0,3,3},{0,3,3},{0,2,1},{0,3,3},{0,4,7},{0,4,7},
                {0,3,3},{0,2,1},{0,3,3},{1,4,7},{0,3,3},{0,3,3},{-1,5,15},{1,5,15},
                {0,3,3},{0,2,1},{-1,3,3},{1,5,15},{0,3,3},{-1,4,7},{-1,5,15},{2,5,15},
                {0,3,3},{0,3,3},{0,3,3},{0,4,7},{0,3,3},{-1,3,3},{0,4,7},{1,4,7},
                {1,3,3},{0,2,1},{-1,3,3},{0,3,3},{0,3,3},{0,3,3},{0,3,3},{1,4,7},
                {0,3,3},{0,2,1},{-1,3,3},{0,4,7},{0,3,3},{0,3,3},{0,4,7},{1,4,7},
                {0,3,3},{0,2,1},{0,3,3},{0,4,7},{0,3,3},{-1,3,3},{0,4,7},{1,4,7},
                {0,3,3},{0,3,3},{0,3,3},{0,3,3},{0,3,3},{-1,3,3},{0,3,3},{-1,4,7}
        };

// Forward Declarations:
//class ScalableColorExtractionTool;

//=============================================================================
class ScalableColorExtractionTool
{
public:
	ScalableColorExtractionTool();
	~ScalableColorExtractionTool();

	bool SupportsPush(void);
	bool SupportsPull(void);

	//virtual int SetSourceMedia(MultiMediaInterfaceABC *media);

	unsigned long InitExtracting(void);
	unsigned long StartExtracting(ImageIO *pImage);
	unsigned long PostExtracting(void);

    unsigned long *GetScalableHistogram(void);
    unsigned long SetOutputNumOfCoeffs(unsigned long);
    unsigned long GetNumberOfCoefficients(void);
    unsigned long GetCoefficient(unsigned long index);
    unsigned long SetCoefficient(unsigned long index, int value);
    unsigned long SetCoeffValue(unsigned long index, int value);
    unsigned long GetCoeffSign(unsigned long index);
    unsigned long SetCoeffSign(unsigned long index, int value);
    unsigned long GetNumberOfBitplanesDiscarded();
    unsigned long SetNumberOfBitplanesDiscarded(unsigned long);

private:

    unsigned long SetNumberOfCoefficients(unsigned long);
	//MultiMediaInterfaceABC *m_Media;

	/*parts of StartExtracting for usage in GoFGoPColor*/
	unsigned long *ComputeNormalizedHistogram(MomVop *theVop);
	unsigned long *QuantizeHistogram(unsigned long *aHist);
	unsigned long *HaarTransform(unsigned long *aHist);

	unsigned long *m_ScalableHistogram;
	unsigned long *m_Sign;
	unsigned long m_NumberOfCoefficients;
	unsigned long m_OutputNumOfCoeffs;
	unsigned long m_NumberOfBitplanesDiscarded;
	
};

};

extern "C" void pixeltohistogram1(TPixel *res, TPixel *in,TCoor x, TCoor y);
extern "C" void pixeltocolorhist1(TPixel *res, TPixel *in,TCoor x, TCoor y);

#endif //__SCALCOLOREXTRACTION_H__
