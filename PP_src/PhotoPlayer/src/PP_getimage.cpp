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

// pp_getimage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <fstream>

ColMpeg7 *get_collection_bim(char *buf, int bufsize);

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	ISOErr err = ISONoErr;
	MP4TrackReader reader = NULL;
	ISOHandle sampleH = NULL;
	PhotoPlayerMAF *pplayer = NULL;
	ImageList *imgList = NULL;
	ColMpeg7 *colmpeg = NULL;
	//char *inputFile = NULL;
	bool bRetrieveAll = true;
	int iNumber = 0;

	//if(argc < 2)
	//{
	//	printf("Missing input file. Usage:\n%s input_pp_movie.mp4 [image_number]\n", argv[0]);
	//	//err = ISOFileNotFoundErr;
	//	goto bail;
	//}
	//else
	//{
	//	inputFile = argv[1];
	//}
	//if(argc > 2)
	//{
	//	iNumber = atoi(argv[2]);
	//	if(iNumber >=0)
	//	{
	//		bRetrieveAll = false;
	//	}

	//}

	

	pplayer = new PhotoPlayerMAF();
	err = pplayer->OpenMovie(inputFile); if (err) goto bail;

	int trackCount = pplayer->GetTrackCount();
	if(iNumber > trackCount)
	{
		printf("Track %d not found\n", iNumber);
		goto bail;
	}

	unsigned int bimsize = 0;
	char *collbim = pplayer->GetCollectionBiM(&bimsize);
	if(!collbim)
	{
		err = ISOBadDataErr;
		goto bail;
	}
	colmpeg = get_collection_bim(collbim, bimsize);
	if(collbim) delete collbim;

	if(!colmpeg) { err = ISOBadDataErr; goto bail; }
	ContentCollectionType *pCollection = colmpeg->m_pDescriptionUnit;
	if(!pCollection) { err = ISOBadDataErr; goto bail; }

	int refCount = pCollection->m_nReferenceCount; 
	if(refCount<=0 || (pCollection->m_ppReferences == NULL))
	{
		printf("Empty collection\n");
		goto bail;
	}

	int i;

	for(i = 1; i <= trackCount; i++)
	{

		unsigned int trackId = 0;
		ISOTrack track;
		err = pplayer->GetTrack(i, &track, &trackId); if (err) goto bail;

		if(!bRetrieveAll && iNumber != trackId)
		{
			continue;
		}
		// Check if the collection contains the trackId
		int j;
		bool bFound = false;
		for(j = 0; j < refCount; j++)
		{
			if(pCollection->m_ppReferences[j] && (pCollection->m_ppReferences[j]->m_nImgId == trackId)) bFound = true;
		}
		if(!bFound)
		{
			printf("Skipping track: %d\n", trackId);
			continue;
		}

		if(pplayer->CheckTrack(track))
		{
			printf("Invaild track (trackID = %d)\n", trackId);
			continue;
		}
		err = ISOCreateTrackReader( track, &reader ); if (err) goto bail;
		u32 unitSize;
		u32 cts;
		u32 dts;
		u32 sampleFlags;
		
		err = ISONewHandle( 0, &sampleH ); if (err) goto bail;	 
		err = MP4TrackReaderGetNextAccessUnit( reader, sampleH, &unitSize, &sampleFlags, &cts, &dts );
		if ( err )
		{
			if ( err == MP4EOF )
				err = MP4NoErr;
			break;
		}

		char outFileName[255];
		sprintf(outFileName, "%d.jpg", trackId);
		FILE *outFile = fopen(outFileName, "wb");
		if(outFile)
		{
			fwrite((char*)*sampleH, unitSize, 1, outFile);
			printf("Extracting image media: %s (%d bytes)\n", outFileName,  unitSize);
		}
		fclose(outFile);

		ISODisposeHandle(sampleH);
		sampleH = NULL;
		ISODisposeTrackReader( reader );
		reader = NULL;

		unsigned int bimsize;
		char *bimdata = pplayer->GetItemBiM(i, &bimsize);
		if(bimdata)
		{
			char outFileName[32];
			sprintf(outFileName, "%d.xml", trackId);
			if(item_bim_to_xml(bimdata, bimsize, outFileName))
			{
				printf("Extracting item metadata: %s\n", outFileName);
			}
			free(bimdata);
		}
	}

bail:
	if(err)
	{
		PrintError(err);
	}

	if(sampleH)
	{
		ISODisposeHandle( sampleH );
	}
	if(reader)
	{
		ISODisposeTrackReader( reader );
	}
	if(pplayer)
	{
		delete pplayer;
	}
	if(imgList)
	{
		delete imgList;
	}
	if(colmpeg) delete colmpeg;


	return err;
}

ColMpeg7 *get_collection_bim(char *buf, int bufsize)
{
	bool bresult = true;
	ColMpeg7 *mpegcol = new ColMpeg7();
	if(!mpegcol) return NULL;
	BitstreamReader *reader = NULL;

	do
	{
		reader = new BitstreamReader((unsigned char*)buf, bufsize);
		if(!buf || !mpegcol || !reader)
		{
			BimXMLUtil::ReportError("Out of memory");
			bresult = false;
			break;
		}
#ifdef BIM_BYTE_0X1F
		bresult = BimXMLUtil::ReadDecodingMode(reader);
#endif

		if(bresult)
		{
			bresult = mpegcol->ReadBitstream(reader);
		}
		if(!bresult)
		{
			BimXMLUtil::ReportError("Collection BiM reading failed");
			break;
		}

	}while(false);

	if(!bresult)
	{
		if(mpegcol) delete mpegcol;
		mpegcol = NULL;
	}

	if(reader) delete reader;

	return mpegcol;
}
