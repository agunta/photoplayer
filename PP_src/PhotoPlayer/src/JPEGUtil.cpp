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

#include "stdafx.h"
#include "JPEGUtil.h"
#include <setjmp.h>
#include < stdio.h>
/*
extern "C"
{
#include <jpeglib.h>
}
*/

#ifdef WIN32
#include <io.h>
#include <sys/stat.h>
#else
#include <sys/stat.h>
#include <sys/unistd.h>
#endif
#include <time.h>

#undef JPEGLIB_INC
#ifdef JPEGLIB_INC
struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;
 
static void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}
 

bool LoadJPEGFile( char* jpegfile , char **data)
{

	struct jpeg_decompress_struct cinfo; 
	unsigned char *jpgData = NULL;
	int jpg_width=0, jpg_height=0;

	FILE * infile;		/* source file */
	//int row_stride;		/* physical row width in output buffer */

	if ((infile = fopen(jpegfile, "rb")) == NULL) {
        return false;
	}

	/* Now we can initialize the JPEG decompression object. */
    struct my_error_mgr jerr; 
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
		if(jpgData) delete jpgData;
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return false;
    } 
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */

	jpeg_stdio_src(&cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */

	if(jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK)
	{
		fclose(infile);
		jpeg_destroy_decompress(&cinfo);
        return false;
	}
	/* We can ignore the return value from jpeg_read_header since
	*   (a) suspension is not possible with the stdio data source, and
	*   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	* See libjpeg.doc for more info.
	*/

	/* Step 4: set parameters for decompression */

	/* In this example, we don't need to change any of the defaults set by
	* jpeg_read_header(), so we do nothing here.
	*/

	/* Step 5: Start decompressor */

	(void)jpeg_start_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* We may need to do some setup of our own at this point before reading
	* the data.  After jpeg_start_decompress() we have the correct scaled
	* output image dimensions available, as well as the output colormap
	* if we asked for color quantization.
	* In this example, we need to make an output work buffer of the right size.
	*/ 
	jpg_width=cinfo.output_width;
	jpg_height=cinfo.output_height;

	// check parameters
	if(jpg_width <= 0 || jpg_height <= 0 ||jpg_width > 16000 || jpg_height >16000){
		fclose(infile);
		jpeg_destroy_decompress(&cinfo);
        return false;
	}

	jpgData = new unsigned char[3 * jpg_width * jpg_height];

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */

	/* Here we use the library's state variable cinfo.output_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	*/
	unsigned char* pScan = jpgData;
	int scan_num=0;
	for(scan_num=0; scan_num<jpg_height; scan_num++)
	{
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could ask for
		* more than one scanline at a time if that's more convenient.
		*/
		int scan_read = jpeg_read_scanlines(&cinfo, &pScan, 1);
		if(scan_read != 1)
		{
			break;
		}
		pScan += 3*jpg_width;
	}
	/* Step 7: Finish decompression */

	(void) jpeg_finish_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* Step 8: Release JPEG decompression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(&cinfo);

	/* After finish_decompress, we can close the input file.
	* Here we postpone it until after no more JPEG errors are possible,
	* so as to simplify the setjmp error logic above.  (Actually, I don't
	* think that jpeg_destroy can do an error exit, but why assume anything...)
	*/
	fclose(infile);
	return true;
}


bool getJPEGFileInfo(char *jpegfile, JPEGInfo *jpg_info)
{
	struct jpeg_decompress_struct cinfo; 
	jpg_info->width = 0;
	jpg_info->height = 0;

	FILE * infile;		/* source file */

	if ((infile = fopen(jpegfile, "rb")) == NULL) {
        return 0;
	}
    struct my_error_mgr jerr; 
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return false;
    } 
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	if(jpeg_read_header(&cinfo, TRUE) == JPEG_HEADER_OK)
	{
		jpg_info->width = cinfo.output_width;
		jpg_info->height = cinfo.output_height;
	}
	fclose(infile);
	jpeg_destroy_decompress(&cinfo);


	return true;
}

#endif

/** returns the file size on disk */
long getJPEGFileSize(char *jpegfile)
{
	long fsize = 0;
	FILE * infile = fopen(jpegfile, "rb");
    if(!infile) return 0;

	int hFile;
#ifdef WIN32
	hFile = _fileno(infile);
    fsize = _filelength( hFile );
#else
	hFile = fileno(infile);
    struct stat st_info;
    fstat( hFile, &st_info );
    fsize=(long)st_info.st_size;
#endif
	fclose(infile);
    return fsize;	
}

/** reads the file data into buffer and returns the buffer */
char *getJPEGFile(char *jpegfile, int size)
{
	char *buf;
	size_t len = size;
	if(size == 0)
	{
		len = getJPEGFileSize(jpegfile);
	}
	if(len <=0)
	{
		return NULL;
	}

	FILE * infile = fopen(jpegfile, "rb");
    if(!infile) return NULL;

	buf = (char*)malloc(len);
	if(!buf)
	{
		// memory error
		return NULL;
	}
	size_t inread = fread(buf, 1, len, infile);

	fclose(infile);

	if(inread != len)
	{
		free(buf);
		return NULL;
	}

	return buf;
}

char *getLastUpdate(char *fileName)
{
	char *retbuf = NULL;
	struct _stat buf;
	int result;

	result = _stat( fileName, &buf );
	if(result != 0)
	{
		return NULL;
	}
	retbuf = ctime(&buf.st_mtime);
	return retbuf;
}