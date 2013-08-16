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

// PhotoPlayer.cpp
//

#include "stdafx.h"
#include "PhotoPlayer.h"
#include <stdio.h>

void PrintError(int err)
{
	char errbuf[256];
	sprintf(errbuf, "Error: ");
	switch(err)
	{
	case MP4FileNotFoundErr:
		sprintf(errbuf+7, "file not found.\n");
		break;
	case MP4BadDataErr:
		sprintf(errbuf+7, "bad data.\n");
		break;
	case MP4BadParamErr:
		sprintf(errbuf+7, "bad parameter.\n");
		break;
	case MP4InvalidMediaErr:
		sprintf(errbuf+7, "invalid media.\n");
		break;
	case MP4IOErr:
		sprintf(errbuf+7, "input/output.\n");
		break;
	case MP4NoMemoryErr:
		sprintf(errbuf+7, "no memory.\n");
		break;
	default:
		sprintf(errbuf + 7, "%d\n", err);
	};
	printf("%s", errbuf);
}


PhotoPlayerMAF::PhotoPlayerMAF()
{
	m_pFileName = NULL;
	m_moov = NULL;
}

PhotoPlayerMAF::~PhotoPlayerMAF()
{
	if(m_pFileName)
	{
		delete(m_pFileName);
	}
	ISOErr err;
	if(m_moov)
	{
		err = ISODisposeMovie( m_moov );
	}
}

ISOErr PhotoPlayerMAF::OpenMovie(char *fileName)
{
	ISOErr err;
	u32 count = 0;
	u32 *list = NULL;
	
	err = ISONoErr;
	err = ISOOpenMovieFile( &m_moov, fileName, MP4OpenMovieNormal ); if (err) goto bail; 
	err = MP4GetCompatibilityList(m_moov, &count, &list); if(err) goto bail;
	bool bComp = false;
	u32 ftype = MP4_FOUR_CHAR_CODE('l', 'P', 'P','M');
	//u32 ftype = MP4_FOUR_CHAR_CODE(' M', 'P', 'P', 'l');
	for(int i = 0; (i < (int)count) && (list !=NULL); i++)
	{
		if(*(list + i) == ftype)
		{
			bComp = true;
		}
	}
	if(!bComp)
	{
		printf("Error: compatible file format (MPPl) not found.\n");
		err = ISOBadDataErr;
	}

bail:

	if(err)
	{
		ISODisposeMovie( m_moov ); 
		m_moov = NULL;
	}
	return err;
}

int PhotoPlayerMAF::GetTrackCount()
{
	if(!m_moov)
	{
		return 0;
	}
	unsigned int trackCount = 0;
	ISOErr err = ISOGetMovieTrackCount( m_moov, &trackCount );
	if(err) return 0;
	return trackCount;
}

ISOErr PhotoPlayerMAF::GetTrack(int num, ISOTrack *outTrack, unsigned int *trackId)
{
	ISOErr err = ISONoErr;
	if(!m_moov)
	{
		err = ISOBadDataErr;
		goto bail;
	}
	err = ISOGetMovieIndTrack(m_moov, num, outTrack); if(err) goto bail;
	err = ISOGetTrackID(*outTrack, trackId); if(err) goto bail;
bail:
	if(err)
	{
		outTrack = NULL;
		trackId = NULL;
	}
	return err;
}

ISOErr PhotoPlayerMAF::GetTrackWithId(unsigned int trackId, ISOTrack *outTrack)
{
	ISOErr err = ISONoErr;
	if(!m_moov)
	{
		err = ISOBadDataErr;
		goto bail;
	}
	err = ISOGetMovieTrack(m_moov, trackId, outTrack); if(err) goto bail;
bail:
	if(err)
	{
		outTrack = NULL;
	}
	return err;
}


ISOErr PhotoPlayerMAF::CheckTrack(ISOTrack theTrack)
{
	MP4Metadata meta;
	ISOErr err = ISONoErr;
	if(!theTrack)
	{
		err = ISOBadDataErr;
		goto bail;
	}
	err = ISOGetTrackMetadata(theTrack, &meta); if(err)	goto bail;
	u32 streamType = 0;
	u32 objectTypeID = 0;
	err = ISOGetTrackDecoderConfig(theTrack, &streamType, &objectTypeID);
	if(err) goto bail;
	if(streamType != MP4SDVisualStreamType || objectTypeID != MP4SDJpegObjectTypeId)
	{
		err = ISOBadDataErr;
	}
	
bail:
	return err;
}


char *PhotoPlayerMAF::GetItemBiM(int trackNum, unsigned int *outSize)
{
	ISOErr err;
	ISOMetadata meta;
	MP4Handle dataH;
	char *retData = NULL;
	*outSize = 0;
	unsigned int len = 0;
	if(!m_moov)
	{
		return NULL;
	}
	ISONewHandle(0, &dataH);
	ISOTrack theTrack;
	unsigned int trackId;
	err = GetTrack(trackNum, &theTrack, &trackId);
	if(err || theTrack == NULL)
	{
		goto bail;
	}
	err = ISOGetTrackMetadata(theTrack, &meta); if(err) goto bail;
	err = MP4GetBiMMetadata( meta, dataH, &len ); if(err) goto bail;
	if(len <=0)
	{
		err = ISOBadDataErr;
		goto bail;
	}
	retData = new char[len];
	if(!retData)
	{
		err = ISONoMemoryErr;
		goto bail;
	}
	memcpy(retData, (char*)*dataH, len);
	*outSize = len;

bail:
	ISODisposeHandle(dataH);
	return retData;
}

char *PhotoPlayerMAF::GetTrackBiM(unsigned int trackId, unsigned int *outSize)
{
	ISOErr err;
	ISOMetadata meta;
	MP4Handle dataH;
	char *retData = NULL;
	*outSize = 0;
	unsigned int len = 0;
	if(!m_moov)
	{
		return NULL;
	}
	ISONewHandle(0, &dataH);
	ISOTrack theTrack;
	unsigned int tId;
	bool bFoundTrack = false;
	for(int i = 1; i <= GetTrackCount(); i++)
	{
		err = GetTrack(i, &theTrack, &tId); if(err) goto bail;
		if(tId == trackId)
		{
			bFoundTrack = true;
			break;
		}
	}
	if(!bFoundTrack || err || theTrack == NULL)
	{
		goto bail;
	}
	err = ISOGetTrackMetadata(theTrack, &meta); if(err) goto bail;
	err = MP4GetBiMMetadata( meta, dataH, &len ); if(err) goto bail;
	if(len <=0)
	{
		err = ISOBadDataErr;
		goto bail;
	}
	retData = new char[len];
	if(!retData)
	{
		err = ISONoMemoryErr;
		goto bail;
	}
	memcpy(retData, (char*)*dataH, len);
	*outSize = len;

bail:
	ISODisposeHandle(dataH);
	return retData;
}


char *PhotoPlayerMAF::GetCollectionBiM(unsigned int *outSize)
{
	ISOErr err;
	ISOMetadata meta;
	MP4Handle dataH;
	char *retData = NULL;
	*outSize = 0;
	unsigned int len = 0;
	if(!m_moov)
	{
		return NULL;
	}
	err = ISOGetMovieMetadata(m_moov, &meta); if(err) goto bail;
	err = ISONewHandle(0, &dataH); if(err) goto bail;
	err = MP4GetBiMMetadata( meta, dataH, &len ); if(err) goto bail;
	if(len <=0)
	{
		err = ISOBadDataErr;
		goto bail;
	}
	retData = new char[len];
	if(!retData)
	{
		err = ISONoMemoryErr;
		goto bail;
	}
	memcpy(retData, (char*)*dataH, len);
	*outSize = len;

bail:
	ISODisposeHandle(dataH);
	return retData;
}

