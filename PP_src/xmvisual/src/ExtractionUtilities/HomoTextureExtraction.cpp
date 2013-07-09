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
// Applicable File Name:  HomoTextureExtraction.cpp
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

#include <stdio.h>
#include <string.h> // KW 2007: for memcpy
#include "momusys.h"
#include "inter.h"
#include "intra.h"
#include "fifo.h"
//#include "Descriptors/Descriptors.h"                     /* Naming Convention */
#include "ExtractionUtilities/ExtractionUtilities.h"
//#include "Utilities/readparam.h"

#include "homo_texture_extractor.h"	//yjyu - 010217

//static int initextractionwascalled=0;


//=============================================================================
	

using namespace XM;

//=============================================================================
HomogeneousTextureExtractionTool::HomogeneousTextureExtractionTool()
{  
  m_FrameCnt=0;
  //SetExposedInterface(&m_Interface);
}

//=============================================================================
/*HomogeneousTextureExtractionTool::
HomogeneousTextureExtractionTool(HomogeneousTextureDescriptorInterfaceABC
			*aHomogeneousTexture):
m_Interface(this),
m_DescriptorInterface(NULL),
m_Media(NULL),
m_FrameCnt(0)

{
  if (!aHomogeneousTexture) {
    HomogeneousTextureDescriptor *descriptor =
      new HomogeneousTextureDescriptor;
    aHomogeneousTexture=descriptor->GetInterface();
  }

  SetDescriptorInterface(aHomogeneousTexture);

  SetExposedInterface(&m_Interface);
}
*/
//----------------------------------------------------------------------------
HomogeneousTextureExtractionTool::~HomogeneousTextureExtractionTool()
{
}

//----------------------------------------------------------------------------
bool HomogeneousTextureExtractionTool::SupportsPush(void)
{ return true; }
//----------------------------------------------------------------------------
bool HomogeneousTextureExtractionTool::SupportsPull(void)
{ return false; }

//----------------------------------------------------------------------------
// This initialises the extraction process. Bascially it should 
// reset all storage etc. However, as this hasn't got any storage
// it's a bit pointless.
unsigned long HomogeneousTextureExtractionTool::InitExtracting(void)

{
  return(0UL);
}

//----------------------------------------------------------------------------
// This uses the m_Media pointer to extract the relevant data and put it
// into the 
unsigned long HomogeneousTextureExtractionTool::StartExtracting(ImageIO *pImg)
{
	if(!pImg) return -1;
	MomVop *ImageMedia;
	static int oldsize=-1;

	/* initialize */

	ImageMedia = pImg->GetImage();
	if(ImageMedia==NULL) {
		fprintf(stderr, "Error: (DC_SE): GetImage failed\n");
		return (unsigned long)-1;
	}

	//---yjyu - 010218
	int HomogeneousTexture_feature[62];
	int flag,i,j;
	//---




	//---yjyu - 010217
	/////////// 2000.10.11 - yjyu@samsung.com  /* For the same size or larger than 128x128 */
	if(ImageMedia->height < 128 || ImageMedia->width < 128)
	{
		fprintf(stderr,"Image size shold be bigger than 128 x 128.\n");
		return (unsigned long)-1;
	}

	//HomogeneousTexture feature extraction
	CHomogeneousTexture_Extractor HomogeneousTexture_extractor;

	// 2000.10.11 - yjyu@samsung.com
	HomogeneousTexture_extractor.image_height = ImageMedia->height;
	HomogeneousTexture_extractor.image_width = ImageMedia->width;

	/*
	// KW: !!! test if this can be skipped in PP MAF
	if ((ImageMedia->a_chan) &&
	    (m_Media->GetMaxAlpha() > m_Media->GetMinAlpha()))
	{
		HomogeneousTexture_extractor.ArbitraryShape(ImageMedia->a_chan->data->u, ImageMedia->y_chan->data->u, ImageMedia->height, ImageMedia->width );
	}
	*/

	HomogeneousTexture_extractor.FeatureExtraction(ImageMedia->y_chan->data->u);

	//receive the parameter for layer -ychoi 010527
	//flag = atoi(getpara("layer", "1"));
	// KW: flag = 0 (Energy), flag = 1 (Energy + EnergyDeviation)
	flag = 1;

#ifdef DEBUG1
	if (flag == 0)
		fprintf(stderr,"Homogeneous Texture Feature with base layer, Extracted .. \n" );
	else
		fprintf(stderr,"Homogeneous Texture Feature with full layer, Extracted .. \n" );
#endif

	//saving 62 features & test show
	HomogeneousTexture_feature[0] = HomogeneousTexture_extractor.m_dc;
	HomogeneousTexture_feature[1] = HomogeneousTexture_extractor.m_std;
#ifdef DEBUG1
	fprintf(stderr,"%3d\t%3d\n",
		HomogeneousTexture_feature[0], HomogeneousTexture_feature[1]);
#endif

	for(i=0; i< 5; i++)
		for(j=0; j< 6; j++) {
			HomogeneousTexture_feature[i*6+j+2] = HomogeneousTexture_extractor.mean2[i][j];
#ifdef DEBUG1
		    fprintf(stderr,"%3d\t",
			    HomogeneousTexture_feature[i*6+j+2]);
#endif
		}
	for(i=0; i< 5; i++)
		for(j=0; j< 6; j++) {
			if (flag != 0) { 
				HomogeneousTexture_feature[i*6+j+30+2] = HomogeneousTexture_extractor.dev2[i][j];
#ifdef DEBUG1
				fprintf(stderr,"%3d\t",
				       HomogeneousTexture_feature[i*6+j+30+2]);
#endif
			}
			else
				HomogeneousTexture_feature[i*6+j+30+2] = 0;
		}

#ifdef DEBUG1
	fprintf(stderr, "\n\n");
#endif
	SetHomogeneousTextureFeature(flag, HomogeneousTexture_feature);	//yjyu - 010223

//---

  // All done
  return 0;
}

//----------------------------------------------------------------------------
// This initialises the extraction process. Bascially it should 
// reset all storage etc. However, as this hasn't got any storage
// it's a bit pointless.
unsigned long HomogeneousTextureExtractionTool::PostExtracting(void)

{
  return(0UL);
}

void HomogeneousTextureExtractionTool::
SetHomogeneousTextureFeature( int f, int* pHomogeneousTextureFeature ) 
{
	energydeviationflag = f;
	memcpy(feature, pHomogeneousTextureFeature, sizeof (feature));
}

//----------------------------------------------------------------------------
int* HomogeneousTextureExtractionTool::GetHomogeneousTextureFeature(void)
{
	if (feature ==NULL) return NULL;
	return feature;
}

//----------------------------------------------------------------------------
int HomogeneousTextureExtractionTool::GetHomogeneousTextureFeatureFlag(void)
{

	return energydeviationflag;
}