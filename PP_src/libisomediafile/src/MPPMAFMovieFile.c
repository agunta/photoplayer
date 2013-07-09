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

/*
  $Id: MPPMAFMovieFile.c,v 1.0 2006/07/17 13:20:00 , KW $
*/
#include "ISOMovies.h"
#include "MP4Atoms.h"
#include "MJ2Atoms.h"

ISO_EXTERN ( ISOErr )
MPPMAFPutMovieIntoHandle( ISOMovie theMovie, ISOHandle movieH )
{
	ISOErr err;
	MP4PrivateMovieRecordPtr movie;
	MP4MovieAtomPtr     moov;
	MP4MediaDataAtomPtr mdat;
	MP4FileTypeAtomPtr	ftyp;
	
	err = ISONoErr;
	if ( (theMovie == NULL) || (movieH == NULL) )
		BAILWITHERROR( MP4BadParamErr );

	// KW: to do - add descriptors
	//err = MP4MovieCreateDescriptors( theMovie ); if (err) goto bail;

	movie = (MP4PrivateMovieRecordPtr) theMovie;
	moov = (MP4MovieAtomPtr) movie->moovAtomPtr;

	/* add the file type atom*/
	ftyp = (MP4FileTypeAtomPtr) movie->ftyp;

	err = moov->calculateSize( (MP4AtomPtr) moov ); if (err) goto bail;
	
	mdat = (MP4MediaDataAtomPtr) movie->mdat;
	err = mdat->calculateSize( (MP4AtomPtr) mdat ); if (err) goto bail;

	/* adjust mdat-related chunk offsets so they reflect the offset of the mdat within the movie handle */

	err = moov->mdatMoved( moov, ftyp->size + moov->size + 8 ); if (err) goto bail;

	err = ISOSetHandleSize( movieH, ftyp->size + moov->size + mdat->size ); if (err) goto bail;

	//err = ftyp->serialize( (MP4AtomPtr) ftyp, *movieH); if (err) goto bail;
	err = ftyp->serialize( (MP4AtomPtr) ftyp, *movieH); if (err) goto bail;
	err = moov->serialize( (MP4AtomPtr) moov, *movieH + ftyp->size ); if (err) goto bail;
	err = mdat->serialize( (MP4AtomPtr) mdat, *movieH + ftyp->size + moov->size ); if (err) goto bail;
bail:

	TEST_RETURN( err );

	return err;
}


ISO_EXTERN ( ISOErr )
MPPMAFWriteMovieToFile( ISOMovie theMovie, const char *filename )
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
	err = MPPMAFPutMovieIntoHandle( theMovie, movieH ); if (err) goto bail;
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

