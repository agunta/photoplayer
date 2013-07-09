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
	$Id: SampleTableAtom.c,v 1.2 2000/05/17 08:01:31 francesc Exp $
*/

#include "MP4Atoms.h"
#include <stdlib.h>

static void destroy( MP4AtomPtr s )
{
	MP4Err err;
	MP4SampleTableAtomPtr self;
	u32 i;
	err = MP4NoErr;
	self = (MP4SampleTableAtomPtr) s;
	if ( self == NULL )
		BAILWITHERROR( MP4BadParamErr )
	DESTROY_ATOM_LIST
	
	if ( self->super )
		self->super->destroy( s );
bail:
	TEST_RETURN( err );

	return;
}

#define ADDCASE( atomName ) \
		case MP4 ## atomName ## AtomType: \
			if ( self->atomName ) \
				BAILWITHERROR( MP4BadDataErr ) \
			self->atomName = atom; \
			break

static MP4Err addAtom( MP4SampleTableAtomPtr self, MP4AtomPtr atom )
{
	MP4Err err;
	err = MP4NoErr;
	err = MP4AddListEntry( atom, self->atomList ); if (err) goto bail;
	switch( atom->type )
	{
		ADDCASE( TimeToSample );
		ADDCASE( CompositionOffset );
		ADDCASE( SyncSample );
		ADDCASE( SampleDescription );
		ADDCASE( SampleSize );
		ADDCASE( SampleToChunk );
		
		case MP4ChunkLargeOffsetAtomType:
		ADDCASE( ChunkOffset );

		ADDCASE( ShadowSync );
		ADDCASE( DegradationPriority );
	}
bail:
	TEST_RETURN( err );

	return err;
}


static MP4Err calculateDuration( struct MP4SampleTableAtom *self, u32 *outDuration )
{
	MP4Err err;
	MP4TimeToSampleAtomPtr ctts;
	
	err = MP4NoErr;
	if ( outDuration == NULL )
		BAILWITHERROR( MP4BadParamErr )

	ctts = (MP4TimeToSampleAtomPtr) self->TimeToSample;
	if ( ctts == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	err = ctts->getTotalDuration( ctts, outDuration ); if (err) goto bail;

bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err setSampleEntry( struct MP4SampleTableAtom *self, MP4AtomPtr entry )
{
	MP4Err err;
	MP4SampleDescriptionAtomPtr stsd;
	
	err = MP4NoErr;
	if ( entry == NULL )
		BAILWITHERROR( MP4BadParamErr )
	
	stsd = (MP4SampleDescriptionAtomPtr) self->SampleDescription;
	if ( stsd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	err = stsd->addEntry( stsd, entry ); if (err) goto bail;
	self->currentSampleEntry = entry;
bail:
	TEST_RETURN( err );

	return err;
}

static 	MP4Err getCurrentDataReferenceIndex( struct MP4SampleTableAtom *self, u32 *outDataReferenceIndex )
{
	GenericSampleEntryAtomPtr entry;
	MP4Err err;
	
	err = MP4NoErr;
	entry = (GenericSampleEntryAtomPtr) self->currentSampleEntry;
	if ( entry == NULL )
		BAILWITHERROR( MP4InvalidMediaErr )
	*outDataReferenceIndex = entry->dataReferenceIndex;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err addSamples( struct MP4SampleTableAtom *self,
                          u32 sampleCount, u32 sampleOffset, MP4Handle durationsH,
                          MP4Handle sizesH, MP4Handle decodingOffsetsH, 
                          MP4Handle syncSamplesH )
{
   u32 beginningSampleNumber;
   u32 sampleDescriptionIndex;
   MP4SampleDescriptionAtomPtr stsd;
   MP4TimeToSampleAtomPtr      stts;
   MP4SampleSizeAtomPtr        stsz;
   MP4SampleToChunkAtomPtr     stsc;
   MP4ChunkOffsetAtomPtr       stco;
   MP4CompositionOffsetAtomPtr    dtts;
   MP4SyncSampleAtomPtr        stss;
	
   MP4Err err;
	
   err = MP4NoErr;
   stsd = (MP4SampleDescriptionAtomPtr) self->SampleDescription;
   if ( stsd == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stts = (MP4TimeToSampleAtomPtr) self->TimeToSample;
   if ( stts == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsz = (MP4SampleSizeAtomPtr) self->SampleSize;
   if ( stsz == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsc = (MP4SampleToChunkAtomPtr) self->SampleToChunk;
   if ( stsc == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stco = (MP4ChunkOffsetAtomPtr) self->ChunkOffset;
   if ( stco == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dtts = NULL;
   if ( decodingOffsetsH != NULL )
   {
      if (self->CompositionOffset == NULL )
      {
         MP4Err MP4CreateCompositionOffsetAtom( MP4CompositionOffsetAtomPtr *outAtom );
         err = MP4CreateCompositionOffsetAtom( &dtts ); if (err) goto bail;
         err = addAtom( self, (MP4AtomPtr) dtts ); if (err) goto bail;
      }
      dtts = (MP4CompositionOffsetAtomPtr) self->CompositionOffset;
   }
   stss = NULL;
   if ( syncSamplesH != NULL )
   {
      if (self->SyncSample == NULL )
      {
         /*
           create a sync sample atom when we get the first
           indication that all samples are not sync samples
          */
         u32 samplesInHandle;
         err = MP4GetHandleSize( syncSamplesH, &samplesInHandle ); if (err) goto bail;
         samplesInHandle /= 4;
         if ( samplesInHandle != sampleCount )
         {
            MP4Err MP4CreateSyncSampleAtom( MP4SyncSampleAtomPtr *outAtom );
            err = MP4CreateSyncSampleAtom( &stss ); if (err) goto bail;
            err = addAtom( self, (MP4AtomPtr) stss ); if (err) goto bail;
         }
      }
      stss = (MP4SyncSampleAtomPtr) self->SyncSample;
   }
   beginningSampleNumber = stsz->sampleCount + 1;
   err = stts->addSamples( stts, sampleCount, durationsH ); if (err) goto bail;
   if ( dtts )
   {
      err = dtts->addSamples( dtts, beginningSampleNumber, sampleCount, decodingOffsetsH ); if (err) goto bail;
   }
   if ( stss )
   {
      err = stss->addSamples( stss, beginningSampleNumber, sampleCount, syncSamplesH ); if (err) goto bail;
   }
   sampleDescriptionIndex = stsd->getEntryCount( stsd );
   if ( sampleDescriptionIndex == 0 )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   err = stsz->addSamples( stsz, sampleCount, sizesH ); if (err) goto bail;
   err = stsc->setEntry( stsc, beginningSampleNumber, sampleCount, sampleDescriptionIndex ); if (err) goto bail;
   err = stco->addOffset( stco, sampleOffset ); if (err) goto bail;

  bail:
   TEST_RETURN( err );

   return err;
}

static MP4Err serialize( struct MP4Atom* s, char* buffer )
{
	MP4Err err;
	MP4SampleTableAtomPtr self = (MP4SampleTableAtomPtr) s;
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
	MP4SampleTableAtomPtr self = (MP4SampleTableAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4CalculateBaseAtomFieldSize( s ); if (err) goto bail;
	ADD_ATOM_LIST_SIZE( atomList );
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err setupNew( struct MP4SampleTableAtom *self )
{
	MP4Err MP4CreateTimeToSampleAtom( MP4TimeToSampleAtomPtr *outAtom );
	MP4Err MP4CreateSampleDescriptionAtom( MP4SampleDescriptionAtomPtr *outAtom );
	MP4Err MP4CreateSampleSizeAtom( MP4SampleSizeAtomPtr *outAtom );
	MP4Err MP4CreateSampleToChunkAtom( MP4SampleToChunkAtomPtr *outAtom );
	MP4Err MP4CreateChunkOffsetAtom( MP4ChunkOffsetAtomPtr *outAtom );

	MP4Err err;
	MP4TimeToSampleAtomPtr ctts;
	MP4SampleDescriptionAtomPtr stsd;
	MP4SampleSizeAtomPtr        stsz;
	MP4SampleToChunkAtomPtr     stsc;
	MP4ChunkOffsetAtomPtr       stco;
	
	err = MP4NoErr;
	
	err = MP4CreateTimeToSampleAtom( &ctts ); if (err) goto bail;
	err = MP4CreateSampleDescriptionAtom( &stsd ); if (err) goto bail;
	err = MP4CreateSampleSizeAtom( &stsz ); if (err) goto bail;
	err = MP4CreateSampleToChunkAtom( &stsc ); if (err) goto bail;
	err = MP4CreateChunkOffsetAtom( &stco ); if (err) goto bail;
	
	err = addAtom( self, (MP4AtomPtr) ctts ); if (err) goto bail;
	err = addAtom( self, (MP4AtomPtr) stsd ); if (err) goto bail;
	err = addAtom( self, (MP4AtomPtr) stsz ); if (err) goto bail;
	err = addAtom( self, (MP4AtomPtr) stsc ); if (err) goto bail;
	err = addAtom( self, (MP4AtomPtr) stco ); if (err) goto bail;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	PARSE_ATOM_LIST(MP4SampleTableAtom)
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4CreateSampleTableAtom( MP4SampleTableAtomPtr *outAtom )
{
	MP4Err err;
	MP4SampleTableAtomPtr self;
	
	self = (MP4SampleTableAtomPtr) calloc( 1, sizeof(MP4SampleTableAtom) );
	TESTMALLOC( self )

	err = MP4CreateBaseAtom( (MP4AtomPtr) self );
	if ( err ) goto bail;
	self->type = MP4SampleTableAtomType;
	self->name                = "sample table";
	err = MP4MakeLinkedList( &self->atomList ); if (err) goto bail;
	self->createFromInputStream        = (cisfunc) createFromInputStream;
	self->destroy                      = destroy;
	self->calculateSize                = calculateSize;
	self->calculateDuration            = calculateDuration;
	self->setSampleEntry               = setSampleEntry;
	self->serialize                    = serialize;
	self->getCurrentDataReferenceIndex = getCurrentDataReferenceIndex;
	self->addSamples                   = addSamples;
	self->setupNew                     = setupNew;
	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}
