///////////////////////////////////////////////////////////////////////////////
//
// This software module was originally developed by
//
// ICU(Information Communication University), Yong-Ju Jung,Ki Won Yoo,Yong Man Ro.
// ETRI, Mun Churl Kim
// Samsung, Yun Ju Yu, Yang Lim Choi.
// in cooperation with UCSB, Hyundai, HHI.
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
// purpose, assign or donate the code to a third party and inhibit third
// parties from using the code for non MPEG-7 conforming products.
//
// Copyright (c) 1998-2001.
//
// This notice must be included in all copies or derivative works.
//
// Applicable File Name:  HomoTextureExtraction.h
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
// Copyright � 2007

#ifndef _HOMOTEXTURE_EXTRACTION_H_
#define _HOMOTEXTURE_EXTRACTION_H_

//#include "BasicArchitecture/XMInterfaces.h"   /* Naming Convention */
//#include "BasicArchitecture/XMObjects.h"      /* Naming Convention */

//#include "Descriptors/Descriptors.h"
//#include "DescriptionSchemes/DescriptionSchemes.h"
//#include "Media/Media.h"

namespace XM
{

//=============================================================================
class HomogeneousTextureExtractionTool
{
public:
  // Null constructor
  HomogeneousTextureExtractionTool();
  ~HomogeneousTextureExtractionTool();

  // Also connects the Descriptor (result memnory) to the extraction
  // If set to "0" it automatically creates the descriptor
  //HomogeneousTextureExtractionTool(HomogeneousTextureDescriptorInterfaceABC
	//		  *HomogeneousTexture);

  // Don't know what this does
  bool SupportsPush(void);
  // Don't know what this does
  bool SupportsPull(void);

  unsigned long InitExtracting(void);
  unsigned long StartExtracting(ImageIO *pImg);
  unsigned long PostExtracting(void);
  
  // From HomogeneousTextureDescriptor
  void SetHomogeneousTextureFeature(int flag, int* HomogeneousTextureFeature);
  int* GetHomogeneousTextureFeature();
  int GetHomogeneousTextureFeatureFlag();


private:

  unsigned long m_FrameCnt;

  // From HomogeneousTextureDescriptor
  // This is the actual data the D/DSType stores.
  int energydeviationflag;
  int feature[62];


}; // End class

}; // End namespace


#endif //_HOMOTEXTURE_EXTRACTION_H_
