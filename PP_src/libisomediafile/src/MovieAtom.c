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
	$Id: MovieAtom.c,v 1.2 2000/05/17 08:01:30 francesc Exp $
*/

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

#include "MP4Atoms.h"
#include <stdlib.h>

static MP4Err addAtom( MP4MovieAtomPtr self, MP4AtomPtr atom );

static void destroy( MP4AtomPtr s )
{
	MP4Err err;
	MP4MovieAtomPtr self;
	u32 i;
	err = MP4NoErr;
	self = (MP4MovieAtomPtr) s;
	if ( self == NULL )
		BAILWITHERROR( MP4BadParamErr )
	DESTROY_ATOM_LIST
	
	err = MP4DeleteLinkedList( self->trackList ); if (err) goto bail;

	if ( self->jp2h ) {
		self->jp2h->destroy( self->jp2h );
		self->jp2h = NULL;
	}
		
	/*if ( self->ftyp ) {
		self->ftyp->destroy( self->ftyp );
		self->ftyp = NULL;
	}
	*/
	
	if ( self->sgnt ) {
		self->sgnt->destroy( self->sgnt );
		self->sgnt = NULL;
	}
		
	if ( self->super )
		self->super->destroy( s );
bail:
	TEST_RETURN( err );

	return;
}

static MP4Err getNextTrackID( MP4MovieAtomPtr self, u32 *outID )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	*outID = mvhd->nextTrackID;
bail:
	TEST_RETURN( err );

	return err;
}

/*
	Try to reserve the specified track ID.
	Sets *outSuccess nonzero if we may use the requested trackID.
	Has a side effect of setting the movie's nextTrackID to one
	greater than the requested trackID if this would make it the
	highest numbered track in the movie.
*/
static MP4Err requestTrackID( struct MP4MovieAtom* self, u32 trackID, u32 *outSuccess )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	
	err = MP4NoErr;
	if ( (outSuccess == 0) || (trackID == 0) )
		BAILWITHERROR( MP4BadParamErr );
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	*outSuccess = 0;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	if ( trackID >= mvhd->nextTrackID )
	{
		mvhd->nextTrackID = trackID + 1;
		*outSuccess = 1;
	}
	else
	{
		u32 i;
		u32 trackCount = 0;
		err = MP4GetListEntryCount( self->trackList, &trackCount ); if (err) goto bail;
		for ( i = 0; i < trackCount; i++ )
		{
			MP4TrackAtomPtr trak;
			MP4TrackHeaderAtomPtr tkhd;
			err = MP4GetListEntry( self->trackList, i, (char**) &trak ); if (err) goto bail;
			if ( trak == NULL )
				BAILWITHERROR( MP4InvalidMediaErr );
			tkhd = (MP4TrackHeaderAtomPtr) trak->trackHeader;
			if ( tkhd == NULL )
				BAILWITHERROR( MP4InvalidMediaErr );
			if  ( tkhd->trackID == trackID )
				goto bail;
		}
		*outSuccess = 1;
	}
bail:
	return err;
}

static MP4Err newTrackWithID( struct MP4MovieAtom* self, u32 newTrackFlags, u32 newTrackID, MP4AtomPtr *outTrack )
{
	MP4Err MP4CreateTrackAtom( MP4TrackAtomPtr *outAtom );
	MP4Err MP4CreateTrackHeaderAtom( MP4TrackHeaderAtomPtr *outAtom );
	MP4Err MP4GetCurrentTime( u64 *outTime );

	MP4MovieHeaderAtomPtr mvhd;
	MP4TrackAtomPtr trak;
	MP4TrackHeaderAtomPtr tkhd;
	u32 success;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )

	err = requestTrackID( self, newTrackID, &success ); if (err) goto bail;
	if ( success == 0 )
	{
		BAILWITHERROR( MP4BadParamErr );
	}
	err = MP4CreateTrackAtom( &trak ); if (err) goto bail;
	err = MP4CreateTrackHeaderAtom( &tkhd ); if (err) goto bail;
	trak->newTrackFlags = newTrackFlags;
	tkhd->trackID = newTrackID;
	err = MP4GetCurrentTime( &tkhd->creationTime ); if (err) goto bail;
	tkhd->modificationTime = tkhd->creationTime;
	if ( newTrackFlags & MP4NewTrackIsVisual )
	{
		tkhd->qt_trackWidth  = (320 << 16);
		tkhd->qt_trackHeight = (240 << 16);
	}
	else if ( newTrackFlags & MP4NewTrackIsAudio )
	{
		tkhd->qt_volume = (1 << 8);
	}
	err = trak->addAtom( trak, (MP4AtomPtr) tkhd ); if (err) goto bail;
	err = addAtom( self, (MP4AtomPtr) trak ); if (err) goto bail;
	*outTrack = (MP4AtomPtr) trak;
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err newTrack( struct MP4MovieAtom* self, u32 newTrackFlags, MP4AtomPtr *outTrack )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err = MP4NoErr;
	
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	err = newTrackWithID( self, newTrackFlags, mvhd->nextTrackID, outTrack );
bail:
	TEST_RETURN( err );
	return err;
}
	
static MP4Err setTimeScale( struct MP4MovieAtom* self, u32 timeScale )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	mvhd->timeScale = timeScale;
	/* 
	   !!! TODO: For each track, recalculate durations, including
	   segment times in edit lists !!!
	*/
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err getTimeScale( struct MP4MovieAtom* self, u32 *outTimeScale )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	*outTimeScale = mvhd->timeScale;
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err setMatrix( struct MP4MovieAtom* self, u32 matrix[9] )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	memcpy( &(mvhd->qt_matrixA), matrix, sizeof(ISOMatrixRecord) );
/* 	mvhd->qt_matrixA = matrix[0];
	mvhd->qt_matrixB = matrix[1];
	mvhd->qt_matrixU = matrix[2];
	mvhd->qt_matrixC = matrix[3];
	mvhd->qt_matrixD = matrix[4];
	mvhd->qt_matrixV = matrix[5];
	mvhd->qt_matrixX = matrix[6];
	mvhd->qt_matrixY = matrix[7];
	mvhd->qt_matrixW = matrix[8]; */
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err getMatrix( struct MP4MovieAtom* self, u32 outMatrix[9] )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	memcpy( outMatrix, &(mvhd->qt_matrixA), sizeof(ISOMatrixRecord) );
/*	outMatrix[0] = mvhd->qt_matrixA;
	outMatrix[1] = mvhd->qt_matrixB;
	outMatrix[2] = mvhd->qt_matrixU;
	outMatrix[3] = mvhd->qt_matrixC;
	outMatrix[4] = mvhd->qt_matrixD;
	outMatrix[5] = mvhd->qt_matrixV;
	outMatrix[6] = mvhd->qt_matrixX;
	outMatrix[7] = mvhd->qt_matrixY;
	outMatrix[8] = mvhd->qt_matrixW; */
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err setPreferredRate( struct MP4MovieAtom* self, u32 rate )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	mvhd->qt_preferredRate = rate;
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err getPreferredRate( struct MP4MovieAtom* self, u32 *outRate )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	if ( outRate == NULL )
		BAILWITHERROR( MP4BadParamErr );
	*outRate = mvhd->qt_preferredRate;
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err setPreferredVolume( struct MP4MovieAtom* self, s16 volume )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	mvhd->qt_preferredVolume = volume;
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err getPreferredVolume( struct MP4MovieAtom* self, s16 *outVolume )
{
	MP4MovieHeaderAtomPtr mvhd;
	MP4Err err;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	if ( outVolume == NULL )
		BAILWITHERROR( MP4BadParamErr );
	*outVolume = (s16) mvhd->qt_preferredVolume;
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err addTrack( struct MP4MovieAtom* self, MP4AtomPtr track )
{
	MP4Err err;
	err = MP4NoErr;
	err = addAtom( self, track ); if (err) goto bail;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err addAtom( MP4MovieAtomPtr self, MP4AtomPtr atom )
{
	MP4Err err;
	err = MP4NoErr;
	assert( atom );
	err = MP4AddListEntry( atom, self->atomList ); if (err) goto bail;
	switch( atom->type )
	{
		case MP4ObjectDescriptorAtomType:
			if ( self->iods )
				BAILWITHERROR( MP4BadDataErr )
			self->iods = atom;
			break;
			
		case MP4MovieHeaderAtomType:
			if ( self->mvhd )
				BAILWITHERROR( MP4BadDataErr )
			self->mvhd = atom;
			break;
			
		case MP4UserDataAtomType:
			if ( self->udta )
				BAILWITHERROR( MP4BadDataErr )
			self->udta = atom;
			break;
			
		case MP4TrackAtomType:
			err = MP4AddListEntry( atom, self->trackList ); if (err) goto bail;
			break;
		case MP4MetadataAtomType:
			if ( self->meta )
				BAILWITHERROR( MP4BadDataErr )
			self->meta = atom;
			break;
	}
bail:
	TEST_RETURN( err );

	return err;
}

static u32 getTrackCount( MP4MovieAtomPtr self )
{
	u32 trackCount = 0;
	MP4GetListEntryCount( self->trackList, &trackCount );
	return trackCount;
}

static MP4Err setupTracks( MP4MovieAtomPtr self, MP4PrivateMovieRecordPtr moov )
{
	u32 trackIdx;
	u32 i;
	MP4Err err;
	u32 trackCount;
	err = MP4NoErr;
	
	self->moov = moov;
	trackCount = getTrackCount( self );
	for( i = 0, trackIdx = 0; i < trackCount; i++ )
	{
		MP4TrackAtomPtr atom;
		err = MP4GetListEntry( self->trackList, i, (char **) &atom ); if (err) goto bail;
		atom->moov = self->moov;
	}
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err getIndTrack( struct MP4MovieAtom* self, u32 trackNumber, MP4AtomPtr *outTrack )
{
	MP4Err err;
	
	err = MP4NoErr;
	if ( (trackNumber == 0) || (outTrack == NULL) || (trackNumber > getTrackCount(self)) )
		BAILWITHERROR( MP4BadParamErr )
	err = MP4GetListEntry( self->trackList, trackNumber - 1, (char**) outTrack ); if (err) goto bail;
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err newMetadata( struct MP4MovieAtom *self, MP4Metadata *outMetadata, u32 handlerType, MP4Handle data )
{
   MP4Err MP4CreateMetadataAtom( MP4MetadataAtomPtr *outAtom );
	
   MP4Err err;
   MP4Metadata meta;
   MP4MetadataAtomPtr mta;
	
   err = MP4NoErr;
   err = MP4CreateMetadataAtom( &mta ); if (err) goto bail;
   meta = (MP4Metadata) mta;
   err = mta->setupNew( mta, handlerType, data ); if (err) goto bail;
   err = addAtom( self, (MP4AtomPtr) mta ); if (err) goto bail;
   *outMetadata = meta;
  bail:
   TEST_RETURN( err );

   return err;
}



static MP4Err calculateDuration( struct MP4MovieAtom* self )
{
	MP4MovieHeaderAtomPtr mvhd;
	u32 maxDuration;
	u32 timeScale;
	u32 trackCount;
	u32 i;
	MP4Err err;
	
	maxDuration = 0;
	err = MP4NoErr;
	mvhd = (MP4MovieHeaderAtomPtr) self->mvhd;
	if ( mvhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );
	err = getTimeScale( self, &timeScale ); if (err) goto bail;
	trackCount = getTrackCount( self );
	
	for ( i = 0; i < trackCount; i++ )
	{
		MP4TrackAtomPtr trak;
		u32 trackDuration;
		
		err = MP4GetListEntry( self->trackList, i, (char**) &trak ); if (err) goto bail;
		if ( trak == NULL )
			BAILWITHERROR( MP4InvalidMediaErr );
		err = trak->calculateDuration( trak, timeScale ); if (err) goto bail;
		err = trak->getDuration( trak, &trackDuration ); if (err) goto bail;
		if ( trackDuration > maxDuration )
			maxDuration = trackDuration;
	}
	mvhd->duration = maxDuration;
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err mdatMoved( struct MP4MovieAtom* self, s32 mdatOffset )
{
	u32 trackCount;
	u32 i;
	MP4Err err;
	
	err = MP4NoErr;

	trackCount = getTrackCount( self );
	
	for ( i = 0; i < trackCount; i++ )
	{
		MP4TrackAtomPtr trak;
		err = MP4GetListEntry( self->trackList, i, (char**) &trak ); if (err) goto bail;
		if ( trak == NULL )
			BAILWITHERROR( MP4InvalidMediaErr );
		err = trak->mdatMoved( trak, mdatOffset ); if (err) goto bail;
	}
bail:
	TEST_RETURN( err );
	return err;
}

static MP4Err serialize( struct MP4Atom* s, char* buffer )
{
	MP4Err err;
	MP4MovieAtomPtr self = (MP4MovieAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4SerializeCommonBaseAtomFields( s, buffer ); if (err) goto bail;
    buffer += self->bytesWritten;	
    SERIALIZE_ATOM_LIST( atomList );
	assert( self->bytesWritten == self->size );
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err calculateSize( struct MP4Atom* s )
{
	MP4Err err;
	MP4MovieAtomPtr self = (MP4MovieAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4CalculateBaseAtomFieldSize( s ); if (err) goto bail;
	ADD_ATOM_LIST_SIZE( atomList );
bail:
	TEST_RETURN( err );

	return err;
}


static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	PARSE_ATOM_LIST(MP4MovieAtom)
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4CreateMovieAtom( MP4MovieAtomPtr *outAtom )
{
	MP4Err err;
	MP4MovieAtomPtr self;
	
	self = (MP4MovieAtomPtr) calloc( 1, sizeof(MP4MovieAtom) );
	TESTMALLOC( self )

	err = MP4CreateBaseAtom( (MP4AtomPtr) self ); if ( err ) goto bail;
	self->type = MP4MovieAtomType;
	self->name                = "movie";
	self->createFromInputStream = (cisfunc) createFromInputStream;
	self->destroy             = destroy;
	self->setupTracks = setupTracks;
	self->addTrack = addTrack;
	self->addAtom     = addAtom;
	self->newTrack = newTrack;
	self->calculateDuration = calculateDuration;
	self->getNextTrackID = getNextTrackID;
	self->setTimeScale = setTimeScale;
	err = MP4MakeLinkedList( &self->atomList ); if (err) goto bail;
	err = MP4MakeLinkedList( &self->trackList ); if (err) goto bail;
	self->calculateSize         = calculateSize;
	self->serialize             = serialize;
	self->getTrackCount = getTrackCount;
	self->getIndTrack = getIndTrack;
	self->getTimeScale = getTimeScale;
	self->setMatrix = setMatrix;
	self->getMatrix = getMatrix;
	self->setPreferredRate = setPreferredRate;
	self->getPreferredRate = getPreferredRate;
	self->setPreferredVolume = setPreferredVolume;
	self->getPreferredVolume = getPreferredVolume;
	self->mdatMoved = mdatMoved;
	self->newTrackWithID = newTrackWithID;
	self->newMetadata = newMetadata;
	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}
