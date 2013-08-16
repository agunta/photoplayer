////////////////////////////////////////////////////////////////////////
//// This software module was originally developed by
//// 
//// Mitsubishi Electric ITE B.V. Visual Information Laboratory
//// ('VIL')
//// 
//// in the course of development of the
//// 
//// MPEG-A Photo-Player Multimedia Application Format 
//// ('Photo-Player MAF').
//// 
//// This software module is an implementation of a part of one or more
//// Photo-Player MAF tools as specified by ISO/IEC 23000-3. ISO/IEC gives
//// users of Photo-Player MAF free license to this software module or
//// modifications thereof for use in hardware or software products claiming
//// conformance to Photo-Player MAF. Those intending to use this software
//// module in hardware or software products are advised that its use may
//// infringe existing patents. The original developer of this software
//// module and his/her company, the subsequent editors and their companies,
//// and ISO/IEC have no liability for use of this software module or
//// modifications thereof in an implementation.
//// 
//// Copyright is not released for non Photo-Player MAF conforming products.
//// VIL retains full right to use the code for its own purpose, assign or
//// donate the code to a third party, and to inhibit third parties from
//// using the code for non Photo-Player MAF conforming products.
//// 
//// To the fullest extent allowed by applicable law, this software program
//// is issued "as is". Any warranties, conditions or representations,
//// expressed or implied, statutory or otherwise, are hereby excluded. Such
//// exclusions shall include without limitation all implied warranties of
//// satisfactory quality or fitness for any particular purpose and non-
//// infringement of third party rights. In no event shall any user of this
//// software program have any claim of any nature whatsoever against VIL or
//// any third party supplier to VIL regarding this software. No warranties
//// express or implied are made by VIL or any such third party supplier.
//// All limitations and exclusions of liability apply equally for the
//// benefit of VIL and all such suppliers. VIL does not warrant that the
//// software is error-free or that it will operate without interruption.
//// 
//// This copyright notice must be included in all copies or derivative
//// works.
//// 
//// Copyright © 2007
//
//#include "stdafx.h"
//
//
////int _tmain(int argc, _TCHAR* argv[])
//int main(int argc, char* argv[])
//{
//
//	ISOErr err = ISONoErr;
//	PPBuilderMAF *ppbuilder = NULL;
//	ImageList *imgList = NULL;
//	int totalCount = 0;
//
//	unsigned char *coll_buf = NULL;
//	int coll_buf_len = 0;
//
//	char *listFile;
//	char *outFile;
//	if(argc < 4)
//	{
//		printf("Missing parameters. Usage:\n%s collection_metadata_file image_list_file pp_movie.mp4\n", argv[0]);
//		//err = ISOBadParamErr;
//		goto bail;
//	}
//	outFile = argv[3];
//
//	ppbuilder = new PPBuilderMAF(outFile);
//	if(!ppbuilder) 
//	{
//		err = ISONoMemoryErr;
//		goto bail;
//	}
//	imgList = new ImageList();
//	if(!imgList) { err = ISONoMemoryErr; goto bail; }
//
//	if(!imgList->LoadCollectionDesc(argv[1], &coll_buf, &coll_buf_len))
//	{
//		err = ISOBadDataErr;
//		goto bail;
//	}
//
//	listFile = argv[2];
//	int imageNum = imgList->LoadImages(listFile);
//	if(imageNum <= 0){
//		err = ISOIOErr;
//		goto bail;
//	}
//	if(!imgList->CheckImageList())
//	{
//		BimXMLUtil::ReportError("Invalid image list");
//		err = ISOBadDataErr;
//		goto bail;
//	}
//
//	err = ppbuilder->InitMovie(); if(err) goto bail;
//
//
//	if(coll_buf && coll_buf_len > 0)
//	{
//		err = ppbuilder->AddCollectionMetadata(coll_buf, coll_buf_len);
//	}
//
//	for(int i = 0; i < imgList->NumberOfElements(); i++)
//	{
//		ImageItem *item = imgList->GetElement(i);
//		if(!item)
//		{
//			err = ISOBadDataErr;
//			goto bail;
//		}
//		err = ppbuilder->AddImageItem(item); if(err) goto bail;
//		totalCount++;
//	}
//	err = ppbuilder->WriteMovie();
//
//bail:
//	if(imgList)
//	{
//		delete imgList;
//	}
//	if(ppbuilder)
//	{
//		delete ppbuilder;
//	}
//	if(coll_buf)
//	{
//		delete coll_buf;
//	}
//
//	if(err)
//	{
//		printf("Error: %d.\n", err);
//	}
//	else if(totalCount > 0)
//	{
//		printf("MP4 item has been build succesfully (count = %d).\n", totalCount);
//	}
//
//	return err;
//}
