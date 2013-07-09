//////////////////////////////////////////////////////////////////////////////
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
// Applicable File Name:  EdgeHistExtraction.cpp
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
#include <math.h>
#include <memory.h>
#include "momusys.h"
#include "inter.h"
#include "intra.h"
#include "fifo.h"
//#include "Descriptors/Descriptors.h"                     /* Naming Convention */
#include "ExtractionUtilities/ExtractionUtilities.h"
//#include "Utilities/readparam.h"

#define	NoEdge				0
#define	vertical_edge			1
#define	horizontal_edge			2
#define	non_directional_edge		3
#define	diagonal_45_degree_edge		4
#define	diagonal_135_degree_edge	5


//=============================================================================
	

using namespace XM;


//=============================================================================
// Quantization quantization tables
double EdgeHistogramExtractionTool::QuantTable[5][8] = { 
  {0.010867,0.057915,0.099526,0.144849,0.195573,0.260504,0.358031,0.530128}, 
  {0.012266,0.069934,0.125879,0.182307,0.243396,0.314563,0.411728,0.564319},
  {0.004193,0.025852,0.046860,0.068519,0.093286,0.123490,0.161505,0.228960},
  {0.004174,0.025924,0.046232,0.067163,0.089655,0.115391,0.151904,0.217745},
  {0.006778,0.051667,0.108650,0.166257,0.224226,0.285691,0.356375,0.450972},
};

//=============================================================================
EdgeHistogramExtractionTool::EdgeHistogramExtractionTool()
{
  m_FrameCnt=0;
  m_pEdge_Histogram=NULL;
  m_pEdge_HistogramElement=NULL;
  m_pEdge_Histogram = new EHD[1];
  m_pEdge_HistogramElement = new char[80];
}

//----------------------------------------------------------------------------
EdgeHistogramExtractionTool::~EdgeHistogramExtractionTool()
{
	if(m_pEdge_Histogram) delete m_pEdge_Histogram;
	if(m_pEdge_HistogramElement) delete m_pEdge_HistogramElement;
}

//----------------------------------------------------------------------------
bool EdgeHistogramExtractionTool::SupportsPush(void)
{ return true; }
//----------------------------------------------------------------------------
bool EdgeHistogramExtractionTool::SupportsPull(void)
{ return false; }

//----------------------------------------------------------------------------
// This informs the extractor where the source data is coming from. This
// could be an audio file, image stream etc. Here it's just a filename.
/*int EdgeHistogramExtractionTool::SetSourceMedia(MultiMediaInterfaceABC* media)
{
  if(media==NULL) return -1;
  m_Media=media;
  return 0;
}
*/

//----------------------------------------------------------------------------
// This initialises the extraction process. Bascially it should 
// reset all storage etc. However, as this hasn't got any storage
// it's a bit pointless.
unsigned long EdgeHistogramExtractionTool::InitExtracting(void)

{
  return(0UL);
}

//----------------------------------------------------------------------------
// This uses the m_Media pointer to extract the relevant data and put it
// into the 
unsigned long EdgeHistogramExtractionTool::StartExtracting(ImageIO *pImage)

{
 	MomVop *ImageMedia;
	if (pImage==NULL) return (unsigned long)-1;
	ImageMedia = pImage->GetImage();
	if (ImageMedia==NULL) return (unsigned long)-1;

  
	unsigned long	desired_num_of_blocks;
	unsigned long	block_size;
	int		Te_Value;
	EHD		*pLocal_Edge;
	pLocal_Edge = NULL;
	pLocal_Edge = new	EHD[1];

	Te_Value = Te_Define;
	desired_num_of_blocks = Desired_Num_of_Blocks;

// Making Gray Image
	int i, j, xsize, ysize;
	unsigned char	*pGrayImage;
// arbitrary shape (Modified by Dongguk)
	unsigned char *pResampleImage=NULL;
	int max_x = 0, max_y = 0, min_x = ImageMedia->width-1, min_y = ImageMedia->height-1;
	double scale, EWweight, NSweight, EWtop, EWbottom;
	unsigned char NW, NE, SW, SE;
	int min_size, re_xsize, re_ysize;
	if(ImageMedia->a_chan){
		for(j=0; j<ImageMedia->height; j++){
			for(i=0; i<ImageMedia->width; i++){
				if(ImageMedia->a_chan->data->u[j*ImageMedia->width+i]){
					if(max_x<i) max_x = i;
					if(max_y<j) max_y = j;
					if(min_x>i) min_x = i;
					if(min_y>j) min_y = j;
				}
			}
		}
		xsize = max_x - min_x + 1;
		ysize = max_y - min_y + 1;
	}
	else{
		xsize = ImageMedia->width;
		ysize = ImageMedia->height;
		min_x = min_y = 0;
	}
	pGrayImage = new	unsigned char[xsize*ysize];
	for( j=0; j < ysize; j++){
		for( i=0; i < xsize; i++) {
			if(ImageMedia->a_chan){
				if(ImageMedia->a_chan->data->u[(j+min_y)*ImageMedia->width+(i+min_x)]){
					pGrayImage[j*xsize+i]=(unsigned char)((float)(ImageMedia->y_chan->data->u[(j+min_y)*ImageMedia->width+(i+min_x)]
						+ImageMedia->u_chan->data->u[(j+min_y)*ImageMedia->width+(i+min_x)]
						+ImageMedia->v_chan->data->u[(j+min_y)*ImageMedia->width+(i+min_x)])/3.0f);
				}
				else{
					pGrayImage[j*xsize+i] = 0;
				}
			}
			else{
				pGrayImage[j*xsize+i]=(unsigned char)((float)(ImageMedia->y_chan->data->u[j*xsize+i]
					+ImageMedia->u_chan->data->u[j*xsize+i]
					+ImageMedia->v_chan->data->u[j*xsize+i])/3.0f);
			}
		}
	}
	min_size = (xsize>ysize)? ysize: xsize;
	if(min_size<70){
		//upsampling
		scale = 70.0/min_size;
		re_xsize = (int)(xsize*scale+0.5);
		re_ysize = (int)(ysize*scale+0.5);
		pResampleImage = new unsigned char[re_xsize*re_ysize];
		for(j=0; j<re_ysize; j++)for(i=0; i<re_xsize; i++){
			EWweight = i/scale-floor(i/scale);
			NSweight = j/scale-floor(j/scale);
			NW = pGrayImage[(int)floor(i/scale)+(int)floor(j/scale)*xsize];
			NE = pGrayImage[(int)floor(i/scale)+1+(int)floor(j/scale)*xsize];
			SW = pGrayImage[(int)floor(i/scale)+(int)(floor(j/scale)+1)*xsize];
			SE = pGrayImage[(int)floor(i/scale)+1+(int)(floor(j/scale)+1)*xsize];
			EWtop = NW + EWweight*(NE-NW);
			EWbottom = SW + EWweight*(SE-SW);
			pResampleImage[i+j*re_xsize] = (unsigned char)(EWtop + NSweight*(EWbottom-EWtop)+0.5);
		}
		block_size = GetBlockSize(re_xsize, re_ysize, desired_num_of_blocks);
		if(block_size<2)
			block_size = 2;
		EdgeHistogramGeneration(pResampleImage, re_xsize, re_ysize, block_size, pLocal_Edge, Te_Value);
		delete  [] pResampleImage;
	}
	else{
		block_size = GetBlockSize(xsize, ysize, desired_num_of_blocks);
		if(block_size<2)
			block_size = 2;
		EdgeHistogramGeneration(pGrayImage, xsize, ysize, block_size, pLocal_Edge, Te_Value);
	}
// end modification
	SetEdgeHistogram( pLocal_Edge );
	
	delete	[] pLocal_Edge;
	delete	[] pGrayImage;
	return 0;
}

//----------------------------------------------------------------------------
unsigned long EdgeHistogramExtractionTool::GetBlockSize(unsigned long image_width, unsigned long image_height, unsigned long desired_num_of_blocks)
{
	unsigned long	block_size;
	double		temp_size;
	
	temp_size = (double) sqrt((double)(image_width*image_height/desired_num_of_blocks));
	block_size = (unsigned long) (temp_size/2)*2;
	
	return block_size;
}

//----------------------------------------------------------------------------
void EdgeHistogramExtractionTool::
EdgeHistogramGeneration(unsigned char* pImage_Y, unsigned long image_width,
			unsigned long image_height, unsigned long block_size,
			EHD* pLocal_Edge, int Te_Value)
{
	int Count_Local[16],sub_local_index;
	int Offset, EdgeTypeOfBlock;
	unsigned int i, j;
	long	LongTyp_Local_Edge[80];

	// Clear
	memset(Count_Local, 0, 16*sizeof(int));		
	memset(LongTyp_Local_Edge, 0, 80*sizeof(long));
	
	for(j=0; j<=image_height-block_size; j+=block_size)
	  for(i=0; i<=image_width-block_size; i+=block_size){
	    sub_local_index = (int)(i*4/image_width)+(int)(j*4/image_height)*4;
	    Count_Local[sub_local_index]++;
		
	    Offset = image_width*j+i;

	    EdgeTypeOfBlock = GetEdgeFeature(pImage_Y+Offset, image_width,
					     block_size, Te_Value);
	    switch(EdgeTypeOfBlock) {
	    case NoEdge:
	      break;
	    case vertical_edge:
	      LongTyp_Local_Edge[sub_local_index*5]++;
	      break;
	    case horizontal_edge:
	      LongTyp_Local_Edge[sub_local_index*5+1]++;
	      break;
	    case diagonal_45_degree_edge:
	      LongTyp_Local_Edge[sub_local_index*5+2]++;
	      break;
	    case diagonal_135_degree_edge:
	      LongTyp_Local_Edge[sub_local_index*5+3]++;
	      break;
	    case non_directional_edge:
	      LongTyp_Local_Edge[sub_local_index*5+4]++;
	      break;
	    } //switch(EdgeTypeOfBlock)
	  } // for( i )

	for( i=0; i<80; i++) {			// Range 0.0 ~ 1.0
	  sub_local_index = (int)(i/5);
	  pLocal_Edge->Local_Edge[i] =
	    (double)LongTyp_Local_Edge[i]/Count_Local[sub_local_index];
	}
}

//----------------------------------------------------------------------------------------------------------------------
int EdgeHistogramExtractionTool::GetEdgeFeature(unsigned char *pImage_Y,
						   int image_width,
						   int block_size,int Te_Value)
{
	int i, j;
	double	d1, d2, d3, d4;
	int e_index;
	double dc_th = Te_Value;
	double e_h, e_v, e_45, e_135, e_m, e_max;
	
	d1=0.0;
	d2=0.0;
	d3=0.0;
	d4=0.0;

	for(j=0; j<block_size; j++)for(i=0; i<block_size; i++){

		if(j<block_size/2){
			if(i<block_size/2)
				d1+=(pImage_Y[i+image_width*j]);
			else
				d2+=(pImage_Y[i+image_width*j]);
		}
		else{
			if(i<block_size/2)
				d3+=(pImage_Y[i+image_width*j]);
			else
				d4+=(pImage_Y[i+image_width*j]);
		}
	}
	d1 = d1/(block_size*block_size/4.0);
	d2 = d2/(block_size*block_size/4.0);
	d3 = d3/(block_size*block_size/4.0);
	d4 = d4/(block_size*block_size/4.0);

	e_h = fabs(d1+d2-(d3+d4));
	e_v = fabs(d1+d3-(d2+d4));
	// KW: explicit double
	//e_45 = sqrt(2)*fabs(d1-d4);
	//e_135 = sqrt(2)*fabs(d2-d3);
	e_45 = sqrt(2.0)*fabs(d1-d4);
	e_135 = sqrt(2.0)*fabs(d2-d3);

	e_m = 2*fabs(d1-d2-d3+d4);

	e_max = e_v;
	e_index = vertical_edge;
	if(e_h>e_max){
		e_max=e_h;
		e_index = horizontal_edge;
	}
	if(e_45>e_max){
		e_max=e_45;
		e_index = diagonal_45_degree_edge;
	}
	if(e_135>e_max){
		e_max=e_135;
		e_index = diagonal_135_degree_edge;
	}
	if(e_m>e_max){
		e_max=e_m;
		e_index = non_directional_edge;
	}
	if(e_max<dc_th)
		e_index = NoEdge;

	return(e_index);
}

//----------------------------------------------------------------------------
// This initialises the extraction process. Bascially it should 
// reset all storage etc. However, as this hasn't got any storage
// it's a bit pointless.
unsigned long EdgeHistogramExtractionTool::PostExtracting(void)
{
  return(0UL);
}

//----------------------------------------------------------------------------
EHD* EdgeHistogramExtractionTool::GetEdgeHistogram()
{
	return	m_pEdge_Histogram;
}

//----------------------------------------------------------------------------
char* EdgeHistogramExtractionTool::GetEdgeHistogramElement()
{
	return	m_pEdge_HistogramElement;
}

//----------------------------------------------------------------------------
void EdgeHistogramExtractionTool::SetEdgeHistogramElement(char*	pEdgeHistogram)
{
	int i;
	for( i=0; i < 80; i++ )
		m_pEdge_Histogram->Local_Edge[i] = QuantTable[ i%5 ][ pEdgeHistogram[i] ];

	memcpy(m_pEdge_HistogramElement, pEdgeHistogram, 80);
}

//----------------------------------------------------------------------------
void EdgeHistogramExtractionTool::SetEdgeHistogram(EHD*	pEdge_Histogram)
{
  int i, j;
  double iQuantValue;

  for( i=0; i < 80; i++ ) {
    j=0;
    while(1){
      if( j < 7 ) // SIZI-1 
        iQuantValue = (QuantTable[i%5][j]+QuantTable[i%5][j+1])/2.0;
      else 
        iQuantValue = 1.0;
      if(pEdge_Histogram->Local_Edge[i] <= iQuantValue)
        break;
      j++;
    }
    m_pEdge_HistogramElement[i] = j;
  }
  for( i=0; i < 80; i++ ){
    m_pEdge_Histogram->Local_Edge[i] = QuantTable[ i%5 ][ m_pEdge_HistogramElement[i] ];
  }
}
