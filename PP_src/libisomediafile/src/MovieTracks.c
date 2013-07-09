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
	$Id: MovieTracks.c,v 1.28 2000/01/11 22:59:55 mc Exp $
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

#include "MP4Movies.h"
#include "MP4Atoms.h"
#include "MP4Impl.h"
#include "MovieTracks.h"
#include "MP4Descriptors.h"
#include <stdio.h>

MP4_EXTERN ( MP4Err ) MP4GetTrackID( MP4Track theTrack, u32 *outTrackID )
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   MP4TrackHeaderAtomPtr trackHeaderAtom;
   err = MP4NoErr;
   if ( theTrack == NULL )
      BAILWITHERROR( MP4BadParamErr )
	 trackAtom = (MP4TrackAtomPtr) theTrack;
   trackHeaderAtom = (MP4TrackHeaderAtomPtr) trackAtom->trackHeader;
   *outTrackID = trackHeaderAtom->trackID;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackEnabled( MP4Track theTrack, u32 *outEnabled )
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   err = MP4NoErr;
   if ( theTrack == NULL )
      BAILWITHERROR( MP4BadParamErr )
	 trackAtom = (MP4TrackAtomPtr) theTrack;
   err = trackAtom->getEnabled( trackAtom, outEnabled ); if (err) goto bail;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4SetTrackEnabled( MP4Track theTrack, u32 enabled )
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   err = MP4NoErr;
   if ( theTrack == NULL )
      BAILWITHERROR( MP4BadParamErr )
	 trackAtom = (MP4TrackAtomPtr) theTrack;
   err = trackAtom->setEnabled( trackAtom, enabled ); if (err) goto bail;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4AddTrackReferenceWithID( MP4Track theTrack, u32 dependsOnID, u32 dependencyType, u32 *outReferenceIndex )
{
   MP4Err err;
   MP4TrackAtomPtr trak;
   MP4TrackReferenceAtomPtr tref;
   MP4TrackReferenceTypeAtomPtr dpnd;

   err = MP4NoErr;
   if ( (theTrack == NULL) || (dependsOnID == 0) )
      BAILWITHERROR( MP4BadParamErr );
   trak = (MP4TrackAtomPtr) theTrack;
   tref = (MP4TrackReferenceAtomPtr) trak->trackReferences;
   if ( tref == NULL )
   {
      MP4Err MP4CreateTrackReferenceAtom( MP4TrackReferenceAtomPtr *outAtom );
      err = MP4CreateTrackReferenceAtom( &tref ); if (err) goto bail;
      err = trak->addAtom( trak, (MP4AtomPtr) tref ); if (err) goto bail;
   }
   err = tref->findAtomOfType( tref, dependencyType, (MP4AtomPtr*) &dpnd ); if (err) goto bail;
   if ( dpnd == NULL )
   {
      MP4Err MP4CreateTrackReferenceTypeAtom( u32 atomType, MP4TrackReferenceTypeAtomPtr *outAtom );
      err = MP4CreateTrackReferenceTypeAtom( dependencyType, &dpnd ); if (err) goto bail;
      err = tref->addAtom( tref, (MP4AtomPtr) dpnd ); if (err) goto bail;
   }
   err = dpnd->addTrackID( dpnd, dependsOnID ); if (err) goto bail;
   *outReferenceIndex = dpnd->trackIDCount;
  bail:
   TEST_RETURN( err );
   return err;
}

MP4_EXTERN ( MP4Err ) MP4AddTrackReference( MP4Track theTrack, MP4Track dependsOn, u32 dependencyType, u32 *outReferenceIndex )
{
   MP4Err err;
   u32 dependsOnID;

   err = MP4NoErr;
   if ( (theTrack == NULL) || (dependsOn == NULL) )
      BAILWITHERROR( MP4BadParamErr );
   err = MP4GetTrackID( dependsOn, &dependsOnID ); if (err) goto bail;
   if ( dependsOnID == 0 )
      BAILWITHERROR( MP4InvalidMediaErr );
   err = MP4AddTrackReferenceWithID( theTrack, dependsOnID, dependencyType, outReferenceIndex ); if (err) goto bail;
  bail:
   TEST_RETURN( err );
   return err;
}

MP4_EXTERN ( MP4Err ) MP4AddTrackToMovieIOD( MP4Track theTrack )
{
   MP4Err MP4MovieAddTrackES_IDToIOD( MP4Movie theMovie, MP4Track theTrack );	
   MP4Movie itsMovie;
   MP4Err err;
   if ( theTrack == 0 )
      BAILWITHERROR( MP4BadParamErr );
   err = MP4GetTrackMovie( theTrack, &itsMovie ); if (err) goto bail;
   err = MP4MovieAddTrackES_IDToIOD( itsMovie, theTrack ); if (err) goto bail;
  bail:
   TEST_RETURN( err );
   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackDuration( MP4Track theTrack, u64 *outDuration )
{
   MP4Err err;
   MP4TrackAtomPtr trak;
   MP4Movie moov;
   MP4Media mdia;
   u32 ts;
   u64 duration;
   u32 trakDuration;
   
   if ( (theTrack == 0) || (outDuration == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   err = MP4GetTrackMovie( theTrack, &moov ); if (err) goto bail;
   err = MP4GetTrackMedia( theTrack, &mdia ); if (err) goto bail;
   err = MP4GetMovieTimeScale( moov, &ts ); if (err) goto bail;
   err = MP4GetMediaDuration( mdia, &duration ); if (err) goto bail;
   trak = (MP4TrackAtomPtr) theTrack;
   err = trak->calculateDuration( trak, ts ); if (err) goto bail;
   err = trak->getDuration( trak, &trakDuration ); if (err) goto bail;
   *outDuration = (u64) trakDuration;
bail:
   TEST_RETURN( err );
   return err;
}

ISO_EXTERN ( ISOErr ) MJ2SetTrackMatrix( ISOTrack theTrack, u32 matrix[9] )
{
	MP4Err err;
	MP4TrackAtomPtr trak;
	u32 aMatrix[9];
	
	if ( theTrack == 0 ) {
		BAILWITHERROR( MP4BadParamErr );
	}
	
	trak = (MP4TrackAtomPtr) theTrack;

	if ( matrix == NULL ) {
		/* if we are passed a NULL matrix, use the identity matrix */
		aMatrix[0] = 0x00010000;
		aMatrix[1] = 0;
		aMatrix[2] = 0;
		aMatrix[3] = 0;
		aMatrix[4] = 0x00010000;
		aMatrix[5] = 0;
		aMatrix[6] = 0;
		aMatrix[7] = 0;
		aMatrix[8] = 0x40000000;
	} else {
		memcpy( &aMatrix, matrix, sizeof(ISOMatrixRecord) );
	}
	err = trak->setMatrix( trak, aMatrix );

bail:
	TEST_RETURN( err );
	return err;
}

ISO_EXTERN ( ISOErr ) MJ2GetTrackMatrix( ISOTrack theTrack, u32 outMatrix[9] )
{
	MP4Err err;
	MP4TrackAtomPtr trak;
	
	if ( theTrack == 0 ) {
		BAILWITHERROR( MP4BadParamErr );
	}
	
	trak = (MP4TrackAtomPtr) theTrack;

	if ( outMatrix == NULL )
		BAILWITHERROR( MP4BadParamErr );

	err = trak->getMatrix( trak, outMatrix );
bail:
	return err;
}

ISO_EXTERN ( ISOErr ) MJ2SetTrackLayer( ISOTrack theTrack, s16 layer )
{
	MP4Err err;
	MP4TrackAtomPtr trak;
	
	if ( theTrack == 0 ) {
		BAILWITHERROR( MP4BadParamErr );
	}
	
	trak = (MP4TrackAtomPtr) theTrack;

	err = trak->setLayer( trak, layer );

bail:
	TEST_RETURN( err );
	return err;
}

ISO_EXTERN ( ISOErr ) MJ2GetTrackLayer( ISOTrack theTrack, s16 *outLayer )
{
	MP4Err err;
	MP4TrackAtomPtr trak;
	
	if ( theTrack == 0 ) {
		BAILWITHERROR( MP4BadParamErr );
	}
	
	trak = (MP4TrackAtomPtr) theTrack;

	if ( outLayer == NULL )
		BAILWITHERROR( MP4BadParamErr );
	err = trak->getLayer( trak, outLayer );
bail:
	return err;
}

ISO_EXTERN ( ISOErr ) MJ2SetTrackDimensions( ISOTrack theTrack, u32 width, u32 height )
{
	MP4Err err;
	MP4TrackAtomPtr trak;
	
	if ( theTrack == 0 ) {
		BAILWITHERROR( MP4BadParamErr );
	}
	
	trak = (MP4TrackAtomPtr) theTrack;

	err = trak->setDimensions( trak, width, height );

bail:
	TEST_RETURN( err );
	return err;

}

ISO_EXTERN ( ISOErr ) MJ2GetTrackDimensions( ISOTrack theTrack, u32 *outWidth, u32 *outHeight )
{
	MP4Err err;
	MP4TrackAtomPtr trak;
	
	if ( theTrack == 0 ) {
		BAILWITHERROR( MP4BadParamErr );
	}
	
	trak = (MP4TrackAtomPtr) theTrack;

	if ( ( outWidth == NULL ) || ( outHeight == NULL ) )
		BAILWITHERROR( MP4BadParamErr );
	err = trak->getDimensions( trak, outWidth, outHeight );
bail:
	return err;
}

ISO_EXTERN ( ISOErr ) MJ2SetTrackVolume( ISOTrack theTrack, s16 volume )
{
	MP4Err err;
	MP4TrackAtomPtr trak;
	
	if ( theTrack == 0 ) {
		BAILWITHERROR( MP4BadParamErr );
	}
	
	trak = (MP4TrackAtomPtr) theTrack;
	assert( trak->setVolume );
	err = trak->setVolume( trak, volume );
bail:
	return err;
}

ISO_EXTERN ( ISOErr ) MJ2GetTrackVolume( ISOTrack theTrack, s16 *outVolume )
{
	MP4Err err;
	MP4TrackAtomPtr trak;
	
	if ( theTrack == 0 ) {
		BAILWITHERROR( MP4BadParamErr );
	}
	
	trak = (MP4TrackAtomPtr) theTrack;

	if ( outVolume == NULL )
		BAILWITHERROR( MP4BadParamErr );
	err = trak->getVolume( trak, outVolume );
bail:
	return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackReference( MP4Track theTrack, u32 referenceType, u32 referenceIndex, MP4Track *outReferencedTrack )
{
   MP4Err err;
   MP4TrackAtomPtr trak;
   MP4Movie moov;
   MP4TrackReferenceAtomPtr tref;
   MP4TrackReferenceTypeAtomPtr dpnd;
   u32 selectedTrackID;

   err = MP4NoErr;
   if ( (theTrack == NULL) || (referenceType == 0) || (referenceIndex == 0) || (outReferencedTrack == NULL) )
      BAILWITHERROR( MP4BadParamErr );
   err = MP4GetTrackMovie( theTrack, &moov ); if (err) goto bail;
   trak = (MP4TrackAtomPtr) theTrack;
   tref = (MP4TrackReferenceAtomPtr) trak->trackReferences;
   if ( tref == NULL )
      BAILWITHERROR( MP4BadParamErr );
   err = tref->findAtomOfType( tref, referenceType, (MP4AtomPtr*) &dpnd ); if (err) goto bail;
   if ( (dpnd == NULL) || (dpnd->trackIDCount < referenceIndex) )
      BAILWITHERROR( MP4BadParamErr );
   selectedTrackID = dpnd->trackIDs[ referenceIndex - 1 ];
   if ( selectedTrackID == 0 )
      BAILWITHERROR( MP4InvalidMediaErr );
   err = MP4GetMovieTrack( moov, selectedTrackID, outReferencedTrack ); if (err) goto bail;
  bail:
   TEST_RETURN( err );
   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackReferenceCount( MP4Track theTrack, u32 referenceType, u32 *outReferenceCount )
{
   MP4Err err;
   MP4TrackAtomPtr trak;
   MP4TrackReferenceAtomPtr tref;
   MP4TrackReferenceTypeAtomPtr dpnd;

   err = MP4NoErr;
   if ( (theTrack == NULL) || (referenceType == 0) || (outReferenceCount == NULL) )
      BAILWITHERROR( MP4BadParamErr );
   trak = (MP4TrackAtomPtr) theTrack;
   tref = (MP4TrackReferenceAtomPtr) trak->trackReferences;
   *outReferenceCount = 0;
   if ( tref != NULL )
   {
      err = tref->findAtomOfType( tref, referenceType, (MP4AtomPtr*) &dpnd );
      if ( (err == MP4NoErr) && (dpnd != NULL) )
		*outReferenceCount = dpnd->trackIDCount;
      else
		err = MP4NoErr;
   }
  bail:
   TEST_RETURN( err );
   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackMovie( MP4Track theTrack, MP4Movie *outMovie )
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   trackAtom = (MP4TrackAtomPtr) theTrack;
   err = MP4NoErr;
   if ( theTrack == NULL )
      BAILWITHERROR( MP4BadParamErr )
	*outMovie = (MP4Movie) trackAtom->moov;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackMedia( MP4Track theTrack, MP4Media *outMedia )
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   trackAtom = (MP4TrackAtomPtr) theTrack;
   err = MP4NoErr;
   if ( theTrack == NULL )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   if ( trackAtom->trackMedia )
   {
	 *outMedia = (MP4Media) trackAtom->trackMedia;
   }
   else
   {
        BAILWITHERROR( MP4InvalidMediaErr );
   }
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackMetadata( MP4Track theTrack, MP4Metadata *outMetadata )
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   trackAtom = (MP4TrackAtomPtr) theTrack;
   err = MP4NoErr;
   *outMetadata = NULL;
   if ( theTrack == NULL || outMetadata == NULL)
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   if(!trackAtom->trackMetadata)
   {
	   BAILWITHERROR(MP4BadDataErr);
   }
   *outMetadata = (MP4Metadata) trackAtom->trackMetadata;

bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackDecoderConfig( MP4Track theTrack, u32 *streamType, u32 *objectTypeId)
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   MP4MediaAtomPtr theMedia;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr       stbl;
   MP4SampleDescriptionAtomPtr stsd;
   GenericSampleEntryAtomPtr entry;
   MP4VisualSampleEntryAtomPtr ventry;
   MP4AtomPtr descAtom;
   MP4ESDAtomPtr esAtom;
   MP4DescriptorPtr theDescriptor;
   MP4ES_DescriptorPtr esDescriptor;
   MP4DecoderConfigDescriptorPtr configDescriptor;
   u32 count;
   int i;

   trackAtom = (MP4TrackAtomPtr) theTrack;
   err = MP4NoErr;
   if ( theTrack == NULL )
   {
      BAILWITHERROR( MP4BadParamErr );
   }

   theMedia = (MP4MediaAtomPtr) trackAtom->trackMedia;
   if(theMedia == NULL) BAILWITHERROR( MP4InvalidMediaErr );
   minf = (MP4MediaInformationAtomPtr) ((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL ) BAILWITHERROR( MP4InvalidMediaErr );
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL ) BAILWITHERROR( MP4InvalidMediaErr );
   stsd = (MP4SampleDescriptionAtomPtr) stbl->SampleDescription;
   if ( stsd == NULL ) BAILWITHERROR( MP4InvalidMediaErr );
   count = stsd->getEntryCount(stsd);
   for(i = 1; i <= (int)count; i++)
   {
	   stsd->getEntry( stsd, i , &entry);
	   if(entry == NULL) BAILWITHERROR( MP4InvalidMediaErr );
	   if(entry->type != MP4VisualSampleEntryAtomType) BAILWITHERROR( MP4BadDataErr );
	   ventry = (MP4VisualSampleEntryAtomPtr) entry;
	   descAtom = ventry->ESDAtomPtr;
	   if(descAtom->type == MP4ESDAtomType)
	   {
		   esAtom = (MP4ESDAtomPtr)descAtom;
	   }
	   else
	   {
		   err = MP4BadDataErr;
		   continue;
	   }
	   theDescriptor = esAtom->descriptor;
	   if(theDescriptor->tag == MP4ES_DescriptorTag)
	   {
		   esDescriptor = (MP4ES_DescriptorPtr)theDescriptor;
	   }
	   else
	   {
		   err = MP4BadDataErr;
		   continue;
	   }
	   theDescriptor = esDescriptor->decoderConfig;
	   if(theDescriptor->tag == MP4DecoderConfigDescriptorTag)
	   {
		   configDescriptor = (MP4DecoderConfigDescriptorPtr)theDescriptor;
	   }
	   else
	   {
		   err = MP4BadDataErr;
		   continue;
	   }
	   *streamType = configDescriptor->streamType;
	   *objectTypeId = configDescriptor->objectTypeIndication;
	   err = 0;
	   break;
   }

bail:
   TEST_RETURN( err );

   return err;
}


MP4_EXTERN ( MP4Err ) MP4GetTrackUserData( MP4Track theTrack, MP4UserData* outUserData )
{
   MP4Err err;
   MP4UserData udta;
   MP4TrackAtomPtr trackAtom;
   trackAtom = (MP4TrackAtomPtr) theTrack;
   err = MP4NoErr;
   if ( theTrack == NULL )
      BAILWITHERROR( MP4BadParamErr )
	 udta = (MP4UserData) trackAtom->udta;
   if ( trackAtom->udta == 0 )
   {
      err = MP4NewUserData( &udta ); if (err) goto bail;
      err = trackAtom->addAtom( trackAtom, (MP4AtomPtr) udta ); if (err) goto bail;
   }
   *outUserData = (MP4UserData) udta;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4NewTrackMedia( MP4Track theTrack, MP4Media *outMedia, u32 mediaType, u32 timeScale, MP4Handle dataReference )
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   trackAtom = (MP4TrackAtomPtr) theTrack;
   err = MP4NoErr;
   if ( theTrack == NULL )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   err = trackAtom->newMedia( trackAtom, outMedia, mediaType, timeScale, dataReference ); if (err) goto bail;

  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err )
MP4NewTrackMetadata( MP4Track theTrack, MP4Metadata *outMetadata, u32 handlerType, MP4Handle dataReference )
{
   MP4Err err;
   MP4TrackAtomPtr trackAtom;
   trackAtom = (MP4TrackAtomPtr) theTrack;
   err = MP4NoErr;
   if ( theTrack == NULL )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   err = trackAtom->newMetadata( trackAtom, outMetadata, handlerType, dataReference ); if (err) goto bail;

  bail:
   TEST_RETURN( err );

   return err;
}
/** KW 2007 */
MP4_EXTERN ( MP4Err ) MP4SetVisualSampleDimensions( MP4Track theTrack, u32 width, u32 height )
{
   MP4Err err;
   MP4TrackAtomPtr trak;
   MP4MediaAtomPtr theMedia;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr       stbl;
   MP4SampleDescriptionAtomPtr stsd;
   GenericSampleEntryAtomPtr entry;
   MP4VisualSampleEntryAtomPtr ventry;
   u32 count;
   int i;
   err = MP4NoErr;

   trak = (MP4TrackAtomPtr)theTrack;
   if ( (trak == 0) || (width < 0) || (height < 0) ) BAILWITHERROR( MP4BadParamErr );
   theMedia = (MP4MediaAtomPtr) trak->trackMedia;
   if(theMedia == NULL) BAILWITHERROR( MP4InvalidMediaErr );
   minf = (MP4MediaInformationAtomPtr) ((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL ) BAILWITHERROR( MP4InvalidMediaErr );
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL ) BAILWITHERROR( MP4InvalidMediaErr );
   stsd = (MP4SampleDescriptionAtomPtr) stbl->SampleDescription;
   if ( stsd == NULL ) BAILWITHERROR( MP4InvalidMediaErr );
   count = stsd->getEntryCount(stsd);
   for(i = 1; i <= (int)count; i++)
   {
	   stsd->getEntry( stsd, i , &entry);
	   if(entry == NULL) BAILWITHERROR( MP4InvalidMediaErr );
	   if(entry->type != MP4VisualSampleEntryAtomType) BAILWITHERROR( MP4BadDataErr );
	   ventry = (MP4VisualSampleEntryAtomPtr) entry;
	   assert(ventry->setDimensions);
	   ventry->setDimensions(ventry, width, height);
   }

bail:
   TEST_RETURN( err );

   return err;
}


MP4_EXTERN ( MP4Err ) MP4InsertMediaIntoTrack( MP4Track track, s32 trackStartTime, u32 mediaStartTime, u64 mediaDuration, s32 mediaRate )
{
   MP4Err err;
   MP4TrackAtomPtr trak;

   err = MP4NoErr;
   if ( (track == 0) || (mediaRate < 0) || (mediaRate > 1) )
      BAILWITHERROR( MP4BadParamErr );
   trak = (MP4TrackAtomPtr) track;
   if ( (trackStartTime == 0) && (mediaStartTime == 0) && (mediaRate == 1) )
   {
      if ( mediaDuration == 0 )
         BAILWITHERROR( MP4BadParamErr );
   }
   else
   {
      /* need an edit list */
      MP4EditAtomPtr     edts;
      MP4EditListAtomPtr elst;
		
      edts = (MP4EditAtomPtr) trak->trackEditAtom;

      if ( edts == 0 )
      {
         /* no edits yet */
         MP4Err MP4CreateEditAtom( MP4EditAtomPtr *outAtom );
			
         err = MP4CreateEditAtom( &edts ); if (err) goto bail;
         err = trak->addAtom( trak, (MP4AtomPtr) edts ); if (err) goto bail;
      }
      elst = (MP4EditListAtomPtr) edts->editListAtom;
      if ( elst == 0 )
      {
         MP4Err MP4CreateEditListAtom( MP4EditListAtomPtr *outAtom );
			
         err = MP4CreateEditListAtom( &elst ); if (err) goto bail;
         err = edts->addAtom( edts, (MP4AtomPtr) elst ); if (err) goto bail;
      }
      err = elst->insertSegment( elst, trackStartTime, mediaStartTime, mediaDuration, mediaRate ); if (err) goto bail;
   }
   if ( trak->moov->inMemoryDataHandler )
   {
      MP4MediaAtomPtr mdia;
      mdia = (MP4MediaAtomPtr) trak->trackMedia;
      if ( mdia )
      {
         MP4MediaInformationAtomPtr minf;
         minf = (MP4MediaInformationAtomPtr) mdia->information;
         if ( minf )
         {
            if ( minf->dataHandler == 0 )
            {
               minf->dataHandler = trak->moov->inMemoryDataHandler;
               if ( minf->dataEntryIndex == 0 )
               {
                  minf->dataEntryIndex = 1;
               }
            }
         }
      }
   }
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetTrackOffset( MP4Track track, u32 *outMovieOffsetTime )
{
   MP4Err err;
   MP4TrackAtomPtr trak;
   MP4EditAtomPtr edts;
   MP4EditListAtomPtr elst;

   err = MP4NoErr;
   if ( (track == 0) || (outMovieOffsetTime == 0) )
      BAILWITHERROR( MP4BadParamErr );

   /* see if we have an edit list */	
   trak = (MP4TrackAtomPtr) track;
   edts = (MP4EditAtomPtr) trak->trackEditAtom;

   if ( edts == 0 )
   {
      *outMovieOffsetTime = 0;
   }
   else
   {
      elst = (MP4EditListAtomPtr) edts->editListAtom;
      if ( elst == 0 )
      {
         *outMovieOffsetTime = 0;
      }
      else
      {
         err = elst->getTrackOffset( elst, outMovieOffsetTime ); if (err) goto bail;
      }
   }
  bail:
   TEST_RETURN( err );

   return err;
}
MP4_EXTERN ( MP4Err ) MP4SetTrackOffset( MP4Track track, u32 movieOffsetTime )
{
   MP4Err err;
   MP4TrackAtomPtr trak;
   MP4EditAtomPtr edts;
   MP4EditListAtomPtr elst;
   u64 trackDuration;

   err = MP4NoErr;
   if ( (track == 0) )
      BAILWITHERROR( MP4BadParamErr );

   err = MP4GetTrackDuration( track, &trackDuration ); if (err) goto bail;

   /* need an edit list */	
   trak = (MP4TrackAtomPtr) track;
   edts = (MP4EditAtomPtr) trak->trackEditAtom;

   if ( edts == 0 )
   {
      /* no edits yet */
      MP4Err MP4CreateEditAtom( MP4EditAtomPtr *outAtom );
		
      err = MP4CreateEditAtom( &edts ); if (err) goto bail;
      err = trak->addAtom( trak, (MP4AtomPtr) edts ); if (err) goto bail;
   }
   elst = (MP4EditListAtomPtr) edts->editListAtom;
   if ( elst == 0 )
   {
      MP4Err MP4CreateEditListAtom( MP4EditListAtomPtr *outAtom );
		
      err = MP4CreateEditListAtom( &elst ); if (err) goto bail;
      err = edts->addAtom( edts, (MP4AtomPtr) elst ); if (err) goto bail;
   }
   err = elst->setTrackOffset( elst, movieOffsetTime, trackDuration ); if (err) goto bail;
bail:
   TEST_RETURN( err );

   return err;
}

MP4Err MP4GetTrackReferenceType( MP4Track track, u32 atomType, MP4TrackReferenceTypeAtomPtr *outAtom )
{
   MP4Err err;
   MP4TrackReferenceAtomPtr     tref;
   MP4TrackReferenceTypeAtomPtr foundAtom;
   MP4TrackAtomPtr              trak;
	
   err = MP4NoErr;
   trak = (MP4TrackAtomPtr) track;

   if ( (track == 0) || (outAtom == 0) )
      BAILWITHERROR( MP4BadParamErr )
	 foundAtom = 0;
   tref = (MP4TrackReferenceAtomPtr) trak->trackReferences;
   if ( tref )
   {
      err = tref->findAtomOfType( tref, atomType, (MP4AtomPtr*) &foundAtom ); if (err) goto bail;
   }
   *outAtom = foundAtom;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4TrackTimeToMediaTime( MP4Track theTrack, u64 inTrackTime, s64 *outMediaTime )
{
   MP4Err err;
   MP4Movie theMovie;
   MP4Media theMedia;
   u32 movieTimeScale;
   u32 mediaTimeScale;
   s64 mediaTime;
   MP4TrackAtomPtr    trak;
   MP4EditAtomPtr     edts;
   MP4EditListAtomPtr elst;
	
   err = MP4NoErr;
   if ( (theTrack == 0) || (outMediaTime == 0) )
      BAILWITHERROR( MP4BadParamErr )

   err = MP4GetTrackMovie( theTrack, &theMovie ); if (err) goto bail;
   err = MP4GetMovieTimeScale( theMovie, &movieTimeScale ); if (err) goto bail;
   err = MP4GetTrackMedia( theTrack, &theMedia ); if (err) goto bail;
   err = MP4GetMediaTimeScale( theMedia, &mediaTimeScale ); if (err) goto bail;
	
   if ( (movieTimeScale == 0) )
      BAILWITHERROR( MP4InvalidMediaErr )

	 trak = (MP4TrackAtomPtr) theTrack;
   edts = (MP4EditAtomPtr) trak->trackEditAtom;
   if ( edts == 0 )
   {
      /* no edits */
      mediaTime = (inTrackTime / movieTimeScale) * mediaTimeScale;
      *outMediaTime = mediaTime;
   }
   else
   {
      /* edit atom is present */
      elst = (MP4EditListAtomPtr) edts->editListAtom;
      if ( elst == 0 )
      {
	 /* edit atom but no edit list, hmm... */
	 mediaTime = (inTrackTime / movieTimeScale) * mediaTimeScale;
	 *outMediaTime = mediaTime;
      }
      else
      {
	 /* edit list is present */
	 u32 mediaRate;
	 u64 prior;
	 u64 next;
	 err = elst->getTimeAndRate( (MP4AtomPtr) elst, inTrackTime, movieTimeScale,
				     mediaTimeScale, &mediaTime, &mediaRate, &prior, &next ); if (err) goto bail;
	 *outMediaTime = mediaTime;
      }
   }
  bail:
   TEST_RETURN( err );

   return err;
}
