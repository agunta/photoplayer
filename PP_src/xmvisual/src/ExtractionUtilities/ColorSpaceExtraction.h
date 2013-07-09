///////////////////////////////////////////////////////////////////////////////
//
// This software module was originally developed by
//
// TU-Munich, S.Herrmann
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
// Applicable File Name:  ColorSpaceExtraction.h
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

#ifndef __COLORSPACEEXTRACTION_H__
#define __COLORSPACEEXTRACTION_H__
//#include "BasicArchitecture/XMInterfaces.h"                                   /* Naming Convention */
//#include "BasicArchitecture/XMObjects.h"                                      /* Naming Convention */
#include <momusys.h>
#include <intra.h>

//=============================================================================

extern int *conversionmatrix;

extern "C" void YUV_To_HSV(TPixel *res, TPixel *in,TCoor x, TCoor y);
extern "C" void YUV_To_RGB(TPixel *res, TPixel *in,TCoor x, TCoor y);
extern "C" void RGB_To_YUV(TPixel *res, TPixel *in,TCoor x, TCoor y);
extern "C" void RGB_To_HSV(TPixel *res, TPixel *in,TCoor x, TCoor y);
extern "C" void LinearTransform(TPixel *res, TPixel *in,TCoor x, TCoor y,
				float *conversionmatrix);
extern "C" void InverseLinearTransform(TPixel *res, TPixel *in,TCoor x,
				       TCoor y,float *conversionmatrix);
extern "C" void HSV_To_RGB(TPixel *res, TPixel *in, TCoor x, TCoor y);
extern "C" void RGB_To_HMMD(TPixel *res, TPixel *in,TCoor x, TCoor y); //added by LG
extern "C" void HMMD_To_RGB(TPixel *res, TPixel *in, TCoor x, TCoor y); //added by LG
extern "C" void RGB_To_Monochrome(TPixel *res, TPixel *in,TCoor x, TCoor y);
extern "C" void NoConvert(TPixel *res, TPixel *in,TCoor x, TCoor y);
void matrix_inversion(float a[9],float m[9]);

/* define tools to convert images*/
#define  convertRGB_To_YUV(outvop,invop) \
	intra_proc(outvop,invop,0, \
	           ((outvop->a_chan) ? A_CH : 0)| \
		   Y_CH|U_CH|V_CH, \
		   ((invop->a_chan)? A_CH : 0)| \
		   Y_CH|U_CH|V_CH,CON_0,SCAN_HO, \
	           0,0,0,0,0, \
	           -1,-1,0,0,0,0, \
	           size1to1,&RGB_To_YUV);

#define  convertYUV_To_RGB(outvop,invop) \
	intra_proc(outvop,invop,0, \
	           ((outvop->a_chan)? A_CH : 0)| \
		   Y_CH|U_CH|V_CH, \
		   ((invop->a_chan)? A_CH : 0)| \
		   Y_CH|U_CH|V_CH,CON_0,SCAN_HO, \
	           0,0,0,0,0, \
	           -1,-1,0,0,0,0, \
	           size1to1,&YUV_To_RGB);

#endif //__COLORSPACEEXTRACTION_H__
