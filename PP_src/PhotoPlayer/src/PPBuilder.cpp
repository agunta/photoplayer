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

// PPBuilder.cpp
//

#include "stdafx.h"


PPBuilderMAF::PPBuilderMAF()
{
	m_pFileName = NULL;
	m_moov = NULL;
}

PPBuilderMAF::PPBuilderMAF(char *outfile)
{
	m_pFileName = NULL;
	m_moov = NULL;
	if(outfile)
	{
		size_t len = strlen(outfile);
		if(len > 0 && len < 255)
		{
			m_pFileName = new char[len+1];
			strcpy(m_pFileName, outfile);
		}
	}
}

PPBuilderMAF::~PPBuilderMAF()
{
	if(m_pFileName)
	{
		delete(m_pFileName);
	}
	ISOErr err;
	if(m_moov)
	{
		err = ISODisposeMovie( m_moov ); //if (err) goto bail;
	}
	
  //bail:;
}

ISOErr PPBuilderMAF::InitMovie()
{
	ISOErr err;
	//ISOMovie moov;
	//char *filename = "test_movie.mp4";

	err = ISONoErr;
	err = MPPMAFNewMovie( &m_moov ); if (err) goto bail;

bail:
	return err; 
}

ISOErr PPBuilderMAF::AddCollectionMetadata(unsigned char *bimbuf, int buflen)
{
	ISOErr err = ISONoErr;
	if(bimbuf == NULL || buflen <= 0)
	{
		err = ISOBadParamErr;
		goto bail;
	}
	if(m_moov == NULL)
	{
		err = ISOBadDataErr;
		goto bail;
	}

	ISOMetadata meta; 
	ISOHandle metadataH = NULL;

	err = ISONewHandle(buflen, &metadataH); if (err) goto bail;
	memcpy((char*)(*metadataH), bimbuf, buflen);
	err = ISONewMovieMetadata( m_moov, &meta, MP4BiMHandlerType, metadataH ); if (err) goto bail;

bail:
	if(metadataH)
	{
		ISODisposeHandle( metadataH );
	}
	return err; 
}

ISOErr PPBuilderMAF::LoadMovie(char *filename)
{
	// not implemented
	return ISONotImplementedErr;
}

ISOErr  PPBuilderMAF::GetImage(ISOHandle sampleH, char *imagename)
{
	if(!imagename) return ISOBadParamErr;
	ISOErr err;
	u32 imageLength;
	u32 frameCount = 1;
	
	err = ISONoErr;
	imageLength = getJPEGFileSize(imagename);
	if(imageLength <=0) return ISOFileNotFoundErr;
	char *buf = getJPEGFile(imagename, imageLength);
	if(!buf) { err = ISONoMemoryErr; goto bail; }
	err = ISOSetHandleSize( sampleH, imageLength ); if (err) goto bail;
	memcpy((char*)(*sampleH), buf, imageLength);

bail:
	if(buf) {
		free(buf);
	}
	return err; 
	//return ISONotImplementedErr;
}

ISOErr PPBuilderMAF::AddImageItem(ImageItem *item)
{
	ISOErr err;
	if(item == NULL || item->name == NULL)
	{
		err = ISOBadParamErr;
		goto bail;
	}
	if(m_moov == NULL)
	{
		err = ISOBadDataErr;
		goto bail;
	}

	char *imagefile = item->name;
	u32 nTrackId = item->nId;
	u32 timeScale = 1;

	ISOTrack trak;
	ISOMedia media; 
	ISOMetadata meta; 
	ISOHandle sampleEntryH;
	ISOHandle sampleDataH;
	ISOHandle sampleDurationH;
	ISOHandle sampleSizeH;
	ISOHandle decoderSpecificInfoH;
	ISOHandle metadataH = NULL;
 	u32 objectTypeIndication;
	u32 streamType;
	u32 decoderBufferSize;
	u32 maxBitrate;
	u32 avgBitrate;
	
	err = ISONoErr;
	err = ISONewMovieTrackWithID( m_moov, MP4NewTrackIsVisual /*MP4NewTrackIsPrivate*/, nTrackId, &trak ); if (err) goto bail;
	err = ISONewTrackMedia( trak, &media, MP4VisualHandlerType, timeScale, NULL ); if (err) goto bail;
	err = ISOBeginMediaEdits( media ); if (err) goto bail;

	err = ISOSetMediaLanguage( media, "und" ); if (err) goto bail;

	u32 nDuration = 5 * timeScale;
	err = ISONewHandle( 0, &sampleDataH ); if (err) goto bail;
	err = ISONewHandle( sizeof(u32), &sampleSizeH ); if (err) goto bail;
	err = GetImage( sampleDataH, imagefile );
	if (err)
	{
		if(!item->bExternalResource)	goto bail;
	}
	err = ISONewHandle( sizeof(u32), &sampleDurationH ); if (err) goto bail;
	*((u32*) *sampleDurationH) = nDuration; // 5 seconds ? //0xFFFFFFFF;
	
	err = ISOGetHandleSize( sampleDataH, (u32*) *sampleSizeH ); if (err) goto bail;
	decoderBufferSize = *((u32*) *sampleSizeH);

	streamType = MP4SDVisualStreamType; // VisualStream (see ISO/IEC 14496-1 - 8.6.5 DecoderConfigDescriptor)
	objectTypeIndication = MP4SDJpegObjectTypeId; // Visual ISO/IEC 10918-1 (JPEG)
	maxBitrate = (u32)((8 * decoderBufferSize  * timeScale)/nDuration + 0.9999f);  // ?
	avgBitrate = maxBitrate;  // ?
	decoderSpecificInfoH = NULL;
	err = ISONewHandle( 0, &sampleEntryH ); if (err) goto bail;
	err = MP4NewSampleDescription( trak, sampleEntryH,
								   1,
					     		   objectTypeIndication,
					               streamType,
					               decoderBufferSize,
					               maxBitrate,
					               avgBitrate,
					               decoderSpecificInfoH ); if (err) goto bail;


	err = ISOAddMediaSamples( media, sampleDataH, 1, sampleDurationH, sampleSizeH, sampleEntryH, NULL, NULL ); if (err) goto bail;
	err = ISOEndMediaEdits( media ); if (err) goto bail;

	MJ2SetTrackDimensions(trak, (u32) item->width << 16, (u32)item->height << 16);
	MP4SetVisualSampleDimensions(trak, item->width, item->height);

	if ( sampleEntryH )
	{
		err = ISODisposeHandle( sampleEntryH ); if (err) goto bail;
		sampleEntryH = NULL;
	}	
	if(!item->bim) { err = ISOBadDataErr; goto bail; }
	int metaLen = item->bimlen;
	if(metaLen > 0)
	{
		err = ISONewHandle(metaLen, &metadataH); if (err) goto bail;
		memcpy((char*)(*metadataH), item->bim, metaLen);
		err = ISONewTrackMetadata( trak, &meta, MP4BiMHandlerType, metadataH ); if (err) goto bail;
	}
	else
	{
		err = ISOBadDataErr;
		goto bail;
	}
bail:
	if(metadataH)
	{
		ISODisposeHandle( metadataH );
	}

	if ( sampleDataH )
		ISODisposeHandle( sampleDataH );
	
	if ( sampleDurationH )
		ISODisposeHandle( sampleDurationH );
	
	if ( sampleSizeH )
		ISODisposeHandle( sampleSizeH );
	
	if ( decoderSpecificInfoH )
		ISODisposeHandle( decoderSpecificInfoH );

	return err; 
}



ISOErr PPBuilderMAF::AddImages(char *imagelist)
{
	return ISONotImplementedErr;
}

ISOErr PPBuilderMAF::WriteMovie(char *outfile)
{
	ISOErr err;

	char *filename = outfile;
	if(!filename) filename = m_pFileName;
	if(filename == NULL)
	{
		err = -1;
		goto bail;
	}
	u64 duration = 0;
	MP4GetMovieDuration(m_moov, &duration);

	err = MPPMAFWriteMovieToFile( m_moov, filename ); if (err) goto bail;
bail:
	return err; 
}
