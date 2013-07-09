/*
This software module was originally developed by Apple Computer, Inc.
in the course of development of MPEG-4. 
This software module is an implementation of a part of one or 
more MPEG-4 tools as specified by MPEG-4. 
ISO/IEC gives users of MPEG-4 free license to this
software module or modifications thereof for use in hardware 
or software products claiming conformance to MPEG-4.
Those intending to use this software module in hardware or software
products are advised that its use may infringe existing patents.
The original developer of this software module and his/her company,
the subsequent editors and their companies, and ISO/IEC have no
liability for use of this software module or modifications thereof
in an implementation.
Copyright is not released for non MPEG-4 conforming
products. Apple Computer, Inc. retains full right to use the code for its own
purpose, assign or donate the code to a third party and to
inhibit third parties from using the code for non 
MPEG-4 conforming products.
This copyright notice must be included in all copies or
derivative works. Copyright (c) 1999.
*/
/*
  $Id: MJ2MovieFile.c,v 1.15 2001/06/06 01:44:15 tm Exp $
*/
#include "ISOMovies.h"
#include "MP4Atoms.h"
#include "MJ2Atoms.h"

ISO_EXTERN ( ISOErr )
MJ2PutMovieIntoHandle( ISOMovie theMovie, ISOHandle movieH )
{
	ISOErr err;
	MP4PrivateMovieRecordPtr movie;
	MP4MovieAtomPtr     moov;
	MP4MediaDataAtomPtr mdat;
	MJ2JPEG2000SignatureAtomPtr	sgnt;
	MJ2FileTypeAtomPtr	ftyp;
	MJ2HeaderAtomPtr	head;
	
	err = ISONoErr;
	if ( (theMovie == NULL) || (movieH == NULL) )
		BAILWITHERROR( MP4BadParamErr );

	movie = (MP4PrivateMovieRecordPtr) theMovie;
	moov = (MP4MovieAtomPtr) movie->moovAtomPtr;

	/* add the signature atom to the front of the file */
	sgnt = (MJ2JPEG2000SignatureAtomPtr) moov->sgnt;

	/* add the file type atom after the signature atom */
	ftyp = (MJ2FileTypeAtomPtr) movie->ftyp;

	/* add the header atom after the file type atom */
	head = (MJ2HeaderAtomPtr) moov->jp2h;
	
	err = head->calculateSize( (MP4AtomPtr) head ); if (err) goto bail;

	err = moov->calculateSize( (MP4AtomPtr) moov ); if (err) goto bail;
	
	mdat = (MP4MediaDataAtomPtr) movie->mdat;
	err = mdat->calculateSize( (MP4AtomPtr) mdat ); if (err) goto bail;

	/* adjust mdat-related chunk offsets so they reflect the offset of the mdat within the movie handle */

	err = moov->mdatMoved( moov, sgnt->size + ftyp->size + head->size + moov->size + 8 ); if (err) goto bail;

	err = ISOSetHandleSize( movieH, sgnt->size + ftyp->size + head->size + moov->size + mdat->size ); if (err) goto bail;

	err = sgnt->serialize( (MP4AtomPtr) sgnt, *movieH ); if (err) goto bail;
	err = ftyp->serialize( (MP4AtomPtr) ftyp, *movieH + sgnt->size); if (err) goto bail;
	err = head->serialize( (MP4AtomPtr) head, *movieH + sgnt->size + ftyp->size); if (err) goto bail;
	err = moov->serialize( (MP4AtomPtr) moov, *movieH + sgnt->size + ftyp->size + head->size ); if (err) goto bail;
	err = mdat->serialize( (MP4AtomPtr) mdat, *movieH + sgnt->size + ftyp->size + head->size + moov->size ); if (err) goto bail;
bail:

	TEST_RETURN( err );

	return err;
}


ISO_EXTERN ( ISOErr )
MJ2WriteMovieToFile( ISOMovie theMovie, const char *filename )
{
	ISOErr err;
	size_t written;
	ISOHandle movieH;
	FILE *fd;
	u32 handleSize;

	err = ISONoErr;
	movieH = NULL;
	fd = NULL;
	if ( (theMovie == NULL) || (filename == NULL) )
		BAILWITHERROR( ISOBadParamErr );
#ifdef macintosh
    {
        int len;
        int i;
        char *s;
        if ( *filename == '/' )
            filename++;
        s = (char *) filename;
        len = strlen( s );
        for ( i = 0; i < len; i++ )
        {
            if ( s[i] == '/' )
                s[i] = ':';
        }
    }
#endif
	fd = fopen( filename, "wb" );
	if ( fd == NULL )
		BAILWITHERROR( ISOIOErr );
	err = ISONewHandle( 0, &movieH ); if (err) goto bail;
	err = MJ2PutMovieIntoHandle( theMovie, movieH ); if (err) goto bail;
	err = ISOGetHandleSize( movieH, &handleSize ); if (err) goto bail;
	written = fwrite( *movieH, 1, handleSize, fd );
	if ( written != handleSize )
		BAILWITHERROR( ISOIOErr );
bail:
	if ( movieH )
		ISODisposeHandle( movieH );
	
	if ( fd )
		fclose( fd );

	TEST_RETURN( err );

	return err;
}

