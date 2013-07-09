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
	$Id: MP4Movies.h,v 1.59 2000/03/04 07:58:05 mc Exp $
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
// Copyright � 2007

#ifndef INCLUDED_MP4MOVIE_H
#define INCLUDED_MP4MOVIE_H

#include "MP4OSMacros.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int MP4Err;

enum
{
	/* JLF 12/00 : support for OD, returned by MP4GetInline... and MP4GetProfiles... */
	MP4HasRootOD              = 2,
	MP4EOF                    = 1,
	MP4NoErr                  = 0,
	MP4FileNotFoundErr        = -1,
	MP4BadParamErr            = -2,
	MP4NoMemoryErr            = -3,
	MP4IOErr                  = -4,
	MP4NoLargeAtomSupportErr  = -5,
	MP4BadDataErr             = -6,
	MP4VersionNotSupportedErr = -7,
	MP4InvalidMediaErr        = -8,
	MP4DataEntryTypeNotSupportedErr = -100,
	MP4NoQTAtomErr                  = -500,
	MP4NotImplementedErr            = -1000
};


enum
{
	MP4OpenMovieNormal  = 0,
	MP4OpenMovieDebug   = (1 << 0)
};

enum
{
	MP4NewTrackIsVisual  = (1 << 1),
	MP4NewTrackIsAudio   = (1 << 2),
	MP4NewTrackIsPrivate = (1 << 8)
};

enum
{
	MP4ObjectDescriptorHandlerType         = MP4_FOUR_CHAR_CODE( 'o', 'd', 's', 'm' ),
	MP4ClockReferenceHandlerType           = MP4_FOUR_CHAR_CODE( 'c', 'r', 's', 'm' ),
	MP4SceneDescriptionHandlerType         = MP4_FOUR_CHAR_CODE( 's', 'd', 's', 'm' ),
	MP4VisualHandlerType                   = MP4_FOUR_CHAR_CODE( 'v', 'i', 'd', 'e' ),
	MP4AudioHandlerType                    = MP4_FOUR_CHAR_CODE( 's', 'o', 'u', 'n' ),
	MP4MPEG7HandlerType                    = MP4_FOUR_CHAR_CODE( 'm', '7', 's', 'm' ),
	MP4OCIHandlerType                      = MP4_FOUR_CHAR_CODE( 'o', 'c', 's', 'm' ),
	MP4IPMPHandlerType                     = MP4_FOUR_CHAR_CODE( 'i', 'p', 's', 'm' ),
	MP4MPEGJHandlerType                    = MP4_FOUR_CHAR_CODE( 'm', 'j', 's', 'm' ),
	MP4HintHandlerType                     = MP4_FOUR_CHAR_CODE( 'h', 'i', 'n', 't' ),
	MP4BiMHandlerType                      = MP4_FOUR_CHAR_CODE( 'm', 'p', '7', 'b' )
};

#define GETMOOV( arg ) \
	MP4PrivateMovieRecordPtr moov; \
	MP4Err err; \
	err = MP4NoErr; \
	if ( arg == NULL ) \
		BAILWITHERROR( MP4BadParamErr ) \
	moov = (MP4PrivateMovieRecordPtr) arg

#define GETMOVIEATOM(arg) \
	MP4MovieAtomPtr movieAtom; \
	GETMOOV(arg); \
	movieAtom = (MP4MovieAtomPtr) moov->moovAtomPtr	

#define GETMOVIEHEADERATOM(arg) \
	MP4MovieHeaderAtomPtr movieHeaderAtom; \
	GETMOVIEATOM(arg); \
	movieHeaderAtom = (MP4MovieHeaderAtomPtr) movieAtom->mvhd	

#define GETIODATOM(arg) \
	MP4ObjectDescriptorAtomPtr iodAtom; \
	GETMOVIEATOM(arg); \
	iodAtom = (MP4ObjectDescriptorAtomPtr) movieAtom->iods;	

struct MP4MovieRecord
{
	void*	data;
};
typedef struct MP4MovieRecord MP4MovieRecord;
typedef MP4MovieRecord*	MP4Movie;

struct MP4TrackRecord
{
	void*	data;
};
typedef struct MP4TrackRecord MP4TrackRecord;
typedef MP4TrackRecord*	MP4Track;

struct MP4MediaRecord
{
	void*	data;
};
typedef struct MP4MediaRecord MP4MediaRecord;
typedef MP4MediaRecord*	MP4Media;

// PP MAF
struct MP4MetadataRecord
{
	void*	data;
};
typedef struct MP4MetadataRecord MP4MetadataRecord;
typedef MP4MetadataRecord*	MP4Metadata;

struct MP4TrackReaderRecord
{
	void *data;
};
typedef struct MP4TrackReaderRecord MP4TrackReaderRecord;
typedef MP4TrackReaderRecord* MP4TrackReader;

struct MP4UserDataRecord
{
	void *data;
};
typedef struct MP4UserDataRecord MP4UserDataRecord;
typedef MP4UserDataRecord* MP4UserData;

struct MP4SLConfigRecord
{
	void *data;
};
typedef struct MP4SLConfigRecord MP4SLConfigRecord;
typedef MP4SLConfigRecord* MP4SLConfig;

#ifdef PRAGMA_EXPORT
#pragma export on
#endif

/* MP4Handle related */

typedef char **MP4Handle;

MP4_EXTERN ( MP4Err )
MP4NewHandle( u32 handleSize, MP4Handle *outHandle );

MP4_EXTERN ( MP4Err )
MP4SetHandleSize( MP4Handle h, u32 handleSize );

MP4_EXTERN ( MP4Err )
MP4DisposeHandle( MP4Handle h );

MP4_EXTERN ( MP4Err )
MP4GetHandleSize( MP4Handle h, u32 *outHandleSize );

/* Movie related */

MP4_EXTERN( MP4Err )
MP4DisposeMovie( MP4Movie theMovie );

MP4_EXTERN ( MP4Err )
MP4GetMovieDuration( MP4Movie theMovie, u64* outDuration );

MP4_EXTERN ( MP4Err )
MP4GetMovieInitialObjectDescriptor( MP4Movie theMovie, MP4Handle outDescriptorH );

MP4_EXTERN ( MP4Err )
MP4GetMovieInitialObjectDescriptorUsingSLConfig( MP4Movie theMovie, MP4SLConfig slconfig, MP4Handle outDescriptorH );

MP4_EXTERN ( MP4Err )
MP4GetMovieIODInlineProfileFlag( MP4Movie theMovie, u8* outFlag );

MP4_EXTERN ( MP4Err )
MP4GetMovieProfilesAndLevels( MP4Movie theMovie, u8 *outOD, u8 *outScene, u8 *outAudio, u8 *outVisual, u8 *outGraphics );

MP4_EXTERN ( MP4Err )
MP4GetMovieTimeScale( MP4Movie theMovie, u32* outTimeScale );

MP4_EXTERN ( MP4Err )
MP4GetMovieTrack( MP4Movie theMovie, u32 trackID, MP4Track *outTrack );


MP4_EXTERN ( MP4Err )
MP4GetMovieUserData( MP4Movie theMovie, MP4UserData* outUserData );

MP4_EXTERN ( MP4Err )
MP4GetMovieMetadata( MP4Movie theMovie, MP4Metadata *outMetadata );

MP4_EXTERN ( MP4Err )
MP4NewMovie( MP4Movie *outMovie, u32 initialODID,
						   u8 OD_profileAndLevel, u8 scene_profileAndLevel,
                           u8 audio_profileAndLevel, u8 visual_profileAndLevel,
                           u8 graphics_profileAndLevel );

MP4_EXTERN ( MP4Err )
MP4NewMovieFromHandle( MP4Movie *outMovie, MP4Handle movieH, u32 newMovieFlags );

/*
MP4_EXTERN ( MP4Err )
MP4SetMovieInitialBIFSTrack( MP4Movie theMovie, MP4Track theBIFSTrack );

MP4_EXTERN ( MP4Err )
MP4SetMovieInitialODTrack( MP4Movie theMovie, MP4Track theODTrack );
*/

MP4_EXTERN ( MP4Err )
MP4SetMovieIODInlineProfileFlag( MP4Movie theMovie, u8 theFlag );

MP4_EXTERN ( MP4Err )
MP4SetMovieTimeScale( MP4Movie theMovie, u32 timeScale );


/* Dealing with Movie files */

MP4_EXTERN ( MP4Err )
MP4OpenMovieFile( MP4Movie		*theMovie,
				  const char    *movieURL,
				  int 			openMovieFlags );
				  
MP4_EXTERN ( MP4Err )
MP4PutMovieIntoHandle( MP4Movie theMovie, MP4Handle movieH );

MP4_EXTERN ( MP4Err )
MP4WriteMovieToFile( MP4Movie theMovie, const char *filename );


/* Track related */

enum
{
	MP4HintTrackReferenceType        = MP4_FOUR_CHAR_CODE( 'h', 'i', 'n', 't' ),
	MP4StreamDependencyReferenceType = MP4_FOUR_CHAR_CODE( 'd', 'p', 'n', 'd' ),
	MP4ODTrackReferenceType          = MP4_FOUR_CHAR_CODE( 'm', 'p', 'o', 'd' ),
	/* JLF 12/00: added "sync" type for OCR_ES_ID (was broken before) */
	MP4SyncTrackReferenceType        = MP4_FOUR_CHAR_CODE( 's', 'y', 'n', 'c' )
};

MP4_EXTERN ( MP4Err )
MP4AddTrackReference( MP4Track theTrack, MP4Track dependsOn, u32 referenceType, u32 *outReferenceIndex );

MP4_EXTERN ( MP4Err )
MP4AddTrackReferenceWithID( MP4Track theTrack, u32 dependsOnID, u32 dependencyType, u32 *outReferenceIndex );

MP4_EXTERN ( MP4Err )
MP4AddTrackToMovieIOD( MP4Track theTrack );

MP4_EXTERN ( MP4Err )
MP4GetMovieIndTrack( MP4Movie theMovie, u32 trackIndex, MP4Track *outTrack );

/*
MP4_EXTERN ( MP4Err )
MP4GetMovieInitialBIFSTrack( MP4Movie theMovie, MP4Track *outBIFSTrack );
*/

MP4_EXTERN ( MP4Err )
MP4GetMovieTrackCount( MP4Movie theMovie, u32* outTrackCount );

MP4_EXTERN ( MP4Err )
MP4GetTrackDuration( MP4Track theTrack, u64 *outDuration );

MP4_EXTERN ( MP4Err )
MP4GetTrackEnabled( MP4Track theTrack, u32 *outEnabled );

MP4_EXTERN ( MP4Err )
MP4GetTrackID( MP4Track theTrack, u32 *outTrackID );

MP4_EXTERN ( MP4Err )
MP4GetTrackMedia( MP4Track theTrack, MP4Media *outMedia );

MP4_EXTERN ( MP4Err )
MP4GetTrackMetadata( MP4Track theTrack, MP4Metadata *outMetadata );

MP4_EXTERN ( MP4Err )
MP4GetTrackDecoderConfig( MP4Track theTrack, u32 *streamType, u32 *objectTypeId);

MP4_EXTERN ( MP4Err )
MP4GetTrackMovie( MP4Track theTrack, MP4Movie *outMovie );

MP4_EXTERN ( MP4Err )
MP4GetTrackOffset( MP4Track track, u32 *outMovieOffsetTime );

MP4_EXTERN ( MP4Err )
MP4GetTrackReference( MP4Track theTrack, u32 referenceType, u32 referenceIndex, MP4Track *outReferencedTrack );

MP4_EXTERN ( MP4Err )
MP4GetTrackReferenceCount( MP4Track theTrack, u32 referenceType, u32 *outReferenceCount );

MP4_EXTERN ( MP4Err )
MP4GetTrackUserData( MP4Track theTrack, MP4UserData* outUserData );

MP4_EXTERN ( MP4Err )
MP4InsertMediaIntoTrack( MP4Track trak, s32 trackStartTime, u32 mediaStartTime, u64 segmentDuration, s32 mediaRate );

MP4_EXTERN ( MP4Err )
MP4NewMovieTrack( MP4Movie theMovie, u32 newTrackFlags, MP4Track *outTrack );

MP4_EXTERN ( MP4Err )
MP4NewMovieTrackWithID( MP4Movie theMovie, u32 newTrackFlags, u32 newTrackID, MP4Track *outTrack );

MP4_EXTERN ( MP4Err )
MP4NewTrackMedia( MP4Track theTrack, MP4Media *outMedia, u32 handlerType, u32 timeScale, MP4Handle dataReference );

// PP MAF ->
MP4_EXTERN ( MP4Err )
MP4NewMovieMetadata( MP4Movie theMovie, MP4Metadata *outMetadata, u32 handlerType, MP4Handle dataReference );

MP4_EXTERN ( MP4Err )
MP4NewTrackMetadata( MP4Track theTrack, MP4Metadata *outMetadata, u32 handlerType, MP4Handle dataReference );

MP4_EXTERN ( MP4Err )
MP4GetBiMMetadata( MP4Metadata theMetaData, MP4Handle dataH, u32*  dataSize);

MP4_EXTERN ( MP4Err )
MP4SetVisualSampleDimensions( MP4Track theTrack, u32 width, u32 height );

// -> PP MAF

MP4_EXTERN ( MP4Err )
MP4SetTrackEnabled( MP4Track theTrack, u32 enabled );

MP4_EXTERN ( MP4Err )
MP4SetTrackOffset( MP4Track track, u32 movieOffsetTime );

MP4_EXTERN ( MP4Err )
MP4TrackTimeToMediaTime( MP4Track theTrack, u64 inTrackTime, s64 *outMediaTime );


/* Media related */

MP4_EXTERN ( MP4Err )
MP4AddMediaDataReference( MP4Media theMedia, u32 *outReferenceIndex, MP4Handle urlHandle, MP4Handle urnHandle );

MP4_EXTERN ( MP4Err )
MP4AddMediaSampleReference( MP4Media media, u64 dataOffset, u32 sampleCount,
				    MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
				    MP4Handle decodingOffsetsH, MP4Handle syncSamplesH );

MP4_EXTERN ( MP4Err )
MP4AddMediaSamples( MP4Media media, MP4Handle sampleH, u32 sampleCount,
				    MP4Handle durationsH, MP4Handle sizesH, MP4Handle sampleEntryH,
				    MP4Handle decodingOffsetsH, MP4Handle syncSamplesH );

MP4_EXTERN ( MP4Err )
MP4BeginMediaEdits( MP4Media theMedia );

MP4_EXTERN ( MP4Err )
MP4CheckMediaDataReferences( MP4Media theMedia );

MP4_EXTERN ( MP4Err )
MP4EndMediaEdits( MP4Media theMedia );

MP4_EXTERN ( MP4Err )
MP4GetIndMediaSample( MP4Media theMedia,
                      u32 sampleNumber,
                      MP4Handle outSample,
                      u32 *outSize,
					  u64 *outDTS,
					  s32 *outCTSOffset,
					  u64 *outDuration,
					  u32 *outSampleFlags,
					  u32 *outSampleDescIndex
					);

MP4_EXTERN ( MP4Err )
MP4GetIndMediaSampleReference( MP4Media theMedia,
                      u32 sampleNumber,
                      u32 *outOffset,
                      u32 *outSize,
					  u32 *outDuration,
					  u32 *outSampleFlags,
					  u32 *outSampleDescIndex,
					  MP4Handle sampleDesc
					);

MP4_EXTERN ( MP4Err )
MP4GetMediaDataRefCount( MP4Media theMedia, u32 *outCount );

/* data reference attributes */
enum
{
	MP4DataRefSelfReferenceMask = (1<<0)
};

enum
{
	MP4URLDataReferenceType = MP4_FOUR_CHAR_CODE( 'u', 'r', 'l', ' ' ),
	MP4URNDataReferenceType = MP4_FOUR_CHAR_CODE( 'u', 'r', 'n', ' ' )
};

MP4_EXTERN ( MP4Err )
MP4GetMediaDataReference( MP4Media theMedia,
						  u32 index,
						  MP4Handle referenceURL,
						  MP4Handle referenceURN,
						  u32 *outReferenceType,
						  u32 *outReferenceAttributes );
						  
MP4_EXTERN ( MP4Err )
MP4GetMediaDuration( MP4Media theMedia, u64 *outDuration );

MP4_EXTERN ( MP4Err )
MP4GetMediaHandlerDescription( MP4Media theMedia, u32 *outType, MP4Handle *outName );

MP4_EXTERN ( MP4Err )
MP4GetMediaLanguage( MP4Media theMedia, char *outThreeCharCode );

/* flags for NextInterestingTime */
enum
{
	MP4NextTimeSearchForward = 0,
	MP4NextTimeSearchBackward = -1,
	MP4NextTimeMediaSample = (1 << 0),
	MP4NextTimeMediaEdit   = (1 << 1),
	MP4NextTimeTrackEdit   = (1 << 2),
	MP4NextTimeSyncSample  = (1 << 3),
	MP4NextTimeEdgeOK      = (1 << 4)
};

/* NB: This ignores any edit list present in the Media's Track */
MP4_EXTERN ( MP4Err )
MP4GetMediaNextInterestingTime( MP4Media theMedia,
							     u32 interestingTimeFlags,   /* eg: MP4NextTimeMediaSample */
							     u64 searchFromTime,		 /* in Media time scale */
							     u32 searchDirection,        /* eg: MP4NextTimeSearchForward */
							     u64 *outInterestingTime,    /* in Media time scale */
							     u64 *outInterestingDuration /* in Media's time coordinate system */
							  );

/* flags for GetMediaSample */
enum
{
	MP4MediaSampleNotSync = (1 << 0),
	MP4MediaSampleHasCTSOffset  = (1 << 1)
};

MP4_EXTERN ( MP4Err )
MP4GetMediaSample( MP4Media theMedia, MP4Handle outSample, u32 *outSize,
					u64 desiredDecodingTime, u64 *outDecodingTime, u64 *outCompositionTime, u64 *outDuration,
					MP4Handle outSampleDescription, u32 *outSampleDescriptionIndex,
					u32 *outSampleFlags );


MP4_EXTERN ( MP4Err )
MP4GetMediaSampleCount( MP4Media theMedia, u32 *outCount );

MP4_EXTERN ( MP4Err )
MP4GetMediaSampleDescription( MP4Media theMedia, u32 index, MP4Handle outDescriptionH, u32 *outDataReferenceIndex );

MP4_EXTERN ( MP4Err )
MP4GetMediaTimeScale( MP4Media theMedia, u32 *outTimeScale );

MP4_EXTERN ( MP4Err )
MP4GetMediaTrack( MP4Media theMedia, MP4Track *outTrack );

MP4_EXTERN ( MP4Err )
MP4MediaTimeToSampleNum( MP4Media theMedia, u64 mediaTime, u32 *outSampleNum, u64 *outSampleCTS, u64 *outSampleDTS, s32 *outSampleDuration );

MP4_EXTERN ( MP4Err )
MP4SampleNumToMediaTime( MP4Media theMedia, u32 sampleNum, u64 *outSampleCTS, u64 *outSampleDTS, s32 *outSampleDuration );

MP4_EXTERN ( MP4Err )
MP4SetMediaLanguage( MP4Media theMedia, char *threeCharCode );

/* Sync Layer media access routines */

MP4_EXTERN ( MP4Err )
MP4GetElementaryStreamPacket( MP4Media theMedia, MP4Handle outSample, u32 *outSize,
				   			  u32 sequenceNumber, u64 desiredTime, u64 *outActualTime, u64 *outDuration );

MP4_EXTERN ( MP4Err )
MP4GetMediaDecoderConfig( MP4Media theMedia, u32 sampleDescIndex, MP4Handle decoderConfigH );

MP4_EXTERN ( MP4Err )
MP4GetMediaDecoderInformation( MP4Media theMedia, 
					   u32 sampleDescIndex, 
					   u32 *outObjectType, 
					   u32 *outStreamType,
					   u32 *outBufferSize,
					   u32 *outUpstream,
					   u32 *outMaxBitrate,
					   u32 *outAvgBitrate,
					   MP4Handle specificInfoH );

MP4_EXTERN ( MP4Err )
MP4GetMediaDecoderType( MP4Media theMedia, 
					   u32 sampleDescIndex, 
					   u32 *outObjectType, 
					   u32 *outStreamType,
					   u32 *outBufferSize,
					   MP4Handle specificInfoH );

MP4_EXTERN ( MP4Err )
MP4NewSampleDescription(  MP4Track theTrack,
						  MP4Handle sampleDescriptionH,
						  u32 dataReferenceIndex,
					      u32 objectTypeIndication,
					      u32 streamType,
					      u32 decoderBufferSize,
					      u32 maxBitrate,
					      u32 avgBitrate,
					      MP4Handle decoderSpecificInfoH );

MP4_EXTERN ( MP4Err )
MP4NewSampleDescriptionWithOCRAssociation(  MP4Track theTrack,
                                            MP4Handle sampleDescriptionH,
                                            u32 dataReferenceIndex,
                                            u32 objectTypeIndication,
                                            u32 streamType,
                                            u32 decoderBufferSize,
                                            u32 maxBitrate,
                                            u32 avgBitrate,
                                            MP4Handle decoderSpecificInfoH,
                                            u32 theOCRESID );
                                                
/* TrackReader stuff */

MP4_EXTERN ( MP4Err )
MP4CreateTrackReader( MP4Track theTrack, MP4TrackReader *outReader );

MP4_EXTERN ( MP4Err )
MP4DisposeTrackReader( MP4TrackReader theReader );

MP4_EXTERN ( MP4Err )
MP4TrackReaderGetCurrentDecoderConfig( MP4TrackReader theReader, MP4Handle decoderConfigH );

MP4_EXTERN ( MP4Err )
MP4TrackReaderGetNextAccessUnit( MP4TrackReader theReader, MP4Handle outAccessUnit, u32 *outSize, u32 *outSampleFlags,
                                 u32 *outCTS, u32 *outDTS );

MP4_EXTERN ( MP4Err )
MP4TrackReaderGetNextAccessUnitWithDuration( MP4TrackReader theReader,
                                             MP4Handle outAccessUnit,
                                             u32 *outSize, u32 *outSampleFlags,
                                             u32 *outCTS, u32 *outDTS,
                                             u32 *outDuration );

MP4_EXTERN ( MP4Err )
MP4TrackReaderGetNextPacket( MP4TrackReader theReader, MP4Handle outPacket, u32 *outSize );

MP4_EXTERN ( MP4Err )
MP4TrackReaderSetSLConfig( MP4TrackReader theReader, MP4SLConfig slConfig );

/* User Data */

MP4_EXTERN ( MP4Err )
MP4AddUserData( MP4UserData theUserData, MP4Handle dataH, u32 userDataType, u32 *outIndex );

MP4_EXTERN ( MP4Err )
MP4GetIndUserDataType( MP4UserData theUserData, u32 typeIndex, u32 *outType );

MP4_EXTERN ( MP4Err )
MP4GetUserDataEntryCount( MP4UserData theUserData, u32 userDataType, u32 *outCount );

MP4_EXTERN ( MP4Err )
MP4GetUserDataItem( MP4UserData theUserData, MP4Handle dataH, u32 userDataType, u32 itemIndex );

MP4_EXTERN ( MP4Err )
MP4GetUserDataTypeCount( MP4UserData theUserData, u32 *outCount );

MP4_EXTERN ( MP4Err )
MP4NewUserData( MP4UserData *outUserData );


/* SLConfig */

typedef struct MP4SLConfigSettingsRecord
{
	u32 predefined;
	u32 useAccessUnitStartFlag;
	u32 useAccessUnitEndFlag;
	u32 useRandomAccessPointFlag;
	u32 useRandomAccessUnitsOnlyFlag;
	u32 usePaddingFlag;
	u32 useTimestampsFlag;
	u32 useIdleFlag;
	u32 durationFlag;
	u32 timestampResolution;
	u32 OCRResolution;
	u32 timestampLength;
	u32 OCRLength;
	u32 AULength;
	u32 instantBitrateLength;
	u32 degradationPriorityLength;
	u32 AUSeqNumLength;
	u32 packetSeqNumLength;
	u32 timeScale;
	u32 AUDuration;
	u32 CUDuration;
	u64	startDTS;
	u64 startCTS;
	u32 OCRESID;
} MP4SLConfigSettings, *MP4SLConfigSettingsPtr;

MP4_EXTERN ( MP4Err )
MP4NewSLConfig( MP4SLConfigSettingsPtr settings, MP4SLConfig *outSLConfig );

MP4_EXTERN ( MP4Err )
MP4GetSLConfigSettings( MP4SLConfig config, MP4SLConfigSettingsPtr outSettings  );

MP4_EXTERN ( MP4Err )
MP4SetSLConfigSettings( MP4SLConfig config, MP4SLConfigSettingsPtr settings  );

#ifndef INCLUDED_ISOMOVIE_H
#include "ISOMovies.h"
#endif


/* JLF 12/00: added support for URL and for exchange files */
MP4_EXTERN ( MP4Err )
MP4NewMovieExt( MP4Movie *outMovie, u32 initialODID,
						   u8 OD_profileAndLevel, u8 scene_profileAndLevel,
                           u8 audio_profileAndLevel, u8 visual_profileAndLevel,
                           u8 graphics_profileAndLevel,
						   char *url,
						   u8 IsExchangeFile);



/* JLF 12/00: added support for stream priority */
MP4_EXTERN ( MP4Err ) 
MP4SetSampleDescriptionPriority(MP4Handle sampleEntryH, u32 priority);

/* JLF 12/00: added support for descriptors in the ESD */
MP4_EXTERN ( MP4Err ) 
MP4AddDescToSampleDescription(MP4Handle sampleEntryH, MP4Handle descriptorH);

/* JLF 12/00: added support for descriptors in the OD/IOD */
MP4_EXTERN ( MP4Err ) 
MP4AddDescToMovieIOD(MP4Movie theMovie, MP4Handle descriptorH);

/* JLF 12/00: checking of a specific data entry. */
MP4_EXTERN ( MP4Err ) 
MP4CheckMediaDataRef( MP4Media theMedia, u32 dataEntryIndex);


#ifdef __cplusplus
}
#endif
#ifdef PRAGMA_EXPORT
#pragma export off
#endif

#endif
