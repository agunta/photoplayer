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
	$Id: MP4Atoms.h,v 1.44 2000/03/04 07:58:05 mc Exp $
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

#ifndef INCLUDED_MP4ATOMS_H
#define INCLUDED_MP4ATOMS_H

#include "MP4Movies.h"
#include "MP4Impl.h"
#include "MP4InputStream.h"
#include "MP4LinkedList.h"

enum
{
	MP4AudioSampleEntryAtomType            = MP4_FOUR_CHAR_CODE( 'm', 'p', '4', 'a' ),
	MP4ChunkLargeOffsetAtomType            = MP4_FOUR_CHAR_CODE( 'c', 'o', '6', '4' ),
	MP4ChunkOffsetAtomType                 = MP4_FOUR_CHAR_CODE( 's', 't', 'c', 'o' ),
	MP4ClockReferenceMediaHeaderAtomType   = MP4_FOUR_CHAR_CODE( 'c', 'r', 'h', 'd' ),
	MP4CompositionOffsetAtomType           = MP4_FOUR_CHAR_CODE( 'c', 't', 't', 's' ),
	MP4CopyrightAtomType                   = MP4_FOUR_CHAR_CODE( 'c', 'p', 'r', 't' ),
	MP4DataEntryURLAtomType                = MP4_FOUR_CHAR_CODE( 'u', 'r', 'l', ' ' ),
	MP4DataEntryURNAtomType                = MP4_FOUR_CHAR_CODE( 'u', 'r', 'n', ' ' ),
	MP4DataInformationAtomType             = MP4_FOUR_CHAR_CODE( 'd', 'i', 'n', 'f' ),
	MP4DataReferenceAtomType               = MP4_FOUR_CHAR_CODE( 'd', 'r', 'e', 'f' ),
	MP4DegradationPriorityAtomType         = MP4_FOUR_CHAR_CODE( 's', 't', 'd', 'p' ),
	MP4ESDAtomType                         = MP4_FOUR_CHAR_CODE( 'e', 's', 'd', 's' ),
	MP4EditAtomType                        = MP4_FOUR_CHAR_CODE( 'e', 'd', 't', 's' ),
	MP4EditListAtomType                    = MP4_FOUR_CHAR_CODE( 'e', 'l', 's', 't' ),
	MP4ExtendedAtomType                    = MP4_FOUR_CHAR_CODE( 'u', 'u', 'i', 'd' ),
	MP4FreeSpaceAtomType                   = MP4_FOUR_CHAR_CODE( 'f', 'r', 'e', 'e' ),
	MP4GenericSampleEntryAtomType		   = MP4_FOUR_CHAR_CODE( '!', 'g', 'n', 'r' ),
	MP4HandlerAtomType                     = MP4_FOUR_CHAR_CODE( 'h', 'd', 'l', 'r' ),
	MP4HintMediaHeaderAtomType             = MP4_FOUR_CHAR_CODE( 'h', 'm', 'h', 'd' ),
	MP4HintTrackReferenceAtomType          = MP4_FOUR_CHAR_CODE( 'h', 'i', 'n', 't' ),
	MP4MPEGMediaHeaderAtomType             = MP4_FOUR_CHAR_CODE( 'n', 'm', 'h', 'd' ),
	MP4MPEGSampleEntryAtomType             = MP4_FOUR_CHAR_CODE( 'm', 'p', '4', 's' ),
	MP4MediaAtomType                       = MP4_FOUR_CHAR_CODE( 'm', 'd', 'i', 'a' ),
	MP4MediaDataAtomType                   = MP4_FOUR_CHAR_CODE( 'm', 'd', 'a', 't' ),
	MP4MediaHeaderAtomType                 = MP4_FOUR_CHAR_CODE( 'm', 'd', 'h', 'd' ),
	MP4MediaInformationAtomType            = MP4_FOUR_CHAR_CODE( 'm', 'i', 'n', 'f' ),
	MP4MovieAtomType                       = MP4_FOUR_CHAR_CODE( 'm', 'o', 'o', 'v' ),
	MP4MovieHeaderAtomType                 = MP4_FOUR_CHAR_CODE( 'm', 'v', 'h', 'd' ),
	MP4ObjectDescriptorAtomType            = MP4_FOUR_CHAR_CODE( 'i', 'o', 'd', 's' ),
	MP4ObjectDescriptorMediaHeaderAtomType = MP4_FOUR_CHAR_CODE( 'o', 'd', 'h', 'd' ),
	MP4ODTrackReferenceAtomType            = MP4_FOUR_CHAR_CODE( 'm', 'p', 'o', 'd' ),
	MP4SampleDescriptionAtomType           = MP4_FOUR_CHAR_CODE( 's', 't', 's', 'd' ),
	MP4SampleSizeAtomType                  = MP4_FOUR_CHAR_CODE( 's', 't', 's', 'z' ),
	MP4SampleTableAtomType                 = MP4_FOUR_CHAR_CODE( 's', 't', 'b', 'l' ),
	MP4SampleToChunkAtomType               = MP4_FOUR_CHAR_CODE( 's', 't', 's', 'c' ),
	MP4SceneDescriptionMediaHeaderAtomType = MP4_FOUR_CHAR_CODE( 's', 'd', 'h', 'd' ),
	MP4ShadowSyncAtomType                  = MP4_FOUR_CHAR_CODE( 's', 't', 's', 'h' ),
	MP4SkipAtomType                        = MP4_FOUR_CHAR_CODE( 's', 'k', 'i', 'p' ),
	MP4SoundMediaHeaderAtomType            = MP4_FOUR_CHAR_CODE( 's', 'm', 'h', 'd' ),
	MP4StreamDependenceAtomType            = MP4_FOUR_CHAR_CODE( 'd', 'p', 'n', 'd' ),
	MP4SyncSampleAtomType                  = MP4_FOUR_CHAR_CODE( 's', 't', 's', 's' ),
	MP4SyncTrackReferenceAtomType          = MP4_FOUR_CHAR_CODE( 's', 'y', 'n', 'c' ),
	MP4TimeToSampleAtomType                = MP4_FOUR_CHAR_CODE( 's', 't', 't', 's' ),
	MP4TrackAtomType                       = MP4_FOUR_CHAR_CODE( 't', 'r', 'a', 'k' ),
	MP4TrackHeaderAtomType                 = MP4_FOUR_CHAR_CODE( 't', 'k', 'h', 'd' ),
	MP4TrackReferenceAtomType              = MP4_FOUR_CHAR_CODE( 't', 'r', 'e', 'f' ),
	MP4UserDataAtomType                    = MP4_FOUR_CHAR_CODE( 'u', 'd', 't', 'a' ),
	MP4VideoMediaHeaderAtomType            = MP4_FOUR_CHAR_CODE( 'v', 'm', 'h', 'd' ),
	MP4VisualSampleEntryAtomType           = MP4_FOUR_CHAR_CODE( 'm', 'p', '4', 'v' ),
	// added, PP MAF
	MP4FileTypeAtomType                    = MP4_FOUR_CHAR_CODE( 'f', 't', 'y', 'p' ),
	MP4BinaryXMLAtomType                   = MP4_FOUR_CHAR_CODE( 'b', 'x', 'm', 'l' ),
	MP4MetadataAtomType                    = MP4_FOUR_CHAR_CODE( 'm', 'e', 't', 'a' ),
	MP4PrimaryItemAtomType                 = MP4_FOUR_CHAR_CODE( 'p', 'i', 't', 'm' ),
	MP4ItemLocationAtomType                = MP4_FOUR_CHAR_CODE( 'i', 'l', 'o', 'c' ),
	MP4ItemProtectionAtomType              = MP4_FOUR_CHAR_CODE( 'i', 'p', 'r', 'o' ),
	MP4ItemInfoAtomType                    = MP4_FOUR_CHAR_CODE( 'i', 'i', 'n', 'f' ),
};

#define MP4_BASE_ATOM    \
	u32	   type;         \
	u8   uuid[16];       \
	u32    size;         \
	u64    size64;       \
	u32    bytesRead;    \
	u32    bytesWritten; \
	char   *name;        \
	struct MP4Atom* super; \
	MP4Err (*createFromInputStream)( struct MP4Atom* self, struct MP4Atom* proto, /*struct MP4InputStreamRecord* */ char *inputStream ); \
	char*  (*getName)( struct MP4Atom* self ); \
	MP4Err (*serialize)( struct MP4Atom* self, char* buffer ); \
	MP4Err (*calculateSize)( struct MP4Atom* self ); \
	void   (*destroy)( struct MP4Atom* self );

#define MP4_FULL_ATOM \
	MP4_BASE_ATOM     \
	u32 version;      \
	u32 flags;


typedef struct MP4Atom
{
	MP4_BASE_ATOM
} MP4Atom, *MP4AtomPtr;

typedef struct MP4FullAtom
{
	MP4_FULL_ATOM
} MP4FullAtom, *MP4FullAtomPtr;

typedef MP4Err (*cisfunc)( struct MP4Atom* self, struct MP4Atom* proto, char *inputStream );

typedef struct MP4PrivateMovieRecord
{
      u32  referenceCount;
      struct FileMappingObjectRecord *fileMappingObject;
      struct MP4InputStreamRecord *inputStream;
      MP4AtomPtr moovAtomPtr;
      MP4AtomPtr mdat;
      MP4AtomPtr ftyp;              /* file type atom */

      /*MP4Track initialBIFS;*/
      /*MP4Track initialOD; */
      u32 fileType;					/* the file type: MPEG-4, Motion JPEG-2000, or QuickTime */
      void *inMemoryDataHandler;
} MP4PrivateMovieRecord, *MP4PrivateMovieRecordPtr;

MP4Err MP4CreateAtom( u32 atomType, MP4AtomPtr *outAtom );
MP4Err MP4CreateBaseAtom( MP4AtomPtr self );
MP4Err MP4CreateFullAtom( MP4AtomPtr s );
MP4Err MP4ParseAtom( struct MP4InputStreamRecord* inputStream, MP4AtomPtr *outAtom );
MP4Err MP4SerializeCommonBaseAtomFields( struct MP4Atom* self, char* buffer );
MP4Err MP4SerializeCommonFullAtomFields( struct MP4FullAtom* self, char* buffer );
MP4Err MP4CalculateBaseAtomFieldSize( struct MP4Atom* self );
MP4Err MP4CalculateFullAtomFieldSize( struct MP4FullAtom* self );

void MP4TypeToString( u32 inType, char* ioStr );


// 2006.07 added for Photo Player

enum
{
	MAFPhotoPlayerBrand						= MP4_FOUR_CHAR_CODE( 'M', 'P', 'P', 'l' ),		/* brand for PP MAF ? */
	//MAFPhotoPlayerBrand						= MP4_FOUR_CHAR_CODE( 'M', 'P', '4', '2' ),		/* brand for PP MAF ? */
};


typedef struct MP4FileTypeAtom
{
	MP4_BASE_ATOM
	
	ISOErr (*addStandard)(struct MP4FileTypeAtom *self, u32 standard );
	u32 brand;											/* the brand of this file */
	u32 minorVersion;									/* the minor version of this file */
	u32 itemCount;										/* the number of items in the compatibility list */
	u32 *compatibilityList;								/* standards this file conforms to */
} MP4FileTypeAtom, *MP4FileTypeAtomPtr;



typedef struct MP4MediaDataAtom
{
	MP4_BASE_ATOM
	MP4Err (*addData)( struct MP4MediaDataAtom *self, MP4Handle dataH );
	MP4Err (*writeToFile)( struct MP4MediaDataAtom *self, FILE *fd );

	char *data;
	u64 dataSize;
} MP4MediaDataAtom, *MP4MediaDataAtomPtr;

typedef struct MP4UnknownAtom
{
	MP4_BASE_ATOM
	char *data;
	u32 dataSize;
} MP4UnknownAtom, *MP4UnknownAtomPtr;

typedef struct MP4MovieAtom
{
	MP4_BASE_ATOM
	MP4Err (*setupTracks)( struct MP4MovieAtom* self, MP4PrivateMovieRecordPtr moov );
	u32    (*getTrackCount)( struct MP4MovieAtom* self );
	MP4Err (*getIndTrack)( struct MP4MovieAtom* self, u32 trackNumber, MP4AtomPtr *outTrack );
	MP4Err (*addAtom)( struct MP4MovieAtom* self, MP4AtomPtr atom );
	MP4Err (*getNextTrackID)( struct MP4MovieAtom* self, u32 *outID );
	MP4Err (*addTrack)( struct MP4MovieAtom* self, MP4AtomPtr track );
	MP4Err (*newTrack)( struct MP4MovieAtom* self, u32 newTrackFlags, MP4AtomPtr *outTrack );
	MP4Err (*newTrackWithID)( struct MP4MovieAtom* self, u32 newTrackFlags, u32 newTrackID, MP4AtomPtr *outTrack );
	MP4Err (*newMetadata)( struct MP4MovieAtom *self, MP4Metadata *outMetadta, u32 handlerType, MP4Handle data );
	MP4Err (*calculateDuration)( struct MP4MovieAtom* self );
	MP4Err (*setTimeScale)( struct MP4MovieAtom* self, u32 timeScale );
	MP4Err (*getTimeScale)( struct MP4MovieAtom* self, u32 *outTimeScale );
	MP4Err (*setMatrix)( struct MP4MovieAtom* self, u32 matrix[9] );
	MP4Err (*getMatrix)( struct MP4MovieAtom* self, u32 outMatrix[9] );
	MP4Err (*setPreferredRate)( struct MP4MovieAtom* self, u32 rate );
	MP4Err (*getPreferredRate)( struct MP4MovieAtom* self, u32 *outRate );
	MP4Err (*setPreferredVolume)( struct MP4MovieAtom* self, s16 volume );
	MP4Err (*getPreferredVolume)( struct MP4MovieAtom* self, s16 *outVolume );
	MP4Err (*mdatMoved)( struct MP4MovieAtom* self, s32 mdatOffset );

	MP4AtomPtr mvhd;              /* the movie header */
	MP4AtomPtr iods;              /* the initial object descriptor */
	//MP4AtomPtr ftyp;              /* for JPEG-2000, the file type atom */
	MP4AtomPtr jp2h;              /* for JPEG-2000, the JP2 header atom */
	MP4AtomPtr sgnt;              /* for JPEG-2000, the signature atom */
	MP4AtomPtr udta;		      /* user data */
	MP4AtomPtr meta;		      /* metadata */
	MP4PrivateMovieRecordPtr moov;
	MP4LinkedList atomList;
	MP4LinkedList trackList;
} MP4MovieAtom, *MP4MovieAtomPtr;

typedef struct MP4MovieHeaderAtom
{
	MP4_FULL_ATOM
	u64	creationTime;
	u64	modificationTime;
	u32 timeScale;
	u64 duration;
	u32 qt_preferredRate;
	u32 qt_preferredVolume;
	char qt_reserved[10];
	u32	qt_matrixA;
	u32 qt_matrixB;
	u32 qt_matrixU;
	u32 qt_matrixC;
	u32 qt_matrixD;
	u32 qt_matrixV;
	u32 qt_matrixX;
	u32 qt_matrixY;
	u32 qt_matrixW;
	u32 qt_previewTime;
	u32 qt_previewDuration;
	u32 qt_posterTime;
	u32 qt_selectionTime;
	u32 qt_selectionDuration;
	u32 qt_currentTime;
	u32	nextTrackID;
} MP4MovieHeaderAtom, *MP4MovieHeaderAtomPtr;

typedef struct MP4ObjectDescriptorAtom
{
	MP4_FULL_ATOM
    MP4Err (*setDescriptor)( struct MP4Atom* self, /*struct MP4DescriptorRecord */ char * desc );
	u32  ODSize;
	struct MP4DescriptorRecord* descriptor;
} MP4ObjectDescriptorAtom, *MP4ObjectDescriptorAtomPtr;

typedef struct MP4TrackAtom
{
	MP4_BASE_ATOM
	MP4PrivateMovieRecordPtr moov;
	MP4Err (*addAtom)( struct MP4TrackAtom* self, MP4AtomPtr atom );
	MP4Err (*setMoov)( struct MP4TrackAtom *self, MP4PrivateMovieRecordPtr moov );
	MP4Err (*setMdat)( struct MP4TrackAtom *self, MP4AtomPtr mdat );
	MP4Err (*newMedia)( struct MP4TrackAtom *self, MP4Media *outMedia, u32 mediaType, u32 timeScale, MP4Handle dataURL );
	MP4Err (*newMetadata)( struct MP4TrackAtom *self, MP4Metadata *outMetadata, u32 handlerType, MP4Handle data );
	MP4Err (*setEnabled)( struct MP4TrackAtom *self, u32 enabled );
	MP4Err (*getEnabled)( struct MP4TrackAtom *self, u32 *outEnabled );
	MP4Err (*calculateDuration)( struct MP4TrackAtom *self, u32 movieTimeScale );
	MP4Err (*getDuration)( struct MP4TrackAtom *self, u32 *outDuration );
	MP4Err (*setMatrix)( struct MP4TrackAtom* self, u32 matrix[9] );
	MP4Err (*getMatrix)( struct MP4TrackAtom* self, u32 outMatrix[9] );
	MP4Err (*setLayer)( struct MP4TrackAtom* self, s16 layer );
	MP4Err (*getLayer)( struct MP4TrackAtom* self, s16 *outLayer );
	MP4Err (*setDimensions)( struct MP4TrackAtom* self, u32 width, u32 height );
	MP4Err (*getDimensions)( struct MP4TrackAtom* self, u32 *outWidth, u32 *outHeight );
	MP4Err (*setVolume)( struct MP4TrackAtom* self, s16 volume );
	MP4Err (*getVolume)( struct MP4TrackAtom* self, s16 *outVolume );
	MP4Err (*mdatMoved)( struct MP4TrackAtom* self, s32 mdatOffset );

	u32 newTrackFlags;
	MP4AtomPtr mdat;
	MP4AtomPtr udta;
	MP4AtomPtr trackHeader;
	MP4AtomPtr trackMedia;
	MP4AtomPtr trackMetadata;
	MP4AtomPtr trackEditAtom;
	MP4AtomPtr trackReferences;
	MP4LinkedList atomList;
} MP4TrackAtom, *MP4TrackAtomPtr;

typedef struct MP4TrackHeaderAtom
{
	MP4_FULL_ATOM
	u64	creationTime;
	u64	modificationTime;
	u32 trackID;
	u32 qt_reserved1;
	u64 duration;
	char qt_reserved2[8];
	u32 qt_layer;
	u32 qt_alternateGroup;
	u32	qt_volume;
	u32 qt_reserved3;
	u32	qt_matrixA;
	u32 qt_matrixB;
	u32 qt_matrixU;
	u32 qt_matrixC;
	u32 qt_matrixD;
	u32 qt_matrixV;
	u32 qt_matrixX;
	u32 qt_matrixY;
	u32 qt_matrixW;
	u32 qt_trackWidth;
	u32 qt_trackHeight;
} MP4TrackHeaderAtom, *MP4TrackHeaderAtomPtr;

typedef struct MP4TrackReferenceAtom
{
	MP4_BASE_ATOM
	MP4Err (*addAtom)( struct MP4TrackReferenceAtom *self, MP4AtomPtr atom );
	MP4Err (*findAtomOfType)( struct MP4TrackReferenceAtom *self, u32 atomType, MP4AtomPtr *outAtom );
	MP4LinkedList atomList;
} MP4TrackReferenceAtom, *MP4TrackReferenceAtomPtr;

typedef struct MP4MediaAtom
{
	MP4_BASE_ATOM
	MP4Err (*setupNew)( struct MP4MediaAtom *self, MP4AtomPtr track, u32 mediaType, u32 timeScale, MP4Handle dataHandlerH );
	MP4Err (*addSampleReference)( struct MP4MediaAtom *self, u64 dataOffset, u32 sampleCount,
				    MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
				    MP4Handle decodingOffsetsH, MP4Handle syncSamplesH );
	MP4Err (*addSamples)( struct MP4MediaAtom *self, MP4Handle sampleH, u32 sampleCount,
				    MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
				    MP4Handle decodingOffsetsH, MP4Handle syncSamplesH );
	MP4Err (*calculateDuration)( struct MP4MediaAtom *self );
	MP4Err (*mdatMoved)( struct MP4MediaAtom* self, s32 mdatOffset );
	
	MP4AtomPtr mediaTrack;
	MP4AtomPtr mediaHeader;
	MP4AtomPtr handler;
	MP4AtomPtr information;
	MP4LinkedList atomList;
} MP4MediaAtom, *MP4MediaAtomPtr;

typedef struct MP4MediaHeaderAtom
{
	MP4_FULL_ATOM
	u64	creationTime;
	u64	modificationTime;
	u32 timeScale;
	u64 duration;
	u32 packedLanguage;
	u32	qt_quality;
} MP4MediaHeaderAtom, *MP4MediaHeaderAtomPtr;

typedef struct MP4HandlerAtom
{
	MP4_FULL_ATOM
	MP4Err (*setName)( struct MP4Atom* s, char* name );
	u32 nameLength;
	u32	qt_componentType;
	u32	handlerType;
	u32 qt_componentManufacturer;
	u32 qt_componentFlags;
	u32 qt_componentFlagsMask;
	char *nameUTF8;
} MP4HandlerAtom, *MP4HandlerAtomPtr;

typedef struct MP4MediaInformationAtom
{
	MP4_BASE_ATOM
	MP4Err (*closeDataHandler)( MP4AtomPtr self );
	MP4Err (*openDataHandler)( MP4AtomPtr self, u32 dataEntryIndex );
	MP4Err (*setupNewMedia)( struct MP4MediaInformationAtom *self, u32 mediaType, MP4Handle dataH, MP4AtomPtr mdat );
	MP4Err (*getMediaDuration)( struct MP4MediaInformationAtom *self, u32 *outDuration );
	MP4Err (*addSampleReference)( struct MP4MediaInformationAtom *self, u64 dataOffset, u32 sampleCount,
				    MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
				    MP4Handle decodingOffsetsH, MP4Handle syncSamplesH );
	MP4Err (*addSamples)( struct MP4MediaInformationAtom *self, MP4Handle sampleH, 
					u32 sampleCount, MP4Handle durationsH, MP4Handle sizesH,
					MP4Handle sampleEntryH,
				    MP4Handle decodingOffsetsH, MP4Handle syncSamplesH );
	MP4Err (*mdatMoved)( struct MP4MediaInformationAtom *self, s32 mdatOffset );
	MP4Err (*testDataEntry)( struct MP4MediaInformationAtom *self, u32 dataEntryIndex );
	MP4AtomPtr dataInformation;
	MP4AtomPtr sampleTable;
	MP4AtomPtr mediaHeader;
	struct MP4InputStreamRecord* inputStream;
	void       *dataHandler;
	u32         dataEntryIndex;
	MP4LinkedList atomList;
} MP4MediaInformationAtom, *MP4MediaInformationAtomPtr;

typedef struct MP4VideoMediaHeaderAtom
{
	MP4_FULL_ATOM
	u32 graphicsMode;
	u32 opColorRed;
	u32 opColorGreen;
	u32 opColorBlue;
} MP4VideoMediaHeaderAtom, *MP4VideoMediaHeaderAtomPtr;

typedef struct MP4SoundMediaHeaderAtom
{
	MP4_FULL_ATOM
	u32 balance;
	u32 reserved;
} MP4SoundMediaHeaderAtom, *MP4SoundMediaHeaderAtomPtr;

typedef struct MP4HintMediaHeaderAtom
{
	MP4_FULL_ATOM
	u32	maxPDUSize;
	u32 avgPDUSize;
	u32 maxBitrate;
	u32 avgBitrate;
	u32 slidingAverageBitrate;
} MP4HintMediaHeaderAtom, *MP4HintMediaHeaderAtomPtr;

typedef struct MP4MPEGMediaHeaderAtom
{
	MP4_FULL_ATOM
} MP4MPEGMediaHeaderAtom, *MP4MPEGMediaHeaderAtomPtr;

typedef struct MP4ObjectDescriptorMediaHeaderAtom
{
	MP4_FULL_ATOM
} MP4ObjectDescriptorMediaHeaderAtom, *MP4ObjectDescriptorMediaHeaderAtomPtr;

typedef struct MP4ClockReferenceMediaHeaderAtom
{
	MP4_FULL_ATOM
} MP4ClockReferenceMediaHeaderAtom, *MP4ClockReferenceMediaHeaderAtomPtr;

typedef struct MP4SceneDescriptionMediaHeaderAtom
{
	MP4_FULL_ATOM
} MP4SceneDescriptionMediaHeaderAtom, *MP4SceneDescriptionMediaHeaderAtomPtr;

typedef struct MP4DataInformationAtom
{
	MP4_BASE_ATOM
	MP4Err (*getOffset)( struct MP4DataInformationAtom *self, u32 dataReferenceIndex, u32 *outOffset );
	MP4Err (*addSampleReference)( struct MP4DataInformationAtom *self, u32 sampleCount, u32 dataReferenceIndex, u64 dataOffset, MP4Handle sizesH );
	MP4Err (*addSamples)( struct MP4DataInformationAtom *self, u32 sampleCount, u32 dataReferenceIndex, MP4Handle sampleH );
	MP4Err (*addAtom)( struct MP4DataInformationAtom *self, MP4AtomPtr atom );

	MP4AtomPtr dataReference;
	MP4LinkedList atomList;
} MP4DataInformationAtom, *MP4DataInformationAtomPtr;

#define COMMON_DATAENTRY_ATOM_FIELDS \
	MP4Err (*getOffset)( struct MP4DataEntryAtom *self, u32 *outOffset ); \
	MP4Err (*addSampleReference)( struct MP4DataEntryAtom *self, u64 dataOffset, MP4Handle sizesH ); \
	MP4Err (*addSamples)( struct MP4DataEntryAtom *self, MP4Handle sampleH ); \
	\
	MP4AtomPtr mdat; \
	u32   offset; \
	u32   locationLength; \
	char* location;

typedef struct MP4DataEntryAtom
{
	MP4_FULL_ATOM
	COMMON_DATAENTRY_ATOM_FIELDS
} MP4DataEntryAtom, *MP4DataEntryAtomPtr;

typedef struct MP4DataEntryURLAtom
{
	MP4_FULL_ATOM
	COMMON_DATAENTRY_ATOM_FIELDS
} MP4DataEntryURLAtom, *MP4DataEntryURLAtomPtr;

typedef struct MP4DataEntryURNAtom
{
	MP4_FULL_ATOM
	COMMON_DATAENTRY_ATOM_FIELDS
	u32   nameLength;
	char* nameURN;
} MP4DataEntryURNAtom, *MP4DataEntryURNAtomPtr;

typedef struct MP4DataReferenceAtom
{
	MP4_FULL_ATOM
	MP4Err (*addDataEntry)( struct MP4DataReferenceAtom *self, MP4AtomPtr entry );
	MP4Err (*getOffset)( struct MP4DataReferenceAtom *self, u32 dataReferenceIndex, u32 *outOffset );
	MP4Err (*addSampleReference)( struct MP4DataReferenceAtom *self, u32 sampleCount, u32 dataReferenceIndex, u64 dataOffset, MP4Handle sizesH );
	MP4Err (*addSamples)( struct MP4DataReferenceAtom *self, u32 sampleCount, u32 dataReferenceIndex, MP4Handle sampleH );
	u32    (*getEntryCount)( struct MP4DataReferenceAtom *self );
	MP4Err (*getEntry)( struct MP4DataReferenceAtom *self, u32 dataReferenceIndex, struct MP4DataEntryAtom **outEntry );	
	MP4LinkedList atomList;

} MP4DataReferenceAtom, *MP4DataReferenceAtomPtr;

typedef struct MP4SampleTableAtom
{
	MP4_BASE_ATOM
	MP4Err (*setupNew)( struct MP4SampleTableAtom *self );
	MP4Err (*calculateDuration)( struct MP4SampleTableAtom *self, u32 *outDuration );
	MP4Err (*setSampleEntry)( struct MP4SampleTableAtom *self, MP4AtomPtr entry );
	MP4Err (*getCurrentDataReferenceIndex)( struct MP4SampleTableAtom *self, u32 *outDataReferenceIndex );
	MP4Err (*addSamples)( struct MP4SampleTableAtom *self,
							u32 sampleCount, u32 sampleOffset, MP4Handle durationsH,
							MP4Handle sizesH, MP4Handle decodingOffsetsH, 
							MP4Handle syncSamplesH );

	MP4AtomPtr TimeToSample;
	MP4AtomPtr CompositionOffset;
	MP4AtomPtr SyncSample;
	MP4AtomPtr SampleDescription;
	MP4AtomPtr SampleSize;
	MP4AtomPtr SampleToChunk;
	MP4AtomPtr ChunkOffset;
	MP4AtomPtr ShadowSync;
	MP4AtomPtr DegradationPriority;

	MP4LinkedList atomList;
	
	MP4AtomPtr currentSampleEntry;

} MP4SampleTableAtom, *MP4SampleTableAtomPtr;

typedef struct MP4TimeToSampleAtom
{
	MP4_FULL_ATOM
	MP4Err (*getTimeForSampleNumber)( MP4AtomPtr self, u32 sampleNumber, u64 *outSampleCTS, s32 *outSampleDuration );
	MP4Err (*findSamples)( MP4AtomPtr self, u64 desiredTime,
							s64 *outPriorSample, s64 *outExactSample, s64 *outNextSample,
							u32 *outSampleNumber, s32 *outSampleDuration );
	MP4Err (*getTotalDuration)( struct MP4TimeToSampleAtom *self, u32 *outDuration );
	MP4Err (*addSamples)( struct MP4TimeToSampleAtom *self, u32 sampleCount, MP4Handle durationsH );
	
	MP4LinkedList entryList;
	void *currentEntry;
	void *foundEntry;
	u32   foundEntryNumber;
	u32   foundEntrySampleNumber;
	u64   foundEntryTime;
} MP4TimeToSampleAtom, *MP4TimeToSampleAtomPtr;

typedef struct MP4CompositionOffsetAtom
{
	MP4_FULL_ATOM
	MP4Err (*addSamples)(struct MP4CompositionOffsetAtom *self, u32 sampleNumber,  u32 sampleCount, MP4Handle offsetsH );
	MP4Err (*getOffsetForSampleNumber)( MP4AtomPtr self, u32 sampleNumber, s32 *outOffset );
	MP4LinkedList entryList;
	void *currentEntry;
	u32 finalSampleNumber;
} MP4CompositionOffsetAtom, *MP4CompositionOffsetAtomPtr;

#define COMMON_SAMPLE_ENTRY_FIELDS \
	u32 dataReferenceIndex;        \
	MP4AtomPtr ESDAtomPtr;

typedef struct GenericSampleEntryAtom
{
	MP4_BASE_ATOM
	COMMON_SAMPLE_ENTRY_FIELDS
} GenericSampleEntryAtom, *GenericSampleEntryAtomPtr;

typedef struct MP4GenericSampleEntryAtom
{
	MP4_BASE_ATOM
	u32 dataReferenceIndex;
	char reserved[ 6 ];
	char *data;
	u32 dataSize;
} MP4GenericSampleEntryAtom, *MP4GenericSampleEntryAtomPtr;

typedef struct MP4MPEGSampleEntryAtom
{
	MP4_BASE_ATOM
	COMMON_SAMPLE_ENTRY_FIELDS
	char       reserved[ 6 ];
} MP4MPEGSampleEntryAtom, *MP4MPEGSampleEntryAtomPtr;

typedef struct MP4VisualSampleEntryAtom
{
	MP4_BASE_ATOM
	COMMON_SAMPLE_ENTRY_FIELDS
	MP4Err (*setDimensions)( struct MP4VisualSampleEntryAtom* self , u16 width, u16 height);
	char       reserved1[ 6 ];
	char	  reserved2[ 16 ];     /* uint(32)[4] */
	// KW: 2007 changed reserved3 to width and height
	//u32			reserved3;         /* uint(32) = 0x01400f0 */
	u16			width;
	u16			height;
	u32			reserved4;          /* uint(32) = 0x0048000 */
	u32			reserved5;			/* uint(32) = 0x0048000 */
	u32			reserved6; /* uint(32) = 0 */
	u32			reserved7;           /* uint(16) = 1 */
	u32			nameLength;
	char		name31[31];
	u32			reserved8; /* uint(16) = 24 */
	s32			reserved9; /* int(16) = -1 */
	
} MP4VisualSampleEntryAtom, *MP4VisualSampleEntryAtomPtr;

typedef struct MP4AudioSampleEntryAtom
{
	MP4_BASE_ATOM
	COMMON_SAMPLE_ENTRY_FIELDS
	char       reserved1[ 6 ];
	char	  reserved2[ 8 ];     /* uint(32)[2] */
	u32			reserved3;         /* uint(16) = 2 */
	u32        reserved4;          /* uint(16) = 16 */
	u32			reserved5;			/* uint(32) = 0 */
	u32			timeScale;			/* uint(16) copied from track! */
	u32			reserved6; /* uint(16) = 0 */
	
} MP4AudioSampleEntryAtom, *MP4AudioSampleEntryAtomPtr;

typedef struct MP4SampleDescriptionAtom
{
	MP4_FULL_ATOM

	MP4Err (*addEntry)( struct MP4SampleDescriptionAtom *self, MP4AtomPtr entry );
	u32    (*getEntryCount)( struct MP4SampleDescriptionAtom *self );
	MP4Err (*getEntry)( struct MP4SampleDescriptionAtom *self, u32 entryNumber,  struct GenericSampleEntryAtom **outEntry );

	MP4LinkedList atomList;
} MP4SampleDescriptionAtom, *MP4SampleDescriptionAtomPtr;


typedef struct MP4ESDAtom
{
	MP4_FULL_ATOM
	u32 descriptorLength;
	struct MP4DescriptorRecord* descriptor;
} MP4ESDAtom, *MP4ESDAtomPtr;

typedef struct MP4SampleSizeAtom
{
	MP4_FULL_ATOM
	MP4Err (*getSampleSize)( MP4AtomPtr self, u32 sampleNumber, u32 *outSize );
	MP4Err (*getSampleSizeAndOffset)( MP4AtomPtr self, u32 sampleNumber, u32 *outSize, u32 startingSampleNumber, u32 *outOffsetSize );
	MP4Err (*addSamples)( struct MP4SampleSizeAtom *self, u32 sampleCount, MP4Handle sizesH );

	u32	sampleSize;
	u32	sampleCount;
	u32 *sizes;
} MP4SampleSizeAtom, *MP4SampleSizeAtomPtr;

typedef struct MP4ChunkOffsetAtom
{
	MP4_FULL_ATOM
	MP4Err (*getChunkOffset)( MP4AtomPtr self, u32 chunkIndex, u64 *outOffset );
	MP4Err (*addOffset)(struct MP4ChunkOffsetAtom *self, u32 offset );

	u32	entryCount;
	u32 *offsets;
} MP4ChunkOffsetAtom, *MP4ChunkOffsetAtomPtr;

typedef struct MP4ChunkLargeOffsetAtom
{
	MP4_FULL_ATOM
	MP4Err (*getChunkOffset)( MP4AtomPtr self, u32 chunkIndex, u64 *outOffset );
	MP4Err (*addOffset)(struct MP4ChunkOffsetAtom *self, u64 offset );

	u32	entryCount;
	u64 *offsets;
} MP4ChunkLargeOffsetAtom, *MP4ChunkLargeOffsetAtomPtr;

typedef struct MP4SampleToChunkAtom
{
	MP4_FULL_ATOM
	MP4Err (*lookupSample)( MP4AtomPtr self, u32 sampleNumber, u32 *outChunkNumber, u32 *outSampleDescriptionIndex, u32 *outFirstSampleNumberInChunk );
	MP4Err (*setEntry)( struct MP4SampleToChunkAtom *self, u32 beginningSampleNumber, u32 sampleCount, u32 sampleDescriptionIndex );
	u32    (*getEntryCount)( struct MP4SampleToChunkAtom *self );
	MP4Err (*mdatMoved)( struct MP4SampleToChunkAtom *self, s32 mdatOffset,
						 struct MP4SampleDescriptionAtom *stsd, 
	                     struct MP4DataReferenceAtom *dref, struct MP4ChunkOffsetAtom *stco );
	MP4LinkedList entryList;

	u32   foundEntryNumber;
	u32   foundEntryFirstSampleNumber;

} MP4SampleToChunkAtom, *MP4SampleToChunkAtomPtr;

typedef struct MP4SyncSampleAtom
{
	MP4_FULL_ATOM
	MP4Err (*addSamples)(struct MP4SyncSampleAtom *self, u32 beginningSampleNumber, u32 sampleCount, MP4Handle syncSamplesH );
	MP4Err (*isSyncSample)( MP4AtomPtr self, u32 sampleNumber, u32 *outSync );
	u32	entryCount;
	u32 *sampleNumbers;
    u32 nonSyncFlag;
} MP4SyncSampleAtom, *MP4SyncSampleAtomPtr;

typedef struct MP4ShadowSyncAtom
{
	MP4_FULL_ATOM
	u32	entryCount;
	void *entries;
} MP4ShadowSyncAtom, *MP4ShadowSyncAtomPtr;

typedef struct MP4DegradationPriorityAtom
{
	MP4_FULL_ATOM
	u32	entryCount;
	u32 *priorities;
} MP4DegradationPriorityAtom, *MP4DegradationPriorityAtomPtr;

typedef struct MP4FreeSpaceAtom
{
	MP4_BASE_ATOM
	char *data;
	u32 dataSize;
} MP4FreeSpaceAtom, *MP4FreeSpaceAtomPtr;

typedef struct MP4EditAtom
{
	MP4_BASE_ATOM
	MP4Err (*addAtom)( struct MP4EditAtom *self, MP4AtomPtr atom );
	MP4Err (*getEffectiveDuration)( struct MP4EditAtom *self, u32 *outDuration );

	MP4LinkedList atomList;
	MP4AtomPtr editListAtom;
} MP4EditAtom, *MP4EditAtomPtr;

typedef struct MP4EditListAtom
{
	MP4_FULL_ATOM
	
	MP4Err (*setTrackOffset)( struct MP4EditListAtom *self, u32 trackStartTime, u64 trackDuration );
	MP4Err (*getTrackOffset)( struct MP4EditListAtom *self, u32 *outTrackStartTime );
	
	MP4Err (*isEmptyEdit)( struct MP4EditListAtom *self, u32 segmentNumber, u32 *outIsEmpty );
	
	MP4Err (*insertSegment)( struct MP4EditListAtom *self,
								s32 trackStartTime,
								u32 mediaStartTime,
								u64 segmentDuration,
								u32 mediaRate );
								
	MP4Err (*getEffectiveDuration)( struct MP4EditListAtom *self, u32 *outDuration );
	MP4Err (*getIndSegmentTime)( MP4AtomPtr self,
								 u32 segmentIndex,          /* one based */
								 u64 *outSegmentMovieTime,
								 s64 *outSegmentMediaTime,
								 u64 *outSegmentDuration    /* in movie's time scale */
							   );
	MP4Err (*getTimeAndRate)( MP4AtomPtr self, u64 movieTime, u32 movieTimeScale,
							  u32 mediaTimeScale, s64 *outMediaTime, u32 *outMediaRate,
							  u64 *outPriorSegmentEndTime, u64 *outNextSegmentBeginTime );
	u32    (*getEntryCount)( struct MP4EditListAtom *self );
	MP4LinkedList entryList;
} MP4EditListAtom, *MP4EditListAtomPtr;

typedef struct MP4UserDataAtom
{
	MP4_BASE_ATOM
	MP4Err (*addUserData) (struct MP4UserDataAtom *self, MP4Handle userDataH, u32 userDataType, u32 *outIndex );
	MP4Err (*getEntryCount) (struct MP4UserDataAtom *self, u32 userDataType, u32 *outCount );
	MP4Err (*getIndType) (struct MP4UserDataAtom *self, u32 typeIndex, u32 *outType );
	MP4Err (*getItem) (struct MP4UserDataAtom *self, MP4Handle userDataH, u32 userDataType, u32 itemIndex );
	MP4Err (*getTypeCount) (struct MP4UserDataAtom *self, u32 *outCount );
	MP4LinkedList recordList;
} MP4UserDataAtom, *MP4UserDataAtomPtr;

typedef struct MP4CopyrightAtom
{
	MP4_FULL_ATOM
	u32 packedLanguageCode;
	char* notice;
} MP4CopyrightAtom, *MP4CopyrightAtomPtr;

typedef struct MP4TrackReferenceTypeAtom
{
	MP4_BASE_ATOM
	MP4Err (*addTrackID)( struct MP4TrackReferenceTypeAtom *self, u32 trackID );
	u32 trackIDCount;
	u32 *trackIDs;
} MP4TrackReferenceTypeAtom, *MP4TrackReferenceTypeAtomPtr;

// included for Photo Player
typedef struct MP4MetadataAtom
{
	MP4_FULL_ATOM
	MP4Err (*setupNew)( struct MP4MetadataAtom *self, /*MP4AtomPtr track,*/ u32 dataType, MP4Handle dataHandlerH);
	MP4Err (*addAtom)( struct MP4MetadataAtom* self, MP4AtomPtr atom );

	MP4LinkedList atomList;
	MP4AtomPtr handler;
	MP4AtomPtr bim;
} MP4MetadataAtom, *MP4MetadataAtomPtr;

typedef struct MP4BinaryXMLAtom
{
	MP4_FULL_ATOM
	MP4Err (*addData)( struct MP4BinaryXMLAtom *self, MP4Handle dataH );
	MP4Err (*getData) ( struct MP4BinaryXMLAtom *self, MP4Handle dataH, u32 *dataSize );
	MP4Err (*writeToFile)( struct MP4BinaryXMLAtom *self, FILE *fd );

	char *data;
	u32 dataSize;
} MP4BinaryXMLAtom, *MP4BinaryXMLAtomPtr;

#endif
