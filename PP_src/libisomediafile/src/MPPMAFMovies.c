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
  $Id: MPPMAFMovies.c,v 1.0 2006/07/17 15:34:33 , KW $
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
MPPMAFNewMovie( ISOMovie *outMovie )
{
	ISOErr MP4CreateFileTypeAtom( MP4FileTypeAtomPtr *outAtom );
	ISOErr MP4CreateMediaDataAtom( MP4MediaDataAtomPtr *outAtom );
	ISOErr MP4CreateMetadataAtom( MP4MetadataAtomPtr *outAtom );
	ISOErr MP4CreateMovieAtom( MP4MovieAtomPtr *outAtom );
	ISOErr MP4CreateMovieHeaderAtom( MP4MovieHeaderAtomPtr *outAtom );
	ISOErr MP4GetCurrentTime( u64 *outTime );
	   
	u64 now;
	ISOErr err;
	MP4PrivateMovieRecordPtr	movie;
	MP4MovieAtomPtr				moov;
	MP4MovieHeaderAtomPtr		mvhd;
	MP4MediaDataAtomPtr			mdat;
	MP4FileTypeAtomPtr			ftyp;
	//MJ2HeaderAtomPtr			jp2h;
	//MJ2JPEG2000SignatureAtomPtr	sgnt;

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
	//mvhd->version = 1;
	err = moov->addAtom( moov, (MP4AtomPtr) mvhd ); if (err) goto bail;
	moov->setTimeScale( moov, 1 ); // 600
	mvhd->duration = 5; // ??
	err = MP4CreateMediaDataAtom( &mdat );
	movie->mdat = (MP4AtomPtr) mdat;
	{
	   MP4DataHandlerPtr dh;
	   
	   err = MP4CreateMdatDataHandler( mdat, &dh ); if (err) goto bail;
	   movie->inMemoryDataHandler = dh;
	}
	movie->fileType = ISOMPEG4FileType; //ISOUnknownFileType
	
	err = MP4CreateFileTypeAtom( &ftyp ); if (err) goto bail;
	ftyp->brand = MAFPhotoPlayerBrand;
	err = ftyp->addStandard(ftyp, MAFPhotoPlayerBrand);
	err = ftyp->calculateSize( (MP4AtomPtr) ftyp ); if (err) goto bail;
	movie->ftyp = (MP4AtomPtr) ftyp;
	
	moov->jp2h = NULL;	
	moov->sgnt = NULL;

	*outMovie = (ISOMovie) movie;
bail:
	TEST_RETURN( err );

	return err;	
}

ISO_EXTERN ( ISOErr )
MP4GetCompatibilityList(  ISOMovie theMovie, u32 *count, u32 **list)
{
	MP4Err err;
	MP4PrivateMovieRecordPtr movie;
	MP4FileTypeAtomPtr	ftyp;

	err = MP4NoErr;
	if (theMovie == NULL)
		BAILWITHERROR( MP4BadParamErr );

	movie = (MP4PrivateMovieRecordPtr) theMovie;
	ftyp = (MP4FileTypeAtomPtr) movie->ftyp;
	if(!ftyp) BAILWITHERROR( MP4BadDataErr );
	*count = ftyp->itemCount;
	*list = ftyp->compatibilityList;

bail:
	TEST_RETURN( err );
	if(err)
	{
		*count = 0;
		*list = NULL;
	}

	return err;
}
