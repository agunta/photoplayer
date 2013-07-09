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
	$Id: MP4Atoms.c,v 1.2 2000/05/17 08:01:28 francesc Exp $
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
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#ifndef INCLUDED_MJ2ATOMS_H
#include "MJ2Atoms.h"
#endif

static MP4AtomPtr MP4BaseAtomClassPtr = 0;

static char* baseAtomGetName( MP4AtomPtr self )
{
	return self->name;
}

static void  baseAtomDestroy( MP4AtomPtr self )
{
	free( self );
}

static MP4Err baseAtomCreateFromInputStream( MP4AtomPtr self, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	inputStream = inputStream;
	self->type = proto->type;
	memcpy( self->uuid, proto->uuid, 16 );
	self->size      = proto->size;
	self->size64    = proto->size64;
	self->bytesRead = proto->bytesRead;
	return MP4NoErr;
}

MP4Err MP4CreateBaseAtom( MP4AtomPtr self )
{
	MP4Err err;
	err = MP4NoErr;
	
	if ( self == NULL )
		BAILWITHERROR( MP4BadParamErr )
	if ( MP4BaseAtomClassPtr == NULL )
	{
		MP4BaseAtomClassPtr = (MP4AtomPtr) calloc( 1, sizeof(MP4Atom) );
		TESTMALLOC( MP4BaseAtomClassPtr );

		MP4BaseAtomClassPtr->name                = "base";
		MP4BaseAtomClassPtr->createFromInputStream = (cisfunc) baseAtomCreateFromInputStream;
		MP4BaseAtomClassPtr->getName             = baseAtomGetName;
		MP4BaseAtomClassPtr->destroy             = baseAtomDestroy;
		MP4BaseAtomClassPtr->super               = MP4BaseAtomClassPtr;
	}
	*self = *MP4BaseAtomClassPtr;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4FullAtomPtr MP4FullAtomClassPtr = 0;

static void  fullAtomDestroy( MP4AtomPtr self )
{
	baseAtomDestroy( self );
}

static MP4Err fullAtomCreateFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	MP4FullAtomPtr self;
	MP4Err err;
	u32 val;
	self = (MP4FullAtomPtr) s;

	err = baseAtomCreateFromInputStream( s, proto, inputStream ); if (err) goto bail;
	err = inputStream->read32( inputStream, &val, NULL ); if (err) goto bail;
	DEBUG_SPRINTF( "atom version = %d", (val >> 24) );
	DEBUG_SPRINTF( "atom flags = 0x%06x", (val & 0xFFFFFF) );
	self->bytesRead += 4;
	self->version = val >> 24;
	self->flags = val & 0xffffff;
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4CreateFullAtom( MP4AtomPtr s )
{
	MP4FullAtomPtr self;
	MP4Err err;
	self = (MP4FullAtomPtr) s;
	
	err = MP4NoErr;
	
	if ( MP4FullAtomClassPtr == NULL )
	{
		MP4FullAtomClassPtr = (MP4FullAtomPtr) calloc( 1, sizeof(MP4FullAtom) );
		TESTMALLOC( MP4FullAtomClassPtr );
		err = MP4CreateBaseAtom( (MP4AtomPtr) MP4FullAtomClassPtr ); if (err) goto bail;
		MP4FullAtomClassPtr->createFromInputStream = (cisfunc) fullAtomCreateFromInputStream;
		MP4FullAtomClassPtr->destroy = fullAtomDestroy;
		MP4FullAtomClassPtr->super     = (MP4AtomPtr) MP4FullAtomClassPtr;
		MP4FullAtomClassPtr->name      = "full";
		MP4FullAtomClassPtr->version   = 0;
		MP4FullAtomClassPtr->flags     = 0;
	}
	*self = *MP4FullAtomClassPtr;
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4CreateAudioSampleEntryAtom( MP4AudioSampleEntryAtomPtr *outAtom );
MP4Err MP4CreateChunkLargeOffsetAtom( MP4ChunkLargeOffsetAtomPtr *outAtom );
MP4Err MP4CreateChunkOffsetAtom( MP4ChunkOffsetAtomPtr *outAtom );
MP4Err MP4CreateClockReferenceMediaHeaderAtom( MP4ClockReferenceMediaHeaderAtomPtr *outAtom );
MP4Err MP4CreateCompositionOffsetAtom( MP4CompositionOffsetAtomPtr *outAtom );
MP4Err MP4CreateCopyrightAtom( MP4CopyrightAtomPtr *outAtom );
MP4Err MP4CreateDataEntryURLAtom( MP4DataEntryURLAtomPtr *outAtom );
MP4Err MP4CreateDataEntryURNAtom( MP4DataEntryURNAtomPtr *outAtom );
MP4Err MP4CreateDataInformationAtom( MP4DataInformationAtomPtr *outAtom );
MP4Err MP4CreateDataReferenceAtom( MP4DataReferenceAtomPtr *outAtom );
MP4Err MP4CreateDegradationPriorityAtom( MP4DegradationPriorityAtomPtr *outAtom );
MP4Err MP4CreateESDAtom( MP4ESDAtomPtr *outAtom );
MP4Err MP4CreateEditAtom( MP4EditAtomPtr *outAtom );
MP4Err MP4CreateEditListAtom( MP4EditListAtomPtr *outAtom );
MP4Err MP4CreateFreeSpaceAtom( MP4FreeSpaceAtomPtr *outAtom );
MP4Err MP4CreateGenericSampleEntryAtom( MP4GenericSampleEntryAtomPtr *outAtom );
MP4Err MP4CreateHandlerAtom( MP4HandlerAtomPtr *outAtom );
MP4Err MP4CreateHintMediaHeaderAtom( MP4HintMediaHeaderAtomPtr *outAtom );
MP4Err MP4CreateMPEGMediaHeaderAtom( MP4MPEGMediaHeaderAtomPtr *outAtom );
MP4Err MP4CreateMPEGSampleEntryAtom( MP4MPEGSampleEntryAtomPtr *outAtom );
MP4Err MP4CreateMediaAtom( MP4MediaAtomPtr *outAtom );
MP4Err MP4CreateMediaDataAtom( MP4MediaDataAtomPtr *outAtom );
MP4Err MP4CreateMediaHeaderAtom( MP4MediaHeaderAtomPtr *outAtom );
MP4Err MP4CreateMediaInformationAtom( MP4MediaInformationAtomPtr *outAtom );
MP4Err MP4CreateMovieAtom( MP4MovieAtomPtr *outAtom );
MP4Err MP4CreateMovieHeaderAtom( MP4MovieHeaderAtomPtr *outAtom );
MP4Err MP4CreateObjectDescriptorAtom( MP4ObjectDescriptorAtomPtr *outAtom );
MP4Err MP4CreateObjectDescriptorMediaHeaderAtom( MP4ObjectDescriptorMediaHeaderAtomPtr *outAtom );
MP4Err MP4CreateSampleDescriptionAtom( MP4SampleDescriptionAtomPtr *outAtom );
MP4Err MP4CreateSampleSizeAtom( MP4SampleSizeAtomPtr *outAtom );
MP4Err MP4CreateSampleTableAtom( MP4SampleTableAtomPtr *outAtom );
MP4Err MP4CreateSampleToChunkAtom( MP4SampleToChunkAtomPtr *outAtom );
MP4Err MP4CreateSceneDescriptionMediaHeaderAtom( MP4SceneDescriptionMediaHeaderAtomPtr *outAtom );
MP4Err MP4CreateShadowSyncAtom( MP4ShadowSyncAtomPtr *outAtom );
MP4Err MP4CreateSoundMediaHeaderAtom( MP4SoundMediaHeaderAtomPtr *outAtom );
MP4Err MP4CreateSyncSampleAtom( MP4SyncSampleAtomPtr *outAtom );
MP4Err MP4CreateTimeToSampleAtom( MP4TimeToSampleAtomPtr *outAtom );
MP4Err MP4CreateTrackAtom( MP4TrackAtomPtr *outAtom );
MP4Err MP4CreateTrackHeaderAtom( MP4TrackHeaderAtomPtr *outAtom );
MP4Err MP4CreateTrackReferenceAtom( MP4TrackReferenceAtomPtr *outAtom );
MP4Err MP4CreateTrackReferenceTypeAtom( u32 atomType, MP4TrackReferenceTypeAtomPtr *outAtom );
MP4Err MP4CreateUnknownAtom( MP4UnknownAtomPtr *outAtom );
MP4Err MP4CreateUserDataAtom( MP4UserDataAtomPtr *outAtom );
MP4Err MP4CreateVideoMediaHeaderAtom( MP4VideoMediaHeaderAtomPtr *outAtom );
MP4Err MP4CreateVisualSampleEntryAtom( MP4VisualSampleEntryAtomPtr *outAtom );

MP4Err MP4CreateFileTypeAtom( MP4FileTypeAtomPtr *outAtom );
MP4Err MP4CreateMetadataAtom( MP4MetadataAtomPtr *outAtom );
MP4Err MP4CreateBinaryXMLAtom( MP4BinaryXMLAtomPtr *outAtom );

MP4Err MP4CreateAtom( u32 atomType, MP4AtomPtr *outAtom )
{
	MP4Err err;
	MP4AtomPtr newAtom;
	
	err = MP4NoErr;
	
	switch ( atomType )
	{
		case MP4HintTrackReferenceAtomType:
		case MP4StreamDependenceAtomType:
		case MP4ODTrackReferenceAtomType:
		case MP4SyncTrackReferenceAtomType:
			err = MP4CreateTrackReferenceTypeAtom( atomType, (MP4TrackReferenceTypeAtomPtr*) &newAtom );
			break;
			
		case MP4FreeSpaceAtomType:
		case MP4SkipAtomType:
			err = MP4CreateFreeSpaceAtom( (MP4FreeSpaceAtomPtr*) &newAtom );
			break;
			
		case MP4MediaDataAtomType:
			err = MP4CreateMediaDataAtom( (MP4MediaDataAtomPtr*) &newAtom );
			break;
			
		case MP4MovieAtomType:
			err = MP4CreateMovieAtom( (MP4MovieAtomPtr*) &newAtom );
			break;
			
		case MP4MovieHeaderAtomType:
			err = MP4CreateMovieHeaderAtom( (MP4MovieHeaderAtomPtr*) &newAtom );
			break;
			
		case MP4MediaHeaderAtomType:
			err = MP4CreateMediaHeaderAtom( (MP4MediaHeaderAtomPtr*) &newAtom );
			break;
			
		case MP4VideoMediaHeaderAtomType:
			err = MP4CreateVideoMediaHeaderAtom( (MP4VideoMediaHeaderAtomPtr*) &newAtom );
			break;
			
		case MP4SoundMediaHeaderAtomType:
			err = MP4CreateSoundMediaHeaderAtom( (MP4SoundMediaHeaderAtomPtr*) &newAtom );
			break;
			
		case MP4MPEGMediaHeaderAtomType:
			err = MP4CreateMPEGMediaHeaderAtom( (MP4MPEGMediaHeaderAtomPtr*) &newAtom );
			break;
		
		case MP4ObjectDescriptorMediaHeaderAtomType:
			err = MP4CreateObjectDescriptorMediaHeaderAtom( (MP4ObjectDescriptorMediaHeaderAtomPtr*) &newAtom );
			break;
			
		case MP4ClockReferenceMediaHeaderAtomType:
			err = MP4CreateClockReferenceMediaHeaderAtom( (MP4ClockReferenceMediaHeaderAtomPtr*) &newAtom );
			break;
			
		case MP4SceneDescriptionMediaHeaderAtomType:
			err = MP4CreateSceneDescriptionMediaHeaderAtom( (MP4SceneDescriptionMediaHeaderAtomPtr*) &newAtom );
			break;

		case MP4HintMediaHeaderAtomType:
			err = MP4CreateHintMediaHeaderAtom( (MP4HintMediaHeaderAtomPtr*) &newAtom );
			break;
			
		case MP4SampleTableAtomType:
			err = MP4CreateSampleTableAtom( (MP4SampleTableAtomPtr*) &newAtom );
			break;

		case MP4DataInformationAtomType:
			err = MP4CreateDataInformationAtom( (MP4DataInformationAtomPtr*) &newAtom );
			break;

		case MP4DataEntryURLAtomType:
			err = MP4CreateDataEntryURLAtom( (MP4DataEntryURLAtomPtr*) &newAtom );
			break;
			
		case MP4CopyrightAtomType:
			err = MP4CreateCopyrightAtom( (MP4CopyrightAtomPtr*) &newAtom );
			break;
			
		case MP4DataEntryURNAtomType:
			err = MP4CreateDataEntryURNAtom( (MP4DataEntryURNAtomPtr*) &newAtom );
			break;
			
		case MP4HandlerAtomType:
			err = MP4CreateHandlerAtom( (MP4HandlerAtomPtr*) &newAtom );
			break;
			
		case MP4ObjectDescriptorAtomType:
			err = MP4CreateObjectDescriptorAtom( (MP4ObjectDescriptorAtomPtr*) &newAtom );
			break;
			
		case MP4TrackAtomType:
			err = MP4CreateTrackAtom( (MP4TrackAtomPtr*) &newAtom );
			break;

		case MP4MPEGSampleEntryAtomType:
			err = MP4CreateMPEGSampleEntryAtom( (MP4MPEGSampleEntryAtomPtr*) &newAtom );
			break;

		case MP4VisualSampleEntryAtomType:
			err = MP4CreateVisualSampleEntryAtom( (MP4VisualSampleEntryAtomPtr*) &newAtom );
			break;

		case MP4AudioSampleEntryAtomType:
			err = MP4CreateAudioSampleEntryAtom( (MP4AudioSampleEntryAtomPtr*) &newAtom );
			break;

		case MP4GenericSampleEntryAtomType:
			err = MP4CreateGenericSampleEntryAtom( (MP4GenericSampleEntryAtomPtr*) &newAtom );
			break;

		case MP4EditAtomType:
			err = MP4CreateEditAtom( (MP4EditAtomPtr*) &newAtom );
			break;

		case MP4UserDataAtomType:
			err = MP4CreateUserDataAtom( (MP4UserDataAtomPtr*) &newAtom );
			break;

		case MP4DataReferenceAtomType:
			err = MP4CreateDataReferenceAtom( (MP4DataReferenceAtomPtr*) &newAtom );
			break;

		case MP4SampleDescriptionAtomType:
			err = MP4CreateSampleDescriptionAtom( (MP4SampleDescriptionAtomPtr*) &newAtom );
			break;

		case MP4TimeToSampleAtomType:
			err = MP4CreateTimeToSampleAtom( (MP4TimeToSampleAtomPtr*) &newAtom );
			break;

		case MP4CompositionOffsetAtomType:
			err = MP4CreateCompositionOffsetAtom( (MP4CompositionOffsetAtomPtr*) &newAtom );
			break;

		case MP4ShadowSyncAtomType:
			err = MP4CreateShadowSyncAtom( (MP4ShadowSyncAtomPtr*) &newAtom );
			break;

		case MP4EditListAtomType:
			err = MP4CreateEditListAtom( (MP4EditListAtomPtr*) &newAtom );
			break;

		case MP4SampleToChunkAtomType:
			err = MP4CreateSampleToChunkAtom( (MP4SampleToChunkAtomPtr*) &newAtom );
			break;

		case MP4SampleSizeAtomType:
			err = MP4CreateSampleSizeAtom( (MP4SampleSizeAtomPtr*) &newAtom );
			break;

		case MP4ChunkOffsetAtomType:
			err = MP4CreateChunkOffsetAtom( (MP4ChunkOffsetAtomPtr*) &newAtom );
			break;

		case MP4SyncSampleAtomType:
			err = MP4CreateSyncSampleAtom( (MP4SyncSampleAtomPtr*) &newAtom );
			break;
			
		case MP4DegradationPriorityAtomType:
			err = MP4CreateDegradationPriorityAtom( (MP4DegradationPriorityAtomPtr*) &newAtom );
			break;

		case MP4ChunkLargeOffsetAtomType:
			err = MP4CreateChunkLargeOffsetAtom( (MP4ChunkLargeOffsetAtomPtr*) &newAtom );
			break;

		case MP4ESDAtomType:
			err = MP4CreateESDAtom( (MP4ESDAtomPtr*) &newAtom );
			break;

		case MP4MediaInformationAtomType:
			err = MP4CreateMediaInformationAtom( (MP4MediaInformationAtomPtr*) &newAtom );
			break;

		case MP4TrackHeaderAtomType:
			err = MP4CreateTrackHeaderAtom( (MP4TrackHeaderAtomPtr*) &newAtom );
			break;
			
		case MP4TrackReferenceAtomType:
			err = MP4CreateTrackReferenceAtom( (MP4TrackReferenceAtomPtr*) &newAtom );
			break;
			
		case MP4MediaAtomType:
			err = MP4CreateMediaAtom( (MP4MediaAtomPtr*) &newAtom );
			break;

			// PP MAF ->

		case MP4MetadataAtomType:
			err = MP4CreateMetadataAtom( (MP4MetadataAtomPtr*) &newAtom );
			break;
		
		case MP4BinaryXMLAtomType:
			err = MP4CreateBinaryXMLAtom( (MP4BinaryXMLAtomPtr*) &newAtom );
			break;

			// -> PP MAF

			/* JPEG-2000 atom ("box") types */
		case MJ2JPEG2000SignatureAtomType:
			err = MJ2CreateSignatureAtom( (MJ2JPEG2000SignatureAtomPtr*) &newAtom );
			break;
			
			// Photo Player version
		/*case MJ2FileTypeAtomType:
			err = MJ2CreateFileTypeAtom( (MJ2FileTypeAtomPtr*) &newAtom );
			break;*/

		case MP4FileTypeAtomType:
			err = MP4CreateFileTypeAtom( (MP4FileTypeAtomPtr*) &newAtom );
			break;
			
		case MJ2ImageHeaderAtomType:
			err = MJ2CreateImageHeaderAtom( (MJ2ImageHeaderAtomPtr*) &newAtom );
			break;
			
		case MJ2BitsPerComponentAtomType:
			err = MJ2CreateBitsPerComponentAtom( (MJ2BitsPerComponentAtomPtr*) &newAtom );
			break;
			
		case MJ2ColorSpecificationAtomType:
			err = MJ2CreateColorSpecificationAtom( (MJ2ColorSpecificationAtomPtr*) &newAtom );
			break;
			
		case MJ2JP2HeaderAtomType:
			err = MJ2CreateHeaderAtom( (MJ2HeaderAtomPtr*) &newAtom );
			break;
			
		default:
			err = MP4CreateUnknownAtom( (MP4UnknownAtomPtr*) &newAtom );
			break;
	}
	if ( err == MP4NoErr )
		*outAtom = newAtom;
	return err;
}

void MP4TypeToString( u32 inType, char* ioStr )
{
	u32 i;
	int ch;
	
	for ( i = 0; i < 4; i++, ioStr++ )
	{
		ch = inType >> (8*(3-i)) & 0xff;
		if ( isprint(ch) )
			*ioStr = ch;
		else
			*ioStr = '.';
	}
	*ioStr = 0;
}

MP4Err MP4ParseAtomUsingProtoList( MP4InputStreamPtr inputStream, u32* protoList, u32 defaultAtom, MP4AtomPtr *outAtom  )
{
	MP4AtomPtr atomProto;
	MP4Err     err;
	long       bytesParsed;
	MP4Atom	   protoAtom;
	MP4AtomPtr newAtom;
	char       typeString[ 8 ];
	char       msgString[ 80 ];
	u32        beginAvail;
	u32        consumedBytes;
	u32        useDefaultAtom;
	
	atomProto = &protoAtom;
	err = MP4NoErr;
	bytesParsed = 0L;
	
	if ((inputStream == NULL) || (outAtom == NULL) )
		BAILWITHERROR( MP4BadParamErr )
	*outAtom = NULL;
	beginAvail = inputStream->available;
	useDefaultAtom = 0;
	inputStream->msg( inputStream, "{" );
	inputStream->indent++;
	err = MP4CreateBaseAtom( atomProto ); if ( err ) goto bail;

	/* atom size */
	err = inputStream->read32( inputStream, &atomProto->size, NULL ); if ( err ) goto bail;
	if ( atomProto->size == 0 )
		BAILWITHERROR( MP4NoQTAtomErr )
	if ( (atomProto->size < 0) || ((atomProto->size - 4) > inputStream->available) ) 
		BAILWITHERROR( MP4BadDataErr )
	bytesParsed += 4L;
	
	sprintf( msgString, "atom size is %d", atomProto->size );
	inputStream->msg( inputStream, msgString );

	/* atom type */
	err = inputStream->read32( inputStream, &atomProto->type, NULL ); if ( err ) goto bail;
	bytesParsed += 4L;
	MP4TypeToString( atomProto->type, typeString );
	sprintf( msgString, "atom type is '%s'", typeString );
	inputStream->msg( inputStream, msgString );
	if ( atomProto->type == MP4ExtendedAtomType )
	{
		err = inputStream->readData( inputStream, 16, (char*) atomProto->uuid, NULL );	if ( err ) goto bail;
		bytesParsed += 16L;
	}
	
	/* large atom */
	if ( atomProto->size == 1 )
	{
		u32 size;
		err = inputStream->read32( inputStream, &size, NULL ); if ( err ) goto bail;
		if ( size )
			BAILWITHERROR( MP4NoLargeAtomSupportErr )
		atomProto->size64 = size;
        atomProto->size64 <<= 32;
		err = inputStream->read32( inputStream, &size, NULL ); if ( err ) goto bail;
		atomProto->size64 |= size;
		atomProto->size = size;
		bytesParsed += 8L;
	}

	atomProto->bytesRead = bytesParsed;
	if ( atomProto->size - bytesParsed < 0 )
		BAILWITHERROR( MP4BadDataErr )
	if ( protoList )
	{
		while ( *protoList  )
		{
			if ( *protoList == atomProto->type )
				break;
			protoList++;
		}
		if ( *protoList == 0 )
		{
			useDefaultAtom = 1;
		}
	}
	err = MP4CreateAtom( useDefaultAtom ? defaultAtom : atomProto->type, &newAtom ); if ( err ) goto bail;
	sprintf( msgString, "atom name is '%s'", newAtom->name );
	inputStream->msg( inputStream, msgString );
	err = newAtom->createFromInputStream( newAtom, atomProto, (char*) inputStream ); if ( err ) goto bail;
	consumedBytes = beginAvail - inputStream->available;
	if ( consumedBytes != atomProto->size )
	{
		sprintf( msgString, "##### atom size is %d but parse used %d bytes ####", atomProto->size, consumedBytes );
		inputStream->msg( inputStream, msgString );
	}
	*outAtom = newAtom;
	inputStream->indent--;
	inputStream->msg( inputStream, "}" );
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4ParseAtom( MP4InputStreamPtr inputStream, MP4AtomPtr *outAtom  )
{
#if 0
	MP4AtomPtr atomProto;
	MP4Err     err;
	long       bytesParsed;
	MP4Atom	   protoAtom;
	MP4AtomPtr newAtom;
	char       typeString[ 8 ];
	char       msgString[ 80 ];
	u32        beginAvail;
	u32        consumedBytes;
	atomProto = &protoAtom;
	err = MP4NoErr;
	bytesParsed = 0L;
	
	if ((inputStream == NULL) || (outAtom == NULL) )
		BAILWITHERROR( MP4BadParamErr )
	*outAtom = NULL;
	beginAvail = inputStream->available;

	inputStream->msg( inputStream, "{" );
	inputStream->indent++;
	err = MP4CreateBaseAtom( atomProto ); if ( err ) goto bail;

	/* atom size */
	err = inputStream->read32( inputStream, &atomProto->size, NULL ); if ( err ) goto bail;
	if ( atomProto->size == 0 ) BAILWITHERROR( MP4NoQTAtomErr )
	if ( (atomProto->size < 0) || ((atomProto->size - 4) > inputStream->available) )  BAILWITHERROR( MP4BadDataErr )
	bytesParsed += 4L;
	
	sprintf( msgString, "atom size is %d", atomProto->size );
	inputStream->msg( inputStream, msgString );

	/* atom type */
	err = inputStream->read32( inputStream, &atomProto->type, NULL ); if ( err ) goto bail;
	bytesParsed += 4L;
	MP4TypeToString( atomProto->type, typeString );
	sprintf( msgString, "atom type is '%s'", typeString );
	inputStream->msg( inputStream, msgString );
	if ( atomProto->type == MP4ExtendedAtomType )
	{
		err = inputStream->readData( inputStream, 16, (char*) atomProto->uuid, NULL );	if ( err ) goto bail;
		bytesParsed += 16L;
	}
	
	/* large atom */
	if ( atomProto->size == 1 )
	{
		u32 size;
		err = inputStream->read32( inputStream, &size, NULL ); if ( err ) goto bail;
		if ( size )
			BAILWITHERROR( MP4NoLargeAtomSupportErr )
		atomProto->size64 = size;
        atomProto->size64 <<= 32;
		err = inputStream->read32( inputStream, &size, NULL ); if ( err ) goto bail;
		atomProto->size64 |= size;
		atomProto->size = size;
		bytesParsed += 8L;
	}

	atomProto->bytesRead = bytesParsed;
	if ( atomProto->size - bytesParsed < 0 )
		BAILWITHERROR( MP4BadDataErr )
	err = MP4CreateAtom( atomProto->type, &newAtom ); if ( err ) goto bail;
	sprintf( msgString, "atom name is '%s'", newAtom->name );
	inputStream->msg( inputStream, msgString );
	err = newAtom->createFromInputStream( newAtom, atomProto, (char*) inputStream ); if ( err ) goto bail;
	consumedBytes = beginAvail - inputStream->available;
	if ( consumedBytes != atomProto->size )
	{
		sprintf( msgString, "##### atom size is %d but parse used %d bytes ####", atomProto->size, consumedBytes );
		inputStream->msg( inputStream, msgString );
	}
	*outAtom = newAtom;
	inputStream->indent--;
	inputStream->msg( inputStream, "}" );
bail:
	TEST_RETURN( err );
#else
	MP4Err err = MP4ParseAtomUsingProtoList( inputStream, NULL, 0, outAtom  );

#endif
	return err;
}

MP4Err MP4CalculateBaseAtomFieldSize( struct MP4Atom* self )
{
	self->size = 8;
	return MP4NoErr;
}

MP4Err MP4CalculateFullAtomFieldSize( struct MP4FullAtom* self )
{
	MP4Err err = MP4CalculateBaseAtomFieldSize( (MP4AtomPtr) self );
	self->size += 4;
	return err;
}

MP4Err MP4SerializeCommonBaseAtomFields( struct MP4Atom* self, char* buffer )
{
	MP4Err err;
	err = MP4NoErr;
	self->bytesWritten = 0;
	assert( self->size );
	assert( self->type );
	PUT32( size );
	PUT32( type );
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4SerializeCommonFullAtomFields( struct MP4FullAtom* self, char* buffer )
{
	MP4Err err;
	err = MP4SerializeCommonBaseAtomFields( (MP4AtomPtr) self, buffer ); if (err) goto bail;
	buffer += self->bytesWritten;
	PUT8( version );
	PUT24( flags );
bail:
	TEST_RETURN( err );

	return err;
}
