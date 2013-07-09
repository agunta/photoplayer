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
  $Id: MP4MovieFile.c,v 1.2 2000/03/29 13:27:47 francesc Exp $
*/
#include "MP4Movies.h"
#include "MP4Atoms.h"

MP4_EXTERN ( MP4Err )
MP4PutMovieIntoHandle( MP4Movie theMovie, MP4Handle movieH )
{
	MP4Err MP4MovieCreateDescriptors( MP4Movie theMovie );

	MP4Err err;
	MP4PrivateMovieRecordPtr movie;
	MP4MovieAtomPtr     moov;
	MP4MediaDataAtomPtr mdat;

	err = MP4NoErr;
	if ( (theMovie == NULL) || (movieH == NULL) )
		BAILWITHERROR( MP4BadParamErr );

	err = MP4MovieCreateDescriptors( theMovie ); if (err) goto bail;

	movie = (MP4PrivateMovieRecordPtr) theMovie;
	moov = (MP4MovieAtomPtr) movie->moovAtomPtr;
	err = moov->calculateSize( (MP4AtomPtr) moov ); if (err) goto bail;
	
	mdat = (MP4MediaDataAtomPtr) movie->mdat;
	err = mdat->calculateSize( (MP4AtomPtr) mdat ); if (err) goto bail;

	/* adjust mdat-related chunk offsets so they reflect the offset of the mdat within the movie handle */
	err = moov->mdatMoved( moov, moov->size + 8 ); if (err) goto bail;

	err = MP4SetHandleSize( movieH, moov->size + mdat->size ); if (err) goto bail;

	err = moov->serialize( (MP4AtomPtr) moov, *movieH ); if (err) goto bail;
	err = mdat->serialize( (MP4AtomPtr) mdat, *movieH + moov->size ); if (err) goto bail;
bail:
	TEST_RETURN( err );

	return err;
}


MP4_EXTERN ( MP4Err )
MP4WriteMovieToFile( MP4Movie theMovie, const char *filename )
{
	MP4Err err;
	size_t written;
	MP4Handle movieH;
	FILE *fd;
	u32 handleSize;

	err = MP4NoErr;
	movieH = NULL;
	fd = NULL;
	if ( (theMovie == NULL) || (filename == NULL) )
		BAILWITHERROR( MP4BadParamErr );
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
		BAILWITHERROR( MP4IOErr );
	err = MP4NewHandle( 0, &movieH ); if (err) goto bail;
	err = MP4PutMovieIntoHandle( theMovie, movieH ); if (err) goto bail;
	err = MP4GetHandleSize( movieH, &handleSize ); if (err) goto bail;
	written = fwrite( *movieH, 1, handleSize, fd );
	if ( written != handleSize )
		BAILWITHERROR( MP4IOErr );
bail:
	if ( movieH )
	{
		MP4DisposeHandle( movieH );
		movieH = NULL;
	}
	if ( fd )
	{
		fclose( fd );
		fd = NULL;
	}

	TEST_RETURN( err );

	return err;
}


