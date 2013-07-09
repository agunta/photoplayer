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
	$Id: MediaAtom.c,v 1.4 2001/06/05 14:06:23 rtm Exp $
*/

#include "MP4Atoms.h"
#include "MP4Descriptors.h"
#include <stdlib.h>

static void destroy( MP4AtomPtr s )
{
	MP4Err err;
	MP4MediaAtomPtr self;
	u32 i;
	err = MP4NoErr;
	self = (MP4MediaAtomPtr) s;
	if ( self == NULL )
		BAILWITHERROR( MP4BadParamErr )
	DESTROY_ATOM_LIST
	
	if ( self->super )
		self->super->destroy( s );
  bail:
	TEST_RETURN( err );

	return;
}

static 	MP4Err calculateDuration( struct MP4MediaAtom *self )
{
   MP4Err err;
   u32 mediaDuration;
   MP4MediaInformationAtomPtr minf;
   MP4MediaHeaderAtomPtr      mdhd;

   err = MP4NoErr;
   minf = (MP4MediaInformationAtomPtr) self->information;
   if ( minf == NULL )
      BAILWITHERROR( MP4InvalidMediaErr );
   mdhd = (MP4MediaHeaderAtomPtr) self->mediaHeader;
   if ( mdhd == NULL )
      BAILWITHERROR( MP4InvalidMediaErr );
   err = minf->getMediaDuration( minf, &mediaDuration ); if (err) goto bail;
   mdhd->duration = mediaDuration;
  bail:
   TEST_RETURN( err );

   return err;
}

static MP4Err mdatMoved( struct MP4MediaAtom* self, s32 mdatOffset )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;	
   err = MP4NoErr;

   minf = (MP4MediaInformationAtomPtr) self->information;
   if ( minf == NULL )
      BAILWITHERROR( MP4InvalidMediaErr );
   err = minf->mdatMoved( minf, mdatOffset ); if (err) goto bail;	
  bail:
   TEST_RETURN( err );
   return err;
}

static MP4Err addAtom( MP4MediaAtomPtr self, MP4AtomPtr atom )
{
   MP4Err err;
   err = MP4NoErr;
   if ( atom == NULL )
      BAILWITHERROR( MP4BadParamErr );
   err = MP4AddListEntry( atom, self->atomList ); if (err) goto bail;
   switch( atom->type )
   {
      case MP4MediaHeaderAtomType:
		if ( self->mediaHeader )
	  	  BAILWITHERROR( MP4BadDataErr )
		self->mediaHeader = atom;
	 break;
			
      case MP4HandlerAtomType:
		if ( self->handler )
			BAILWITHERROR( MP4BadDataErr )
		self->handler = atom;
	 break;
			
      case MP4MediaInformationAtomType:
		if ( self->information )
			BAILWITHERROR( MP4BadDataErr )
		self->information = atom;
	 break;
   }
  bail:
   TEST_RETURN( err );

   return err;
}

static MP4Err setupNew( struct MP4MediaAtom *self, MP4AtomPtr track, u32 mediaType, u32 timeScale, MP4Handle dataHandlerH )
{
   MP4Err MP4CreateMediaHeaderAtom( MP4MediaHeaderAtomPtr *outAtom );
   MP4Err MP4CreateMediaInformationAtom( MP4MediaInformationAtomPtr *outAtom );
   MP4Err MP4CreateHandlerAtom( MP4HandlerAtomPtr *outAtom );
   MP4Err MP4GetCurrentTime( u64 *outTime );
   void MP4TypeToString( u32 inType, char* ioStr );

   char name[ 8 ];
   MP4Err err;
   u64 currentTime;
   MP4MediaHeaderAtomPtr mdhd;
   MP4MediaInformationAtomPtr minf;
   MP4HandlerAtomPtr hdlr;
   MP4AtomPtr mdat;
	
   err = MP4NoErr;
   self->mediaTrack = track;
   mdat = ((MP4TrackAtomPtr) track)->mdat;
   err = MP4CreateMediaHeaderAtom( &mdhd ); if (err) goto bail;
   err = MP4CreateMediaInformationAtom( &minf ); if (err) goto bail;
   err = MP4CreateHandlerAtom( &hdlr ); if (err) goto bail;
	
   err = MP4GetCurrentTime( &currentTime ); if (err) goto bail;
   mdhd->timeScale = timeScale;
   mdhd->creationTime = currentTime;
   mdhd->modificationTime = currentTime;
   mdhd->packedLanguage = 21956;
	
   hdlr->handlerType = mediaType;
   MP4TypeToString( mediaType, name );
   hdlr->nameLength = 5;
   hdlr->nameUTF8 = calloc( 1, hdlr->nameLength );
   TESTMALLOC( hdlr->nameUTF8 )
   memcpy( hdlr->nameUTF8, name, hdlr->nameLength );
	
   err = minf->setupNewMedia( minf, mediaType, dataHandlerH, mdat );

   err = addAtom( self, (MP4AtomPtr) mdhd ); if (err) goto bail;
   err = addAtom( self, (MP4AtomPtr) hdlr ); if (err) goto bail;
   err = addAtom( self, (MP4AtomPtr) minf ); if (err) goto bail;
  bail:
   TEST_RETURN( err );

   return err;
}

static MP4Err addSampleReference( struct MP4MediaAtom *self, u64 dataOffset, u32 sampleCount,
				  MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
				  MP4Handle decodingOffsetsH, MP4Handle syncSamplesH )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
	
   err = MP4NoErr;
   minf = (MP4MediaInformationAtomPtr) self->information;
   assert( minf );
   assert( minf->addSampleReference );
   err = minf->addSampleReference( minf, dataOffset, sampleCount, durationsH, sizesH, sampleEntryH, decodingOffsetsH, syncSamplesH ); if (err) goto bail;
  bail:
   TEST_RETURN( err );

   return err;
}

static 	MP4Err addSamples( struct MP4MediaAtom *self, MP4Handle sampleH, u32 sampleCount,
			   MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
			   MP4Handle decodingOffsetsH, MP4Handle syncSamplesH )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   
   err = MP4NoErr;
   minf = (MP4MediaInformationAtomPtr) self->information;
   assert( minf );
   assert( minf->addSamples );

   /*JLF 06/00 (jlefeuvre@e-vue.com)
		Check for the sample type, if it's an OD command frame, rewrite it in case
		it contains ESDescriptors instead of ES_ID_RefDescriptors
   */
   if ( ((MP4HandlerAtomPtr)self->handler)->handlerType == MP4ObjectDescriptorHandlerType) {

	   MP4Err MP4ParseODFrame(struct MP4MediaAtom *self, MP4Handle sampleH, MP4Handle sizesH);

	   /* Warning, only one OD-AU can be handled within each call.*/
		if (sampleCount != 1) {
			BAILWITHERROR( MP4NotImplementedErr)
		}
		/* rewrite the OD Access Unit (don't forget the size !!) */
		err = MP4ParseODFrame(self, sampleH, sizesH); if (err) goto bail;
   }
   
   err = minf->addSamples( minf, sampleH, sampleCount, durationsH, sizesH, sampleEntryH, decodingOffsetsH, syncSamplesH ); if (err) goto bail;
  bail:
   TEST_RETURN( err );

   return err;
}

static MP4Err serialize( struct MP4Atom* s, char* buffer )
{
   MP4Err err;
   MP4MediaAtomPtr self = (MP4MediaAtomPtr) s;
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
   MP4MediaAtomPtr self = (MP4MediaAtomPtr) s;
   err = MP4NoErr;
	
   err = MP4CalculateBaseAtomFieldSize( s ); if (err) goto bail;
   ADD_ATOM_LIST_SIZE( atomList );
  bail:
   TEST_RETURN( err );

   return err;
}


static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
   PARSE_ATOM_LIST(MP4MediaAtom)
      bail:
   TEST_RETURN( err );

   return err;
}

MP4Err MP4CreateMediaAtom( MP4MediaAtomPtr *outAtom )
{
   MP4Err err;
   MP4MediaAtomPtr self;
	
   self = (MP4MediaAtomPtr) calloc( 1, sizeof(MP4MediaAtom) );
   TESTMALLOC( self )

   err = MP4CreateBaseAtom( (MP4AtomPtr) self );
   if ( err ) goto bail;
   self->type = MP4MediaAtomType;
   self->name                = "media";
   self->createFromInputStream = (cisfunc) createFromInputStream;
   self->destroy             = destroy;
   err = MP4MakeLinkedList( &self->atomList ); if (err) goto bail;
   self->calculateSize         = calculateSize;
   self->serialize             = serialize;
   self->addSamples = addSamples;
   self->calculateDuration = calculateDuration;
   self->setupNew = setupNew;
   self->mdatMoved = mdatMoved;
   self->addSampleReference = addSampleReference;
   *outAtom = self;
  bail:
   TEST_RETURN( err );

   return err;
}

/*JLF 06/00 (jlefeuvre@e-vue.com) Rewriting the OD frame and 
  replace ESDescriptors by ES_ID_RefDescriptors */
MP4Err MP4ParseODFrame(struct MP4MediaAtom *self, MP4Handle sampleH, MP4Handle sizesH)
{
	MP4Err err = MP4NoErr;

	u32 commandSize = 0;
	u32 numCmds = 0;
	char *encodedCmds = NULL;
	char *buffer;
	u32 i;
	u32 j;
	u32 frameSize;
	MP4InputStreamPtr is;
	MP4LinkedList descList;
	MP4TrackReferenceAtomPtr tref;
	MP4TrackAtomPtr trak;
	MP4TrackReferenceTypeAtomPtr mpod;

	MP4Err MP4CreateTrackReferenceAtom(MP4TrackReferenceAtomPtr *outAtom);
	MP4Err MP4CreateTrackReferenceTypeAtom( u32 atomType, MP4TrackReferenceTypeAtomPtr *outAtom );
	MP4Err MP4ParseCommand( MP4InputStreamPtr inputStream, MP4DescriptorPtr *outDesc );
	MP4Err MP4CreateES_ID_RefDescriptor( u32 tag, u32 size, u32 bytesRead, MP4DescriptorPtr *outDesc );

	err = MP4GetHandleSize( sampleH, &frameSize); if (err) goto bail;
	err = MP4CreateMemoryInputStream( *sampleH, frameSize, &is ); if (err) goto bail;
	err = MP4MakeLinkedList( &descList ); if (err) goto bail;


	trak = (MP4TrackAtomPtr) self->mediaTrack;
	tref = (MP4TrackReferenceAtom *) trak->trackReferences;
	if (tref == NULL) {
		err = MP4CreateTrackReferenceAtom(&tref); if (err) goto bail;
		trak->addAtom(trak, (MP4AtomPtr) tref); if (err) goto bail;
	}

	err = tref->findAtomOfType(tref, MP4ODTrackReferenceAtomType, (MP4AtomPtr *) &mpod);
	if (err) goto bail;
	if (mpod == NULL) {
		err = MP4CreateTrackReferenceTypeAtom(MP4ODTrackReferenceAtomType, &mpod);
		tref->addAtom(tref, (MP4AtomPtr) mpod);
	}

	while (is->available > 0) {

		MP4DescriptorPtr             desc;
		MP4ObjectDescriptorUpdatePtr odUpdate;
		MP4ESDescriptorUpdatePtr     esUpdate;
		u32                          odCount;
		u32                          esCount;

		err = MP4ParseCommand(is, &desc);
		if (!desc) goto bail;
		numCmds += 1;

		switch (desc->tag) {
			/* ObjectDescriptor UPDATE COMMAND */
		case MP4ObjectDescriptorUpdateTag:
			odUpdate = (MP4ObjectDescriptorUpdatePtr) desc;
			err = MP4GetListEntryCount( odUpdate->objectDescriptors, &odCount ); if (err) goto bail;
			if (err) goto bail;

			for ( i = 0; i < odCount; i++ )	{
				MP4ObjectDescriptorPtr od;
				err = MP4GetListEntry( odUpdate->objectDescriptors, i, (char **) &od ); if (err) goto bail;
				err = MP4GetListEntryCount(od->ESDescriptors, &esCount ); if (err) goto bail;

				/* Guido 11/10/00 : also rewrite the OD TAG */
				od->tag = MP4_OD_Tag;

				for( j = 0; j < esCount; j++ ) {
					MP4ES_ID_RefDescriptorPtr ref;
					MP4ES_DescriptorPtr esd;

					err = MP4GetListEntry( od->ESDescriptors, j, (char**) &esd); if (err) goto bail;
					if (err) goto bail;

					/* TO DO in libisomp4 : Check if this ES is in the movie
					   ES_ID == TrackID */
					err = mpod->addTrackID(mpod, esd->ESID); 
					if (err) goto bail;

					/* The ref_index == mpod->TrackIdCount after adding the ref (NON NULL) */
					err = MP4CreateES_ID_RefDescriptor(MP4ES_ID_RefDescriptorTag, 0 , 0, (MP4DescriptorPtr *) &ref);
					if (err) goto bail;
					assert(ref);
					ref->refIndex = mpod->trackIDCount;
					err = od->addDescriptor((MP4DescriptorPtr) od, (MP4DescriptorPtr) ref);
					if (err) goto bail;

				}
				/* Remove the ESDescriptor List from each OD present in the ObjectDescriptor !! */
				if (od->ESDescriptors) {
					DESTROY_DESCRIPTOR_LIST_V(od->ESDescriptors);
					od->ESDescriptors = NULL;
				}
			}

			break;
			

			/* ElementaryStreamDescriptor UPDATE COMMAND */
		case MP4ESDescriptorUpdateTag:
				esUpdate = (MP4ESDescriptorUpdatePtr) desc;
				err = MP4GetListEntryCount( esUpdate->ESDescriptors, &esCount ); if (err) goto bail;
				for (j = 0; j < esCount; j++) {
					MP4ES_ID_RefDescriptorPtr ref;
					MP4ES_DescriptorPtr esd;

					err = MP4GetListEntry( esUpdate->ESDescriptors, j, (char **) &esd ); if (err) goto bail;
					
					/* TO DO in libisomp4 : Check if this ES is in the movie */
					/* ES_ID == TrackID */
					err = mpod->addTrackID(mpod, esd->ESID); 
					if (err ) goto bail;
					
					err = MP4CreateES_ID_RefDescriptor(MP4ES_ID_RefDescriptorTag, 0 ,0, (MP4DescriptorPtr *) &ref);
					if (err) goto bail;
					assert(ref);
					ref->refIndex = mpod->trackIDCount;
					err = esUpdate->addDescriptor((MP4DescriptorPtr) esUpdate, (MP4DescriptorPtr)  ref);
					if (err) goto bail;
				}

				/* Remove the ESDescriptors from the ES Listr present in this update!! */
				if (esUpdate->ESDescriptors) {
					DESTROY_DESCRIPTOR_LIST_V(esUpdate->ESDescriptors);
					esUpdate->ESDescriptors = NULL;
				}

				break;
			
			default:
				break;
		}
		err = MP4AddListEntry( desc, descList ); if (err) goto bail;
		err = desc->calculateSize( desc ); if (err) goto bail;
		commandSize += desc->size;
	}

	encodedCmds = calloc( 1, commandSize );
	buffer = encodedCmds;
	TESTMALLOC( encodedCmds )

	for ( i = 0; i < numCmds; i++ ) {
		MP4DescriptorPtr desc;
		err = MP4GetListEntry( descList, i, (char **) &desc ); if (err) goto bail;
		err = desc->serialize( desc, buffer ); if (err) goto bail;
		buffer += desc->size;
	}
	err = MP4SetHandleSize( sampleH, commandSize ); if (err) goto bail;
	memcpy( *sampleH, encodedCmds, commandSize );
	free( encodedCmds );

	*(u32 *) *sizesH = commandSize;

bail:
	if (is)
		is->destroy(is);
	if (descList) {
		DESTROY_DESCRIPTOR_LIST_V(descList);
	}
	TEST_RETURN( err );
	return err;
}