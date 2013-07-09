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
derivative works. Copyright (c) 1999, 2000.
*/
/*
	$Id: QTMovies.c,v 1.56 2000/10/06 07:38:50 tm Exp $
*/
#include "ISOMovies.h"
#include "MP4Atoms.h"
#include "MJ2Atoms.h"
#include "MP4InputStream.h"
#include "MP4Impl.h"
#include "MovieTracks.h"
#include "FileMappingObject.h"
#include "MP4Descriptors.h"
#include "MdatDataHandler.h"

#ifdef macintosh
#include <OSUtils.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

ISO_EXTERN ( ISOErr )
QTNewMovie( ISOMovie *outMovie )
{
	ISOErr MP4CreateMediaDataAtom( MP4MediaDataAtomPtr *outAtom );
	ISOErr MP4CreateMovieAtom( MP4MovieAtomPtr *outAtom );
	ISOErr MP4CreateMovieHeaderAtom( MP4MovieHeaderAtomPtr *outAtom );
	ISOErr MP4GetCurrentTime( u64 *outTime );
	   
	u64 now;
	ISOErr err;
	MP4PrivateMovieRecordPtr	movie;
	MP4MovieAtomPtr				moov;
	MP4MovieHeaderAtomPtr		mvhd;
	MP4MediaDataAtomPtr			mdat;

	movie = (MP4PrivateMovieRecordPtr) calloc( 1, sizeof(MP4PrivateMovieRecord) );
	if ( movie == NULL )
	{
		err = MP4NoMemoryErr;
		goto bail;
	}
	movie->referenceCount     = 1;
	err = MP4GetCurrentTime( &now ); if (err) goto bail;
	err = MP4CreateMovieAtom( (MP4MovieAtomPtr *) &movie->moovAtomPtr ); if (err) goto bail;
	moov = (MP4MovieAtomPtr) movie->moovAtomPtr;
	err = MP4CreateMovieHeaderAtom( &mvhd ); if (err) goto bail;
	mvhd->nextTrackID = 1;
	mvhd->creationTime = now;
	mvhd->modificationTime = now;
	err = moov->addAtom( moov, (MP4AtomPtr) mvhd ); if (err) goto bail;
	moov->setTimeScale( moov, 600 );
	err = MP4CreateMediaDataAtom( &mdat );
	movie->mdat = (MP4AtomPtr) mdat;
	{
	   MP4DataHandlerPtr dh;
	   
	   err = MP4CreateMdatDataHandler( mdat, &dh ); if (err) goto bail;
	   movie->inMemoryDataHandler = dh;
	}
	movie->fileType = ISOQuickTimeFileType;
	
	*outMovie = (MP4Movie) movie;
bail:
	TEST_RETURN( err );

	return err;	
}
