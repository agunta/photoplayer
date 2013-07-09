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
	$Id: MP4Media.c,v 1.44 2001/06/09 16:30:50 rtm Exp $
*/
#include "MP4Movies.h"
#include "MP4Atoms.h"
#include "MP4Descriptors.h"
#include "MP4DataHandler.h"
#include <stdlib.h>
#include <string.h>

MP4_EXTERN ( MP4Err ) MP4BeginMediaEdits( MP4Media theMedia )
{
   MP4Err err;
   err = MP4NoErr;
   if ( theMedia == NULL )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   /* finish this !!! */
	
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4EndMediaEdits( MP4Media theMedia )
{
   MP4MediaAtomPtr mdia;
   MP4Err err;
   err = MP4NoErr;
   if ( theMedia == NULL )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   mdia = (MP4MediaAtomPtr) theMedia;
   err = mdia->calculateDuration( mdia ); if (err) goto bail;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4AddMediaSampleReference( MP4Media media, u64 dataOffset, u32 sampleCount,
                                                  MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
                                                  MP4Handle decodingOffsetsH, MP4Handle syncSamplesH )
{
   MP4Err err;
   MP4MediaAtomPtr mdia;

   err = MP4NoErr;
   if ( media == NULL )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   mdia = (MP4MediaAtomPtr) media;
   err = mdia->addSampleReference( mdia, dataOffset, sampleCount, durationsH, sizesH, sampleEntryH,
                                   decodingOffsetsH, syncSamplesH );
	
  bail:
   TEST_RETURN( err );

   return err;		
}

MP4_EXTERN ( MP4Err ) MP4AddMediaSamples( MP4Media media, MP4Handle sampleH, u32 sampleCount,
                                          MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
                                          MP4Handle decodingOffsetsH, MP4Handle syncSamplesH )
{
   MP4Err err;
   MP4MediaAtomPtr mdia;

   err = MP4NoErr;
   if ( media == NULL )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   mdia = (MP4MediaAtomPtr) media;
   err = mdia->addSamples( mdia, sampleH, sampleCount, durationsH, sizesH, sampleEntryH,
                           decodingOffsetsH, syncSamplesH );
	
  bail:
   TEST_RETURN( err );

   return err;	
}

MP4_EXTERN ( MP4Err ) MP4SetMediaLanguage( MP4Media theMedia, char *threeCharCode )
{
   MP4Err err;
   u32 packedLanguage;
   MP4MediaHeaderAtomPtr mediaHeader;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (threeCharCode == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   mediaHeader = (MP4MediaHeaderAtomPtr) ((MP4MediaAtomPtr) theMedia)->mediaHeader;
   if ( mediaHeader == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   packedLanguage = 0;
   packedLanguage |= (*threeCharCode++ - 0x60) << 10;
   packedLanguage |= (*threeCharCode++ - 0x60) << 5;
   packedLanguage |= (*threeCharCode - 0x60);
   mediaHeader->packedLanguage = packedLanguage;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaLanguage( MP4Media theMedia, char *outThreeCharCode )
{
   MP4Err err;
   u32 packedLanguage;
   MP4MediaHeaderAtomPtr mediaHeader;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (outThreeCharCode == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   mediaHeader = (MP4MediaHeaderAtomPtr) ((MP4MediaAtomPtr) theMedia)->mediaHeader;
   if ( mediaHeader == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   packedLanguage = mediaHeader->packedLanguage;
   *outThreeCharCode++ = ((packedLanguage >> 10) & 0x1F) + 0x60;
   *outThreeCharCode++ = ((packedLanguage >> 5) & 0x1F) + 0x60;
   *outThreeCharCode   = (packedLanguage & 0x1F) + 0x60;
  bail:
   TEST_RETURN( err );

   return err;
}


ISO_EXTERN ( ISOErr ) MJ2SetMediaGraphicsMode( ISOMedia theMedia, u32 mode, const ISORGBColor *opColor )
{
	MP4Err err;
	MP4MediaInformationAtomPtr minf;
	MP4VideoMediaHeaderAtomPtr vmhd;
	u32 handlerType;
	
	err = MP4NoErr;
	if ( (theMedia == NULL) || (opColor == NULL) )
	{
		BAILWITHERROR( MP4BadParamErr );
	}
   
	err = MP4GetMediaHandlerDescription( theMedia, &handlerType, NULL ); if (err) goto bail;
	if ( handlerType != MP4VisualHandlerType )
		BAILWITHERROR( MP4NotImplementedErr )

	minf = (MP4MediaInformationAtomPtr) ((MP4MediaAtomPtr) theMedia)->information;
	if ( minf == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );

	vmhd = (MP4VideoMediaHeaderAtomPtr) minf->mediaHeader;
	if ( vmhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );
	
	vmhd->graphicsMode =  mode;
	vmhd->opColorRed = (u32) opColor->red;
	vmhd->opColorGreen = (u32) opColor->green;
	vmhd->opColorBlue = (u32) opColor->blue;
bail:
   TEST_RETURN( err );

   return err;
}

ISO_EXTERN ( ISOErr ) MJ2GetMediaGraphicsMode( ISOMedia theMedia, u32 *outMode, ISORGBColor *outOpColor )
{
	MP4Err err;
	MP4MediaInformationAtomPtr minf;
	MP4VideoMediaHeaderAtomPtr vmhd;
	u32 handlerType;
	
	err = MP4NoErr;
	if ( (theMedia == NULL) || (outOpColor == NULL) )
	{
		BAILWITHERROR( MP4BadParamErr );
	}
   
	err = MP4GetMediaHandlerDescription( theMedia, &handlerType, NULL ); if (err) goto bail;
	if ( handlerType != MP4VisualHandlerType )
		BAILWITHERROR( MP4NotImplementedErr )

	minf = (MP4MediaInformationAtomPtr) ((MP4MediaAtomPtr) theMedia)->information;
	if ( minf == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );

	vmhd = (MP4VideoMediaHeaderAtomPtr) minf->mediaHeader;
	if ( vmhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );
		
	*outMode = vmhd->graphicsMode;
	outOpColor->red = (u16) vmhd->opColorRed;
	outOpColor->green = (u16) vmhd->opColorGreen;
	outOpColor->blue = (u16) vmhd->opColorBlue;
bail:
	TEST_RETURN( err );

	return err;
}

ISO_EXTERN ( ISOErr ) MJ2SetMediaSoundBalance( ISOMedia theMedia, s16 balance )
{
	MP4Err err;
	MP4MediaInformationAtomPtr minf;
	MP4SoundMediaHeaderAtomPtr smhd;
	u32 handlerType;
	
	err = MP4NoErr;
	if ( theMedia == NULL )
	{
		BAILWITHERROR( MP4BadParamErr );
	}
   
	err = MP4GetMediaHandlerDescription( theMedia, &handlerType, NULL ); if (err) goto bail;
	if ( handlerType != MP4AudioHandlerType )
		BAILWITHERROR( MP4NotImplementedErr )

	minf = (MP4MediaInformationAtomPtr) ((MP4MediaAtomPtr) theMedia)->information;
	if ( minf == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );

	smhd = (MP4SoundMediaHeaderAtomPtr) minf->mediaHeader;
	if ( smhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );
	
	smhd->balance = (u32) balance;
bail:
   TEST_RETURN( err );

   return err;
}

ISO_EXTERN ( ISOErr ) MJ2GetMediaSoundBalance( ISOMedia theMedia, s16 *outBalance )
{
	MP4Err err;
	MP4MediaInformationAtomPtr minf;
	MP4SoundMediaHeaderAtomPtr smhd;
	u32 handlerType;
	
	err = MP4NoErr;
	if ( (theMedia == NULL) || (outBalance == NULL) )
	{
		BAILWITHERROR( MP4BadParamErr );
	}
   
	err = MP4GetMediaHandlerDescription( theMedia, &handlerType, NULL ); if (err) goto bail;
	if ( handlerType != MP4AudioHandlerType )
		BAILWITHERROR( MP4NotImplementedErr )

	minf = (MP4MediaInformationAtomPtr) ((MP4MediaAtomPtr) theMedia)->information;
	if ( minf == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );

	smhd = (MP4SoundMediaHeaderAtomPtr) minf->mediaHeader;
	if ( smhd == NULL )
		BAILWITHERROR( MP4InvalidMediaErr );
		
	*outBalance = (s16) smhd->balance;
bail:
	TEST_RETURN( err );

	return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaTimeScale( MP4Media theMedia, u32 *outTimeScale )
{
   MP4Err err;
   MP4MediaHeaderAtomPtr mediaHeader;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (outTimeScale == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   mediaHeader = (MP4MediaHeaderAtomPtr) ((MP4MediaAtomPtr) theMedia)->mediaHeader;
   if ( mediaHeader == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   *outTimeScale = mediaHeader->timeScale;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaDuration( MP4Media theMedia, u64 *outDuration )
{
   MP4Err err;
   MP4MediaHeaderAtomPtr mediaHeader;
   MP4MediaAtomPtr mdia;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (outDuration == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   mdia = (MP4MediaAtomPtr) theMedia;
   mediaHeader = (MP4MediaHeaderAtomPtr) (mdia)->mediaHeader;
   if ( mediaHeader == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   err = mdia->calculateDuration( mdia ); if (err) goto bail;
   *outDuration = mediaHeader->duration;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaTrack( MP4Media theMedia, MP4Track *outTrack )
{
   MP4Err err;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (outTrack == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   *outTrack = (MP4Track) ((MP4MediaAtomPtr) theMedia)->mediaTrack;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaSampleCount( MP4Media theMedia, u32 *outCount )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr      stbl;
   MP4SampleSizeAtomPtr       stsz;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (outCount == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   } 
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsz = (MP4SampleSizeAtomPtr) stbl->SampleSize;
   if ( stsz == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   *outCount = stsz->sampleCount;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4NewSampleDescription(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
                                                u32 objectTypeIndication,
                                                u32 streamType,
                                                u32 decoderBufferSize,
                                                u32 maxBitrate,
                                                u32 avgBitrate,
                                                MP4Handle decoderSpecificInfoH )
{
    return MP4NewSampleDescriptionWithOCRAssociation( theTrack, sampleDescriptionH,
                                                dataReferenceIndex,
                                                objectTypeIndication,
                                                streamType,
                                                decoderBufferSize,
                                                maxBitrate,
                                                avgBitrate,
                                                decoderSpecificInfoH,
                                                0 );
}

MP4_EXTERN ( MP4Err ) MP4NewSampleDescriptionWithOCRAssociation(  MP4Track theTrack,
                                                MP4Handle sampleDescriptionH,
                                                u32 dataReferenceIndex,
                                                u32 objectTypeIndication,
                                                u32 streamType,
                                                u32 decoderBufferSize,
                                                u32 maxBitrate,
                                                u32 avgBitrate,
                                                MP4Handle decoderSpecificInfoH,
                                                u32 theOCRESID )
{
   MP4Err MP4CreateMPEGSampleEntryAtom( MP4MPEGSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateVisualSampleEntryAtom( MP4VisualSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateAudioSampleEntryAtom( MP4AudioSampleEntryAtomPtr *outAtom );
   MP4Err MP4CreateESDAtom( MP4ESDAtomPtr *outAtom );
   MP4Err MP4CreateES_Descriptor( u32 tag, u32 size, u32 bytesRead, MP4DescriptorPtr *outDesc );
   MP4Err MP4CreateDecoderConfigDescriptor( u32 tag, u32 size, u32 bytesRead, MP4DescriptorPtr *outDesc );
   MP4Err MP4CreateSLConfigDescriptor( u32 tag, u32 size, u32 bytesRead, MP4DescriptorPtr *outDesc );

   MP4Err err;
   GenericSampleEntryAtomPtr     entry;
   MP4ESDAtomPtr                 esdAtom;
   MP4ES_DescriptorPtr           esd;
   MP4DecoderConfigDescriptorPtr config;
   MP4SLConfigDescriptorPtr      slconfig;
   MP4TrackAtomPtr               trak;
   u32 outReferenceIndex;

   if ( (theTrack == NULL) || (sampleDescriptionH == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }

   trak = (MP4TrackAtomPtr) theTrack;
   if ( trak->newTrackFlags & MP4NewTrackIsVisual )
   {
      err = MP4CreateVisualSampleEntryAtom( (MP4VisualSampleEntryAtomPtr*) &entry ); if (err) goto bail;
   }
   else if ( trak->newTrackFlags & MP4NewTrackIsAudio )
   {
      MP4AudioSampleEntryAtomPtr audioSampleEntry;
      MP4Media media;
      u32 timeScale;

      err = MP4CreateAudioSampleEntryAtom(  (MP4AudioSampleEntryAtomPtr*) &entry ); if (err) goto bail;
      audioSampleEntry = (MP4AudioSampleEntryAtomPtr) entry;
      err = MP4GetTrackMedia( theTrack, &media ); if (err) goto bail;
      err = MP4GetMediaTimeScale( media, &timeScale ); if (err) goto bail;
      audioSampleEntry->timeScale = timeScale;
   }
   else
   {
      err = MP4CreateMPEGSampleEntryAtom( (MP4MPEGSampleEntryAtomPtr*) &entry ); if (err) goto bail;
   }
   entry->dataReferenceIndex = dataReferenceIndex;
   err = MP4CreateESDAtom( &esdAtom ); if (err) goto bail;
   err = MP4CreateES_Descriptor( MP4ES_DescriptorTag, 0, 0, (MP4DescriptorPtr*) &esd ); if (err) goto bail;
   err = MP4CreateDecoderConfigDescriptor( MP4DecoderConfigDescriptorTag, 0, 0,
                                           (MP4DescriptorPtr*) &config ); if (err) goto bail;
   config->objectTypeIndication = objectTypeIndication;
   config->streamType           = streamType;
   config->upstream             = 0;
   config->bufferSizeDB         = decoderBufferSize;
   config->maxBitrate           = maxBitrate;
   config->avgBitrate           = avgBitrate;
   if ( decoderSpecificInfoH )
   {
      MP4Err MP4CreateMemoryInputStream( char *base, u32 size, MP4InputStreamPtr *outStream );
      MP4DescriptorPtr specificInfoDesc;
      MP4InputStreamPtr is;
      u32 infoSize;
                   
      err = MP4GetHandleSize( decoderSpecificInfoH, &infoSize ); if (err) goto bail;
      err = MP4CreateMemoryInputStream( *decoderSpecificInfoH, infoSize, &is ); if (err) goto bail;
      is->debugging = 0;
      err = MP4ParseDescriptor( is, &specificInfoDesc ); if (err) goto bail;
      config->decoderSpecificInfo = specificInfoDesc;
   }
   esd->decoderConfig = (MP4DescriptorPtr) config;
   err = MP4CreateSLConfigDescriptor( MP4SLConfigDescriptorTag, 0, 0,
                                      (MP4DescriptorPtr*) &slconfig ); if (err) goto bail;
   slconfig->predefined = SLConfigPredefinedMP4;
   slconfig->useTimestampsFlag = 1;

   /* JLF 11/00: OCR is always 0, if OCR wanted, create a sync dependancy */
   esd->OCRESID = 0;
   if (theOCRESID) {
	   MP4Err MP4AddTrackReferenceWithID( MP4Track theTrack, u32 dependsOnID, u32 dependencyType, u32 *outReferenceIndex);
	   err = MP4AddTrackReferenceWithID((MP4Track) trak, theOCRESID, MP4SyncTrackReferenceAtomType, &outReferenceIndex);
	   if (err) goto bail;
   }
   esd->slConfig = (MP4DescriptorPtr) slconfig;
   esdAtom->descriptor =  (MP4DescriptorPtr) esd;
   entry->ESDAtomPtr = (MP4AtomPtr) esdAtom;
   err = entry->calculateSize( (MP4AtomPtr) entry ); if (err) goto bail;
   err = MP4SetHandleSize( sampleDescriptionH, entry->size ); if (err) goto bail;
   err = entry->serialize( (MP4AtomPtr) entry, *sampleDescriptionH ); if (err) goto bail;

bail:
   if ( esdAtom ) {
		esdAtom->destroy( (MP4AtomPtr) esdAtom );
		//JLF 07/01: need to set the esd atom to NULL as the SampleDescEntry destructors
		//attempts to delete it
		if (entry) entry->ESDAtomPtr = NULL;
   }
   		
	if ( entry )
		entry->destroy( (MP4AtomPtr) entry );
   
	TEST_RETURN( err );
   	
	return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaSampleDescription( MP4Media theMedia, u32 index,
                                                    MP4Handle outDescriptionH, u32 *outDataReferenceIndex )
{
   MP4Err err;
   MP4MediaInformationAtomPtr  minf;
   MP4SampleTableAtomPtr       stbl;
   MP4SampleDescriptionAtomPtr stsd;
   GenericSampleEntryAtomPtr   entry;

   err = MP4NoErr;
   if ( (theMedia == NULL) || (index == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsd = (MP4SampleDescriptionAtomPtr) stbl->SampleDescription;
   if ( stsd == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( index > stsd->getEntryCount(stsd) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   err = stsd->getEntry( stsd, index, &entry ); if (err) goto bail;
   if ( entry == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( outDescriptionH )
   {
      err = entry->calculateSize( (MP4AtomPtr) entry ); if (err) goto bail;
      err = MP4SetHandleSize( outDescriptionH, entry->size ); if (err) goto bail;
      err = entry->serialize( (MP4AtomPtr) entry, *outDescriptionH ); if (err) goto bail;
   }
   if ( outDataReferenceIndex )
   {
      *outDataReferenceIndex = entry->dataReferenceIndex;
   }
  bail:
   TEST_RETURN( err );

   return err;

}

MP4Err MP4GetMediaESD( MP4Media theMedia, u32 index, MP4ES_DescriptorPtr *outESD, u32 *outDataReferenceIndex )
{
   MP4Err err;
   MP4MediaInformationAtomPtr  minf;
   MP4SampleTableAtomPtr       stbl;
   MP4SampleDescriptionAtomPtr stsd;
   GenericSampleEntryAtomPtr   entry;
   MP4ESDAtomPtr               ESD;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (index == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsd = (MP4SampleDescriptionAtomPtr) stbl->SampleDescription;
   if ( stsd == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( index > stsd->getEntryCount(stsd) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   err = stsd->getEntry( stsd, index, &entry ); if (err) goto bail;
   if ( entry == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   ESD = (MP4ESDAtomPtr) entry->ESDAtomPtr;
   if ( ESD == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( outESD )
   {
      MP4ES_DescriptorPtr esdDescriptor;
      esdDescriptor = (MP4ES_DescriptorPtr) ESD->descriptor;
      *outESD = esdDescriptor;
   }
   if ( outDataReferenceIndex )
   {
      *outDataReferenceIndex = entry->dataReferenceIndex;
   }
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaDecoderConfig( MP4Media theMedia, u32 sampleDescIndex, MP4Handle decoderConfigH )
{
   MP4Err err;
   MP4MediaInformationAtomPtr  minf;
   MP4SampleTableAtomPtr       stbl;
   MP4SampleDescriptionAtomPtr stsd;
   GenericSampleEntryAtomPtr   entry;
   MP4ES_DescriptorPtr esdDescriptor;
   MP4DescriptorPtr decoderConfig;
   MP4ESDAtomPtr               ESD;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (sampleDescIndex == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsd = (MP4SampleDescriptionAtomPtr) stbl->SampleDescription;
   if ( stsd == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( sampleDescIndex > stsd->getEntryCount(stsd) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   err = stsd->getEntry( stsd, sampleDescIndex, &entry ); if (err) goto bail;
   if ( entry == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   ESD = (MP4ESDAtomPtr) entry->ESDAtomPtr;
   if ( ESD == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }   
   esdDescriptor = (MP4ES_DescriptorPtr) ESD->descriptor;
   if ( esdDescriptor == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   decoderConfig = esdDescriptor->decoderConfig;
   if ( decoderConfig )
   {
      err = decoderConfig->calculateSize( decoderConfig ); if (err) goto bail;
      err = MP4SetHandleSize( decoderConfigH, decoderConfig->size ); if (err) goto bail;
      if ( decoderConfig->size )
      {
         err = decoderConfig->serialize( decoderConfig, *decoderConfigH ); if (err) goto bail;
      }
   }
   else
   {
      MP4SetHandleSize( decoderConfigH, 0 );
   }

  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaDecoderType( MP4Media theMedia, u32 sampleDescIndex, 
                                              u32 *outObjectType, u32 *outStreamType,
                                              u32 *outBufferSize,
                                              MP4Handle specificInfoH )
{
    return MP4GetMediaDecoderInformation( theMedia, sampleDescIndex,
                                           outObjectType, outStreamType,
                                           outBufferSize,
                                           NULL, /* upstream */
                                           NULL, /* max */
                                           NULL, /* avg */
                                           specificInfoH );
}

MP4_EXTERN ( MP4Err )
MP4GetMediaDecoderInformation( MP4Media theMedia, 
					   u32 sampleDescIndex, 
					   u32 *outObjectType, 
					   u32 *outStreamType,
					   u32 *outBufferSize,
					   u32 *outUpstream,
					   u32 *outMaxBitrate,
					   u32 *outAvgBitrate,
					   MP4Handle specificInfoH )
{
   MP4Err err;
   MP4MediaInformationAtomPtr  minf;
   MP4SampleTableAtomPtr       stbl;
   MP4SampleDescriptionAtomPtr stsd;
   GenericSampleEntryAtomPtr   entry;
   MP4ES_DescriptorPtr esdDescriptor;
   MP4DecoderConfigDescriptorPtr decoderConfig;
   MP4ESDAtomPtr               ESD;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (sampleDescIndex == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsd = (MP4SampleDescriptionAtomPtr) stbl->SampleDescription;
   if ( stsd == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( sampleDescIndex > stsd->getEntryCount(stsd) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   err = stsd->getEntry( stsd, sampleDescIndex, &entry ); if (err) goto bail;
   if ( entry == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   ESD = (MP4ESDAtomPtr) entry->ESDAtomPtr;
   if ( ESD == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   esdDescriptor = (MP4ES_DescriptorPtr) ESD->descriptor;
   if ( esdDescriptor == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   decoderConfig = (MP4DecoderConfigDescriptorPtr) esdDescriptor->decoderConfig;
   if ( decoderConfig )
   {
      MP4DescriptorPtr specificInfo;
      *outObjectType = decoderConfig->objectTypeIndication;
      *outStreamType = decoderConfig->streamType;
      *outBufferSize = decoderConfig->bufferSizeDB;
      if ( outUpstream )
      {
        *outUpstream = decoderConfig->upstream;
      }
      if ( outMaxBitrate )
      {
        *outMaxBitrate = decoderConfig->maxBitrate;
      }
      if ( outAvgBitrate )
      {
        *outAvgBitrate = decoderConfig->avgBitrate;
      }
      if ( specificInfoH )
      {
         specificInfo = decoderConfig->decoderSpecificInfo;
         if ( specificInfo )
         {
            err = specificInfo->calculateSize( specificInfo ); if (err) goto bail;
            err = MP4SetHandleSize( specificInfoH, specificInfo->size ); if (err) goto bail;
            if ( specificInfo->size )
            {
               u32 bytesSkipped;
               char *cp = *specificInfoH;
               err = specificInfo->serialize( specificInfo, *specificInfoH ); if (err) goto bail;
               cp++; /* skip tag; */
               for( bytesSkipped = 1; *cp & 0x80; )
               {
                  cp++;
                  bytesSkipped++;
                  if ( bytesSkipped == specificInfo->size )
                  {
                     BAILWITHERROR( MP4InvalidMediaErr );
                  }
               }
               bytesSkipped++;
               /* rtm: changed memcpy to memmove, since memory may overlap */
               memmove( *specificInfoH, *specificInfoH + bytesSkipped, specificInfo->size - bytesSkipped );
               err = MP4SetHandleSize( specificInfoH, specificInfo->size - bytesSkipped ); if (err) goto bail;
            }
         }
      }
      else
      {
         MP4SetHandleSize( specificInfoH, 0 );
      }
   }
   else
   {
      if ( specificInfoH )
      {
         MP4SetHandleSize( specificInfoH, 0 );
      }
   }

  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaDataRefCount( MP4Media theMedia, u32 *outCount )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4DataInformationAtomPtr dinf;
   MP4DataReferenceAtomPtr dref;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (outCount == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dinf = (MP4DataInformationAtomPtr) minf->dataInformation;
   if ( dinf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dref = (MP4DataReferenceAtomPtr) dinf->dataReference;
   if ( dref == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   *outCount = dref->getEntryCount( dref );
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4CheckMediaDataReferences( MP4Media theMedia )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4DataInformationAtomPtr dinf;
   MP4DataReferenceAtomPtr dref;
   u32 count;
   u32 dataEntryIndex;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dinf = (MP4DataInformationAtomPtr) minf->dataInformation;
   if ( dinf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dref = (MP4DataReferenceAtomPtr) dinf->dataReference;
   if ( dref == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
  count = dref->getEntryCount( dref );
  for ( dataEntryIndex = 1; dataEntryIndex <= count; dataEntryIndex++ )
  {
     err = minf->testDataEntry( minf, dataEntryIndex ); if (err) goto bail;
  }
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4AddMediaDataReference( MP4Media theMedia, u32 *outReferenceIndex, MP4Handle urlHandle, MP4Handle urnHandle )
{
   MP4Err MP4CreateDataEntryURLAtom( MP4DataEntryURLAtomPtr *outAtom );
   MP4Err MP4CreateDataEntryURNAtom( MP4DataEntryURNAtomPtr *outAtom );

   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4DataInformationAtomPtr dinf;
   MP4DataReferenceAtomPtr dref;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dinf = (MP4DataInformationAtomPtr) minf->dataInformation;
   if ( dinf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dref = (MP4DataReferenceAtomPtr) dinf->dataReference;
   if ( dref == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( urlHandle == NULL )
   {
      MP4Track track;
      MP4Movie movie;
      MP4PrivateMovieRecordPtr movieRecord;
      MP4DataEntryURLAtomPtr url;
      err = MP4GetMediaTrack( theMedia, &track ); if (err) goto bail;
      err = MP4GetTrackMovie( track, &movie ); if (err) goto bail;
      err = MP4CreateDataEntryURLAtom( &url ); if (err) goto bail;
      movieRecord = (MP4PrivateMovieRecordPtr) movie;
      
      url->flags |= 1;
      url->mdat = movieRecord->mdat;
      err = dref->addDataEntry( dref, (MP4AtomPtr) url ); if (err) goto bail;
   }
   else if ( urnHandle == NULL )
   {
      u32 sz;
      MP4DataEntryURLAtomPtr url;
      err = MP4CreateDataEntryURLAtom( &url ); if (err) goto bail;
      err = MP4GetHandleSize( urlHandle, &sz ); if (err) goto bail;
      url->locationLength = (u32) sz;
      url->location = calloc( 1, sz );
      memcpy( url->location, *urlHandle, sz );
      err = dref->addDataEntry( dref, (MP4AtomPtr) url ); if (err) goto bail;
   }
   else
   {
      u32 sz;
      MP4DataEntryURNAtomPtr urn;
      err = MP4CreateDataEntryURNAtom( &urn ); if (err) goto bail;
      err = MP4GetHandleSize( urlHandle, &sz ); if (err) goto bail;
      urn->locationLength = (u32) sz;
      urn->location = calloc( 1, sz );
      memcpy( urn->location, *urlHandle, sz );
      
      err = MP4GetHandleSize( urnHandle, &sz ); if (err) goto bail;
      urn->nameLength = (u32) sz;
      urn->name = calloc( 1, sz );
      memcpy( urn->name, *urnHandle, sz );
      err = dref->addDataEntry( dref, (MP4AtomPtr) urn ); if (err) goto bail;
   }
   *outReferenceIndex = dref->getEntryCount( dref );
  bail:
   TEST_RETURN( err );
   return err;
}


MP4_EXTERN ( MP4Err ) MP4GetMediaDataReference( MP4Media   theMedia,
                                                u32        index,
                                                MP4Handle  referenceH,
                                                MP4Handle  urnH,
                                                u32       *outReferenceType,
                                                u32       *outReferenceAttributes )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4DataInformationAtomPtr dinf;
   MP4DataReferenceAtomPtr dref;
   MP4DataEntryAtomPtr referenceAtom;
   MP4DataEntryURLAtomPtr url;
   MP4DataEntryURNAtomPtr urn;
   u32 referenceType;
   u32 referenceFlags;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (index == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dinf = (MP4DataInformationAtomPtr) minf->dataInformation;
   if ( dinf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dref = (MP4DataReferenceAtomPtr) dinf->dataReference;
   if ( dref == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( index > dref->getEntryCount(dref) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   err = dref->getEntry( dref, index, &referenceAtom ); if (err) goto bail;
   if ( referenceAtom == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   referenceType  = referenceAtom->type;
   referenceFlags = referenceAtom->flags;
   if ( outReferenceType )
   {
      *outReferenceType = referenceType;
   }
   if ( outReferenceAttributes )
   {
      *outReferenceAttributes = referenceFlags;
   }
   if ( referenceH || urnH )
   {
      switch ( referenceType )
      {
         case MP4DataEntryURLAtomType:
             if ( referenceH )
             {
                url = (MP4DataEntryURLAtomPtr) referenceAtom;
                err = MP4SetHandleSize( referenceH, url->locationLength ); if (err) goto bail;
                if ( url->locationLength )
                {
                   memcpy( *referenceH, url->location, url->locationLength );
                }
             }
             break;

         case MP4DataEntryURNAtomType:
             if ( referenceH )
             {
                url = (MP4DataEntryURLAtomPtr) referenceAtom;
                err = MP4SetHandleSize( referenceH, url->locationLength ); if (err) goto bail;
                if ( url->locationLength )
                {
                   memcpy( *referenceH, url->location, url->locationLength );
                }
             }
             if ( urnH )
             {
                urn = (MP4DataEntryURNAtomPtr) referenceAtom;
                err = MP4SetHandleSize( referenceH, urn->nameLength ); if (err) goto bail;
                if ( urn->nameLength )
                {
                   memcpy( *referenceH, urn->location, urn->nameLength );
                }
             }
             break;
				
         default:
             BAILWITHERROR( MP4InvalidMediaErr );
             break;
      }
   }
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaHandlerDescription( MP4Media theMedia, u32 *outType, MP4Handle *outName )
{
   MP4Err err;
   MP4HandlerAtomPtr handler;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || ((outType == NULL) && (outName == 0)) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   handler = (MP4HandlerAtomPtr) ((MP4MediaAtomPtr) theMedia)->handler;
   if ( handler == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( outType )
   {
      *outType = handler->handlerType;
   }
   if ( outName )
   {
      MP4Handle h;
      err = MP4NewHandle( handler->nameLength, &h ); if (err) goto bail;
      memcpy( *h, handler->nameUTF8, handler->nameLength );
      *outName = h;
   }
  bail:
   TEST_RETURN( err );

   return err;	
}

MP4_EXTERN ( MP4Err ) MP4GetMediaNextInterestingTime( MP4Media theMedia,
                                                      u32 interestingTimeFlags,   /* eg: MP4NextTimeMediaSample */
                                                      u64 searchFromTime,		 /* in Media time scale */
                                                      u32 searchDirection,        /* eg: MP4NextTimeSearchForward */
                                                      u64 *outInterestingTime,    /* in Media time scale */
                                                      u64 *outInterestingDuration /* in Media's time coordinate system */
   )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr      stbl;
   MP4TimeToSampleAtomPtr     stts;
   MP4SyncSampleAtomPtr       stss;
   s64 priorSample;
   s64 exactSample;
   s64 nextSample;
   u32 sampleNumber;
   s32 sampleDuration;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) || (interestingTimeFlags == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stts = (MP4TimeToSampleAtomPtr) stbl->TimeToSample;
   if ( stts == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stss = (MP4SyncSampleAtomPtr) stbl->SyncSample;
   err = stts->findSamples( stbl->TimeToSample, searchFromTime, 
                            &priorSample, &exactSample, &nextSample, 
                            &sampleNumber, &sampleDuration ); if (err) goto bail;
   if ( outInterestingTime )
   {
      if ( searchDirection == MP4NextTimeSearchForward )
      {
         if ( (exactSample >= 0) && (interestingTimeFlags & MP4NextTimeEdgeOK) )
         {
            *outInterestingTime = exactSample;
         }
         else
         {
            *outInterestingTime = nextSample;
         }
			
      }
      else
      {
         if ( (exactSample >= 0) && (interestingTimeFlags & MP4NextTimeEdgeOK) )
         {
            *outInterestingTime = exactSample;
         }
         else
         {
            *outInterestingTime = priorSample;
         }
      }
   }
   if ( outInterestingDuration )
   {
      *outInterestingDuration = sampleDuration;
   }
	
  bail:
   TEST_RETURN( err );

   return err;
}

MP4Err MP4GetMediaSampleDescIndex( MP4Media theMedia, u64 desiredTime, u32 *outSampleDescriptionIndex )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr      stbl;
   MP4TimeToSampleAtomPtr     stts;
   MP4SyncSampleAtomPtr       stss;
   MP4SampleSizeAtomPtr       stsz;
   MP4SampleToChunkAtomPtr    stsc;

   s64 priorSample;
   s64 exactSample;
   s64 nextSample;
   u32 sampleNumber;
   s32 sampleDuration;
   u32 chunkNumber;
   u32 sampleDescriptionIndex;
   u32 firstSampleNumberInChunk;

   err = MP4NoErr;
   if ( (theMedia == NULL) || (outSampleDescriptionIndex == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stts = (MP4TimeToSampleAtomPtr) stbl->TimeToSample;
   if ( stts == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stss = (MP4SyncSampleAtomPtr) stbl->SyncSample;
   stsz = (MP4SampleSizeAtomPtr) stbl->SampleSize;
   if ( stsz == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsc = (MP4SampleToChunkAtomPtr) stbl->SampleToChunk;
   if ( stsc == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   /* get sample number and time */
   err = stts->findSamples( stbl->TimeToSample, desiredTime, 
                            &priorSample, &exactSample, &nextSample, 
                            &sampleNumber, &sampleDuration ); if (err) goto bail;

   /* get sample description */
   err = stsc->lookupSample( stbl->SampleToChunk, sampleNumber,
                             &chunkNumber, &sampleDescriptionIndex, &firstSampleNumberInChunk ); if (err) goto bail;
							  
   *outSampleDescriptionIndex = sampleDescriptionIndex;
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetMediaSample( MP4Media theMedia, MP4Handle outSample, u32 *outSize,
                                         u64 desiredDecodingTime, u64 *outDecodingTime, u64 *outCompositionTime, u64 *outDuration,
                                         MP4Handle outSampleDescription, u32 *outSampleDescriptionIndex,
                                         u32 *outSampleFlags )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr      stbl;
   MP4TimeToSampleAtomPtr     stts;
   MP4CompositionOffsetAtomPtr   dtts;
   MP4SyncSampleAtomPtr       stss;
   MP4SampleSizeAtomPtr       stsz;
   MP4SampleToChunkAtomPtr    stsc;
   MP4ChunkOffsetAtomPtr      stco;
   MP4DataHandlerPtr          dhlr;

   s64 priorSample;
   s64 exactSample;
   s64 nextSample;
   u32 sampleNumber;
   s32 sampleDuration;
   u32 sampleSize;
   u32 chunkNumber;
   u32 sampleDescriptionIndex;
   u64 chunkOffset;
   u32 dataReferenceIndex;
   u32 firstSampleNumberInChunk;
   u32 sampleOffsetWithinChunk;

   err = MP4NoErr;
   if ( (theMedia == NULL) || (outSample == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stts = (MP4TimeToSampleAtomPtr) stbl->TimeToSample;
   if ( stts == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dtts = (MP4CompositionOffsetAtomPtr) stbl->CompositionOffset;
   stss = (MP4SyncSampleAtomPtr) stbl->SyncSample;
   stsz = (MP4SampleSizeAtomPtr) stbl->SampleSize;
   if ( stsz == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsc = (MP4SampleToChunkAtomPtr) stbl->SampleToChunk;
   if ( stsc == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stco = (MP4ChunkOffsetAtomPtr) stbl->ChunkOffset;
   if ( stco == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   /* get sample number and time */
   if ( outSampleFlags )
   {
      *outSampleFlags = 0;
   }
   err = stts->findSamples( stbl->TimeToSample, desiredDecodingTime, 
                            &priorSample, &exactSample, &nextSample, 
                            &sampleNumber, &sampleDuration ); if (err) goto bail;

   if ( dtts )
   {
      if ( outSampleFlags )
      {
         *outSampleFlags |= MP4MediaSampleHasCTSOffset;
      }

      if ( outCompositionTime )
      {
         s32 compositionTimeOffset;
         err = dtts->getOffsetForSampleNumber( (MP4AtomPtr) dtts, sampleNumber, &compositionTimeOffset ); if (err) goto bail;
         *outCompositionTime = exactSample + compositionTimeOffset;
      }
   }
	
   /* get sample description */
   err = stsc->lookupSample( stbl->SampleToChunk, sampleNumber,
                             &chunkNumber, &sampleDescriptionIndex, &firstSampleNumberInChunk ); if (err) goto bail;
	

   if ( outSampleDescriptionIndex )
   {
      *outSampleDescriptionIndex = sampleDescriptionIndex;
   }

   /* get sample size */
   err = stsz->getSampleSizeAndOffset( stbl->SampleSize, sampleNumber, &sampleSize, firstSampleNumberInChunk, &sampleOffsetWithinChunk ); if (err) goto bail;
   *outSize = sampleSize;


   /* get the chunk offset */
   err = stco->getChunkOffset( stbl->ChunkOffset, chunkNumber, &chunkOffset ); if (err) goto bail;

   /* get the sample */
   err = MP4GetMediaSampleDescription( theMedia, sampleDescriptionIndex, outSampleDescription, &dataReferenceIndex ); if (err) goto bail;
   err = minf->openDataHandler( (MP4AtomPtr) minf, dataReferenceIndex ); if (err) goto bail;
   dhlr = (MP4DataHandlerPtr) minf->dataHandler;
   if ( dhlr == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   /* finally we can get the sample */
   err = MP4SetHandleSize( outSample, sampleSize ); if (err) goto bail;
   err = dhlr->copyData( dhlr, (chunkOffset + sampleOffsetWithinChunk), *outSample, sampleSize ); if (err) goto bail;

   /* sync sample test */
   if ( outSampleFlags )
   {
      if ( stss )
      {
         u32 sync;
         err = stss->isSyncSample( stbl->SyncSample, sampleNumber, &sync ); if (err) goto bail;
         *outSampleFlags |= sync ? 0 : MP4MediaSampleNotSync;
      }
   }
   if ( outDecodingTime )
   {
      *outDecodingTime = exactSample ? exactSample : nextSample;
   }
   if ( outDuration )
   {
      *outDuration = sampleDuration;
   }
	
  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetIndMediaSample( MP4Media theMedia,
                                            u32 sampleNumber,
                                            MP4Handle outSample,
                                            u32 *outSize,
                                            u64 *outDTS,
                                            s32 *outCTSOffset,
                                            u64 *outDuration,
                                            u32 *outSampleFlags,
                                            u32 *outSampleDescIndex
   )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr      stbl;
   MP4TimeToSampleAtomPtr     stts;
   MP4CompositionOffsetAtomPtr   dtts;
   MP4SyncSampleAtomPtr       stss;
   MP4SampleSizeAtomPtr       stsz;
   MP4SampleToChunkAtomPtr    stsc;
   MP4ChunkOffsetAtomPtr      stco;
   MP4DataHandlerPtr          dhlr;


   s64 sampleDTS;

   s32 sampleDuration;
   u32 sampleSize;
   u32 chunkNumber;
   u32 sampleDescriptionIndex;
   u64 chunkOffset;
   u32 dataReferenceIndex;
   u32 firstSampleNumberInChunk;
   u32 sampleOffsetWithinChunk;

   err = MP4NoErr;
   if ( (theMedia == NULL) || (outSample == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stts = (MP4TimeToSampleAtomPtr) stbl->TimeToSample;
   if ( stts == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dtts = (MP4CompositionOffsetAtomPtr) stbl->CompositionOffset;
   stss = (MP4SyncSampleAtomPtr) stbl->SyncSample;
   stsz = (MP4SampleSizeAtomPtr) stbl->SampleSize;
   if ( stsz == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsc = (MP4SampleToChunkAtomPtr) stbl->SampleToChunk;
   if ( stsc == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stco = (MP4ChunkOffsetAtomPtr) stbl->ChunkOffset;
   if ( stco == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   if ( outSampleFlags )
   {
      *outSampleFlags = 0;
   }
   /* get sample DTS */
   err = stts->getTimeForSampleNumber( stbl->TimeToSample, sampleNumber, &sampleDTS, &sampleDuration ); if (err) goto bail;
   if ( dtts )
   {
      if ( outSampleFlags )
      {
         *outSampleFlags |= MP4MediaSampleHasCTSOffset;
      }
      if ( outCTSOffset )
      {
         s32 decodingOffset;
         err = dtts->getOffsetForSampleNumber( (MP4AtomPtr) dtts, sampleNumber, &decodingOffset ); if (err) goto bail;
         *outCTSOffset = decodingOffset;
      }
   }
	
   /* get sample description */
   err = stsc->lookupSample( stbl->SampleToChunk, sampleNumber,
                             &chunkNumber, &sampleDescriptionIndex, &firstSampleNumberInChunk ); if (err) goto bail;
	
   /* get sample size */
   err = stsz->getSampleSizeAndOffset( stbl->SampleSize, sampleNumber, &sampleSize, firstSampleNumberInChunk, &sampleOffsetWithinChunk ); if (err) goto bail;
   *outSize = sampleSize;

   /* get the chunk offset */
   err = stco->getChunkOffset( stbl->ChunkOffset, chunkNumber, &chunkOffset ); if (err) goto bail;

   /* get the sample */
   err = MP4GetMediaSampleDescription( theMedia, sampleDescriptionIndex, NULL, &dataReferenceIndex ); if (err) goto bail;
   err = minf->openDataHandler( (MP4AtomPtr) minf, dataReferenceIndex ); if (err) goto bail;
   dhlr = (MP4DataHandlerPtr) minf->dataHandler;
   if ( dhlr == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   /* finally we can get the sample */
   err = MP4SetHandleSize( outSample, sampleSize ); if (err) goto bail;
   if ( sampleSize > 0 )
   {
      err = dhlr->copyData( dhlr, (chunkOffset + sampleOffsetWithinChunk), *outSample, sampleSize ); if (err) goto bail;
   }
   if ( outSampleDescIndex )
   {
      *outSampleDescIndex = sampleDescriptionIndex;
   }

   /* sync sample test */
   if ( outSampleFlags )
   {
      if ( stss )
      {
         u32 sync;
         err = stss->isSyncSample( stbl->SyncSample, sampleNumber, &sync ); if (err) goto bail;
         *outSampleFlags |= sync ? 0 : MP4MediaSampleNotSync;
      }
   }
	
   if ( outDTS )
   {
      *outDTS = sampleDTS;
   }
   if ( outDuration )
   {
      *outDuration = sampleDuration;
   }

  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4GetIndMediaSampleReference( MP4Media theMedia,
                                                     u32 sampleNumber,
                                                     u32 *outOffset,
                                                     u32 *outSize,
                                                     u32 *outDuration,
                                                     u32 *outSampleFlags,
                                                     u32 *outSampleDescIndex,
                                                     MP4Handle sampleDesc
   )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr      stbl;
   MP4TimeToSampleAtomPtr     stts;
   MP4CompositionOffsetAtomPtr   dtts;
   MP4SyncSampleAtomPtr       stss;
   MP4SampleSizeAtomPtr       stsz;
   MP4SampleToChunkAtomPtr    stsc;
   MP4ChunkOffsetAtomPtr      stco;

   s64 sampleCTS;

   s32 sampleDuration;
   u32 sampleSize;
   u32 chunkNumber;
   u32 sampleDescriptionIndex;
   u64 chunkOffset;
   u32 dataReferenceIndex;
   u32 firstSampleNumberInChunk;
   u32 sampleOffsetWithinChunk;

   err = MP4NoErr;
   if ( (theMedia == NULL) || (sampleNumber == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stts = (MP4TimeToSampleAtomPtr) stbl->TimeToSample;
   if ( stts == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dtts = (MP4CompositionOffsetAtomPtr) stbl->CompositionOffset;
   stss = (MP4SyncSampleAtomPtr) stbl->SyncSample;
   stsz = (MP4SampleSizeAtomPtr) stbl->SampleSize;
   if ( stsz == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stsc = (MP4SampleToChunkAtomPtr) stbl->SampleToChunk;
   if ( stsc == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stco = (MP4ChunkOffsetAtomPtr) stbl->ChunkOffset;
   if ( stco == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }

   if ( outSampleFlags )
   {
      *outSampleFlags = 0;
   }
   /* get sample CTS */
   err = stts->getTimeForSampleNumber( stbl->TimeToSample, sampleNumber, &sampleCTS, &sampleDuration ); if (err) goto bail;
   if ( dtts )
   {
      if ( outSampleFlags )
      {
         *outSampleFlags |= MP4MediaSampleHasCTSOffset;
      }
   }
	
   /* get sample description */
   err = stsc->lookupSample( stbl->SampleToChunk, sampleNumber,
                             &chunkNumber, &sampleDescriptionIndex, &firstSampleNumberInChunk ); if (err) goto bail;
	
   /* get sample size */
   err = stsz->getSampleSizeAndOffset( stbl->SampleSize, sampleNumber, &sampleSize, firstSampleNumberInChunk, &sampleOffsetWithinChunk ); if (err) goto bail;
   *outSize = sampleSize;

   /* get the chunk offset */
   err = stco->getChunkOffset( stbl->ChunkOffset, chunkNumber, &chunkOffset ); if (err) goto bail;

   /* get the sample description */
   err = MP4GetMediaSampleDescription( theMedia, sampleDescriptionIndex, sampleDesc, &dataReferenceIndex ); if (err) goto bail;

   if ( outOffset )
   {
      *outOffset = (u32) chunkOffset + sampleOffsetWithinChunk;
   }
   if ( outSampleDescIndex )
   {
      *outSampleDescIndex = sampleDescriptionIndex;
   }
   /* sync sample test */
   if ( outSampleFlags )
   {
      if ( stss )
      {
         u32 sync;
         err = stss->isSyncSample( stbl->SyncSample, sampleNumber, &sync ); if (err) goto bail;
         *outSampleFlags |= sync ? 0 : MP4MediaSampleNotSync;
      }
   }
   if ( outDuration )
   {
      *outDuration = sampleDuration;
   }
	
  bail:
   TEST_RETURN( err );
   return err;
}

MP4_EXTERN ( MP4Err ) MP4MediaTimeToSampleNum( MP4Media theMedia, u64 mediaTime, u32 *outSampleNum, u64 *outSampleCTS, u64 *outSampleDTS, s32 *outSampleDuration )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr      stbl;
   MP4TimeToSampleAtomPtr     stts;
   MP4CompositionOffsetAtomPtr   dtts;

   s64 priorSample;
   s64 exactSample;
   s64 nextSample;
   u32 sampleNumber;
   s32 sampleDuration;


   err = MP4NoErr;
   if ( (theMedia == NULL) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stts = (MP4TimeToSampleAtomPtr) stbl->TimeToSample;
   if ( stts == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dtts = (MP4CompositionOffsetAtomPtr) stbl->CompositionOffset;	
   /* get sample number and time */
   err = stts->findSamples( stbl->TimeToSample, mediaTime, 
                            &priorSample, &exactSample, &nextSample, 
                            &sampleNumber, &sampleDuration ); if (err) goto bail;

   if ( dtts )
   {
      if ( outSampleDTS )
      {
         s32 decodingOffset;
         err = dtts->getOffsetForSampleNumber( (MP4AtomPtr) dtts, sampleNumber, &decodingOffset ); if (err) goto bail;
         *outSampleDTS = exactSample - decodingOffset;
      }
   }
   if ( outSampleNum )
   {
      *outSampleNum = sampleNumber;
   }
   if ( outSampleCTS )
   {
      *outSampleCTS = exactSample;
   }
   if ( outSampleDuration )
   {
      *outSampleDuration = sampleDuration;
   }

  bail:
   TEST_RETURN( err );

   return err;
}

MP4_EXTERN ( MP4Err ) MP4SampleNumToMediaTime( MP4Media theMedia, u32 sampleNum, u64 *outSampleCTS, u64 *outSampleDTS, s32 *outSampleDuration )
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4SampleTableAtomPtr      stbl;
   MP4TimeToSampleAtomPtr     stts;
   MP4CompositionOffsetAtomPtr   dtts;
   s32 sampleDuration;
   s64 sampleCTS;

   err = MP4NoErr;
   if ( (theMedia == NULL) || (sampleNum == 0) )
   {
      BAILWITHERROR( MP4BadParamErr );
   }
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stbl = (MP4SampleTableAtomPtr) minf->sampleTable;
   if ( stbl == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   stts = (MP4TimeToSampleAtomPtr) stbl->TimeToSample;
   if ( stts == NULL )
   {
      BAILWITHERROR( MP4InvalidMediaErr );
   }
   dtts = (MP4CompositionOffsetAtomPtr) stbl->CompositionOffset;
   /* get sample time and duration */
   err = stts->getTimeForSampleNumber( stbl->TimeToSample, sampleNum, &sampleCTS, &sampleDuration ); if (err) goto bail;

   if ( dtts )
   {
      if ( outSampleDTS )
      {
         s32 decodingOffset;
         err = dtts->getOffsetForSampleNumber( (MP4AtomPtr) dtts, sampleNum, &decodingOffset ); if (err) goto bail;
         *outSampleDTS = sampleCTS - decodingOffset;
      }
   }
   if ( outSampleCTS )
   {
      *outSampleCTS = sampleCTS;
   }
   if ( outSampleDuration )
   {
      *outSampleDuration = sampleDuration;
   }
  bail:
   TEST_RETURN( err );

   return err;
}

/* JLF: set the stream priority in the sample desc */
MP4_EXTERN ( MP4Err ) MP4SetSampleDescriptionPriority(MP4Handle sampleEntryH, u32 priority)
{
	MP4Err MP4CreateMemoryInputStream( char *base, u32 size, MP4InputStreamPtr *outStream );
	MP4Err MP4ParseAtomUsingProtoList( MP4InputStreamPtr inputStream, u32* protoList, u32 defaultAtom, MP4AtomPtr *outAtom  );

	MP4Err err = MP4NoErr;
	MP4InputStreamPtr is;
	MP4ESDAtomPtr esda;
	MP4ES_Descriptor *esd;
	u32 size;
	GenericSampleEntryAtomPtr entry = NULL;
	is = NULL;
	
	if ( sampleEntryH == NULL ) return MP4BadParamErr;

	err = MP4GetHandleSize( sampleEntryH, &size ); if (err) goto bail;
	err = MP4CreateMemoryInputStream(*sampleEntryH, size, &is ); if (err) goto bail;
	is->debugging = 0;
	err = MP4ParseAtom( is, (MP4AtomPtr *)&entry ); if (err) goto bail;

	esda = (MP4ESDAtomPtr) entry->ESDAtomPtr;
	if (esda == NULL) {
		err = MP4BadParamErr;
		goto bail;
	}
	esd = (MP4ES_DescriptorPtr) esda->descriptor;
	if (esd == NULL) {
		err = MP4InvalidMediaErr;
		goto bail;
	}
	esd->streamPriority = priority;

	/* rewrite it... */
	err = entry->calculateSize( (MP4AtomPtr) entry ); if (err) goto bail;
	err = MP4SetHandleSize( sampleEntryH, entry->size ); if (err) goto bail;
	err = entry->serialize( (MP4AtomPtr) entry, *sampleEntryH ); if (err) goto bail;

bail:
	if (entry)
		entry->destroy((MP4AtomPtr) entry);
	if (is)
		is->destroy(is);
	return err;
}


/* JLF: add a Descriptor to the esd in the sample desc */
MP4_EXTERN ( MP4Err ) MP4AddDescToSampleDescription(MP4Handle sampleEntryH, MP4Handle descriptorH)
{

	MP4Err MP4CreateMemoryInputStream( char *base, u32 size, MP4InputStreamPtr *outStream );
	MP4Err MP4ParseAtomUsingProtoList( MP4InputStreamPtr inputStream, u32* protoList, u32 defaultAtom, MP4AtomPtr *outAtom  );

	MP4Err err = MP4NoErr;
	MP4InputStreamPtr is;
	MP4ESDAtomPtr esda;
	MP4ES_Descriptor *esd;
	MP4DescriptorPtr desc;
	u32 size;
	GenericSampleEntryAtomPtr entry = NULL;
	is = NULL;
	
	if ( sampleEntryH == NULL ) return MP4BadParamErr;

	/* First get our ESdescriptor */
	err = MP4GetHandleSize( sampleEntryH, &size ); if (err) goto bail;
	err = MP4CreateMemoryInputStream(*sampleEntryH, size, &is ); if (err) goto bail;
	is->debugging = 0;
	err = MP4ParseAtom( is, (MP4AtomPtr *)&entry ); if (err) goto bail;
	is->destroy(is);

	esda = (MP4ESDAtomPtr) entry->ESDAtomPtr;
	if (esda == NULL) {
		err = MP4BadParamErr;
		goto bail;
	}
	esd = (MP4ES_DescriptorPtr) esda->descriptor;
	if (esd == NULL) {
		err = MP4InvalidMediaErr;
		goto bail;
	}
	/* then parse our descriptor... */
	err = MP4GetHandleSize( descriptorH, &size ); if (err) goto bail;
	err = MP4CreateMemoryInputStream(*descriptorH, size, &is ); if (err) goto bail;
	is->debugging = 0;
	err = MP4ParseDescriptor( is, (MP4DescriptorPtr *)&desc); if (err) goto bail;
	is->destroy(is);
	
	if (!desc) {
		err = MP4BadParamErr;
		goto bail;
	}
	err = esd->addDescriptor((MP4DescriptorPtr) esd, desc);
	if (err) goto bail;

	/* rewrite it... */
	err = entry->calculateSize( (MP4AtomPtr) entry ); if (err) goto bail;
	err = MP4SetHandleSize( sampleEntryH, entry->size ); if (err) goto bail;
	err = entry->serialize( (MP4AtomPtr) entry, *sampleEntryH ); if (err) goto bail;

bail:
	if (entry) entry->destroy((MP4AtomPtr) entry);
	return err;

}


/* jlf 12/00: check a specific data ref */
MP4_EXTERN ( MP4Err ) 
MP4CheckMediaDataRef( MP4Media theMedia, u32 dataEntryIndex)
{
   MP4Err err;
   MP4MediaInformationAtomPtr minf;
   MP4DataInformationAtomPtr dinf;
   MP4DataReferenceAtomPtr dref;
	
   err = MP4NoErr;
   if ( (theMedia == NULL) )  BAILWITHERROR( MP4BadParamErr );
   minf = (MP4MediaInformationAtomPtr)((MP4MediaAtomPtr) theMedia)->information;
   if ( minf == NULL )     BAILWITHERROR( MP4InvalidMediaErr );
   dinf = (MP4DataInformationAtomPtr) minf->dataInformation;
   if ( dinf == NULL )  BAILWITHERROR( MP4InvalidMediaErr );
   dref = (MP4DataReferenceAtomPtr) dinf->dataReference;
   if ( dref == NULL )  BAILWITHERROR( MP4InvalidMediaErr );

   return minf->testDataEntry( minf, dataEntryIndex);

bail:
   return err;
}
