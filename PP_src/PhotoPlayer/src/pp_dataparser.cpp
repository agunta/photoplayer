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

#include "stdafx.h"

//#include "../../ppbim/src/collectionbim.h"
#include <fstream>

bool parse_collection_bim(PhotoPlayerMAF *pPlayer, char *buf, int bufsize);


//int _tmain(int argc, _TCHAR* argv[])
/*int main(int argc, char* argv[])
{
	ISOErr err = ISONoErr;
	PhotoPlayerMAF *pplayer = NULL;
	ImageList *imgList = NULL;
	char *inputFile = NULL;
	char *collbim = NULL;

	if(argc < 2)
	{
		printf("Missing input file. Usage:\n%s pp_movie.mp4\n", argv[0]);
		//err = ISOFileNotFoundErr;
		goto bail;
	}
	else
	{
		inputFile = argv[1];
	}

	pplayer = new PhotoPlayerMAF();
	err = pplayer->OpenMovie(inputFile); if(err) goto bail;
	unsigned int bimsize = 0;
	collbim = pplayer->GetCollectionBiM(&bimsize);
	parse_collection_bim(pplayer, collbim, bimsize);

bail:
	if(pplayer)
	{
		delete pplayer;
	}
	if(imgList)
	{
		delete imgList;
	}
	if(collbim) delete collbim;

	if(err)
	{
		printf("Error: %d\n", err);
	}

	return err;
}*/

void print_collection(PhotoPlayerMAF *pPlayer, char *header, ContentCollectionType *pCollection)
{
	// name of the collection
	if(!pPlayer || !pCollection) return;
	char *name = NULL;
	int headLen = 0;
	int nameLen = 0;
	int totalLen = 0;
	if(header)
	{
		headLen = (int)strlen(header);
	}
	if(pCollection->m_pAttributes && pCollection->m_pAttributes->m_pName)
	{
		name = pCollection->m_pAttributes->m_pName->GetUTF8Data();
		nameLen = (int)strlen(name);
	}
	if(nameLen == 0)
	{
		name = "<unnamed>";
		nameLen = 9;
	}
	totalLen = headLen + 1 + nameLen;
	char *nameBuf = new char[totalLen + 1];
	if(!nameBuf) return;
	int pos = 0;
	if(header && headLen >0)
	{
		pos = sprintf( nameBuf, "%s/", header);
	}
	sprintf(nameBuf + pos, "%s", name);
	printf("\nCollection: %s\n", nameBuf);
	int refCount = pCollection->m_nReferenceCount;
	if(refCount > 0 && pCollection->m_ppReferences)
	{
		for(int i = 0; i < refCount; i++)
		{
			ReferenceType *ref = pCollection->m_ppReferences[i];
			if(!ref) continue;
			ISOTrack tempTrack;
			ISOErr trackerr = pPlayer->GetTrackWithId(ref->m_nImgId, &tempTrack);
			if(trackerr)
			{
				printf("  Track %d not found\n", ref->m_nImgId); 
				continue;
			}
			ItemMpeg7 *itemmpeg = NULL;
			unsigned int size = 0;
			char *bim = pPlayer->GetTrackBiM(ref->m_nImgId, &size);
			if(bim && size >0)
			{
				if(size > 0)
				{
					itemmpeg = get_item_bim(bim, size);
				}
				delete bim;
			}
			bool bExternal = false;
			wchar_t *pExtResource = NULL;
			if(itemmpeg)
			{
				bExternal = ImageList::IsExternalResource(itemmpeg, &pExtResource);
			}
			else
			{
				printf(" Resource metadata not found, track: %d\n", ref->m_nImgId);
				continue;
			}
			if(bExternal)
			{
				printf("  Track %d (external): ", ref->m_nImgId); 
				if(pExtResource)
				{
					wprintf(L"%s", pExtResource);
				}
				printf("\n");
			}
			else
			{
				printf("  Track %d (internal)\n", ref->m_nImgId); 
			}
			if(itemmpeg) delete itemmpeg;
		}
	}
	int collCount = pCollection->m_nCollectionCount;
	if(collCount > 0 && pCollection->m_ppCollections)
	{
		for(int i = 0; i < collCount; i++)
		{
			ContentCollectionType *coll = pCollection->m_ppCollections[i];
			if(!coll) continue;
			print_collection(pPlayer, nameBuf, coll);
		}
	}

	if(nameBuf) delete nameBuf;
}

bool parse_collection_bim(PhotoPlayerMAF *pPlayer, char *buf, int bufsize)
{
	bool bresult = true;
	ColMpeg7 *mpegcol = new ColMpeg7();
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
			BimXMLUtil::ReportError("BiM reading failed");
			break;
		}
		if(mpegcol->m_pDescriptionUnit)
		{
			print_collection(pPlayer, NULL, mpegcol->m_pDescriptionUnit);
		}

	}while(false);

	if(mpegcol) delete mpegcol;
	if(reader) delete reader;

	return bresult;
}
