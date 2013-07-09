///////////////////////////////////////////////////////////////////////////////
//
// This software module was originally developed by
//
// ETRI / Dongguk University, Seoul
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
// Applicable File Name:  EdgeHistExtraction.h
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

#ifndef __EDGEHISTEXTRACTION_H__
#define __EDGEHISTEXTRACTION_H__

//#include "BasicArchitecture/XMInterfaces.h"   /* Naming Convention */
//#include "BasicArchitecture/XMObjects.h"      /* Naming Convention */

//#include "Descriptors/Descriptors.h"
//#include "DescriptionSchemes/DescriptionSchemes.h"
//#include "Media/Media.h"

#define		Te_Define	11
#define		Desired_Num_of_Blocks	1100

namespace XM
{

typedef	struct Edge_Histogram_Descriptor{
  double Local_Edge[80]; 
} EHD;


//=============================================================================
class EdgeHistogramExtractionTool
{
public:
  // Null constructor
  EdgeHistogramExtractionTool();
  ~EdgeHistogramExtractionTool();

  // Don't know what this does
  bool SupportsPush(void);
  // Don't know what this does
  bool SupportsPull(void);

  // This informs the extractor where the source data comes from
  //virtual int SetSourceMedia(MultiMediaInterfaceABC* media);

  /*virtual EdgeHistogramDescriptorInterfaceABC*
    GetDescriptorInterface(void);
  virtual int SetDescriptorInterface(EdgeHistogramDescriptorInterfaceABC
				   * aEdgeHistogramDescriptorInterface);*/
	
  unsigned long InitExtracting(void);
  unsigned long StartExtracting(ImageIO *pImage);
  unsigned long PostExtracting(void);

  // Accesssor methods (KW: From EdgeHistogramDescriptor)
  virtual EHD*	GetEdgeHistogram();
  virtual char*	GetEdgeHistogramElement();
  virtual void	SetEdgeHistogram(EHD*	pEdge_Histogram);
  virtual void	SetEdgeHistogramElement(char*	pEdge_Histogram);

  // Extraction object must no be used, only its interface is allowd to
  // to be used. This function is to get the interface
  //virtual EdgeHistogramExtractionInterfaceABC *GetInterface(void);

private:

  unsigned long m_FrameCnt;
  int m_EdgeHistogramExtractionParameter;	
  
  unsigned long GetBlockSize(unsigned long image_width,
	unsigned long image_height, unsigned long desired_num_of_blocks);

  void EdgeHistogramGeneration(unsigned char* pImage_Y,
					     unsigned long image_width,
					     unsigned long image_height,
					     unsigned long block_size,
					     EHD* pLocal_Edge, int Te_Value);

  int GetEdgeFeature(unsigned char *pImage_Y,
				      int image_width, int block_size,
				      int Te_Value);

  // actual values: (KW: From EdgeHistogramDescriptor)
  EHD	*m_pEdge_Histogram;
  char	*m_pEdge_HistogramElement;

  static double QuantTable[5][8];

}; // End class

}; // End namespace


#endif //__EDGEHISTEXTRACTION_H__
