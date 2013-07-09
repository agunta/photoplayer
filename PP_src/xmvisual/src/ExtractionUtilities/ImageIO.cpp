///////////////////////////////////////////////////////////////////////////////
//
// This software module was originally developed by
//
// TU-Munich
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
// Copyright (c) 1998-2003.
//
// This notice must be included in all copies or derivative works.
//
// MediaIO.cpp

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

#include "stdio.h"
#include "ImageIO.h"
extern "C"
{
#include "../../jpeglib/jpeglib.h"
}
#include <vopio.h>
#include <setjmp.h>

ImageIO::ImageIO()
{
	m_pImage = NULL;
}

ImageIO::~ImageIO()
{
	if(m_pImage)
	{
		freevop(m_pImage);
	}
}

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};
 
typedef struct my_error_mgr * my_error_ptr;
 
METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


// based on MediaIO.cpp from MPEG-7 XM
// using jpeglib to decode jpeg files
bool ImageIO::LoadImage(const char *fileName)
{
	unsigned int xsize,ysize;

	struct jpeg_decompress_struct cinfo; 
	int jpg_width=0, jpg_height=0;

	FILE * infile;		/* source file */

	if ((infile = fopen(fileName, "rb")) == NULL) {
        return false;
	}

	struct my_error_mgr jerr; 
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return false;
    } 

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	if(jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK)
	{
		fclose(infile);
		jpeg_destroy_decompress(&cinfo);
        return false;
	}
	(void)jpeg_start_decompress(&cinfo);

	xsize=cinfo.output_width;
	ysize=cinfo.output_height;

	// check parameters
	if(xsize <= 0 || ysize <= 0 || xsize > 16000 || ysize >16000){
		fclose(infile);
		jpeg_destroy_decompress(&cinfo);
        return false;
	}

	unsigned char* pData = new unsigned char[xsize * ysize * 3];
	if(!pData) return false;

	unsigned char *pScan = pData;
	unsigned int scan_num=0;
	for(scan_num=0; scan_num<ysize; scan_num++)
	{
		int scan_read = jpeg_read_scanlines(&cinfo, &pScan, 1);
		if(scan_read != 1)
		{
			break;
		}
		pScan += xsize*3;
	}

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	MomVop *vop;
	if(m_pImage)
	{
		freevop(m_pImage);
	}
	MomImageType buf_type;
	TChan bitfilter;
	MomImageData r,g,b,a;
	int sr,sg,sb,sa;        // size of pixel for the buffers
	unsigned long pixels;
	unsigned long totalpix=0;
	int minalpha=255, maxalpha=0;

    /* test open file*/
    /*FILE* =fopen(fileName,O_RDONLY);
    if (a) *a='[';
    if (inputfile==-1) return 0;
    close(inputfile);*/
	
//#ifdef _DEBUG
	fprintf(stderr,"Loading : %s\n",fileName);
	fflush(stderr);
//#endif
	/* allocate MomVop buffers*/
	totalpix=xsize*ysize;
	
	buf_type=UCHAR_TYPE;
	bitfilter=255;
	vop=initvop(xsize,ysize,_444_,buf_type,buf_type,buf_type,buf_type);
	
	if (!vop) {
		fprintf(stderr,"\n");
		return false;
	}

	/* copy data from DecodedImage to Vop*/
	if (vop->y_chan) g.u=vop->y_chan->data->u;
	else g.u=0;
	sg=datasize(vop->y_chan);

	if (vop->u_chan) b.u=vop->u_chan->data->u;
	else b.u=0;
	sb=datasize(vop->u_chan);
	
	if (vop->v_chan) r.u=vop->v_chan->data->u;
	else r.u=0;
	sr=datasize(vop->v_chan);

	if (vop->a_chan) a.u=vop->a_chan->data->u;
	else a.u=0;
	sa=datasize(vop->a_chan);

    minalpha=255;
    maxalpha=255;

	unsigned long packets;
    packets = xsize * ysize;
	
	long red, green, blue;

	pScan = pData;
	for (pixels=0; pixels < totalpix; pixels++) {
		red = (long)*pScan;
		pScan++;
		if (r.u) {
			setdata(r,buf_type,((TChan)(red))&bitfilter);
			r.u+=sr;
		}
		green = (long)*pScan;
		pScan++;
		if (g.u) {
			setdata(g,buf_type,((TChan)(green))&bitfilter);
			g.u+=sg;
		}
		blue = (long)*pScan;
		pScan++;
		if (b.u) {
			setdata(b,buf_type,((TChan)(blue))&bitfilter);
			b.u+=sb;
		}
		if (a.u) {
			setdata(a,buf_type,(TChan)255);
			a.u+=sa;
		}
	}
	if(pData) delete pData;
	m_pImage = vop;
	return true;
}

MomVop* ImageIO::GetImage()
{
	return m_pImage;
}

int ImageIO::GetWidth()
{
	if(m_pImage) return m_pImage->width;
	return 0;
}
int ImageIO::GetHeight()
{
	if(m_pImage) return m_pImage->height;
	return 0;
}
