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

#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "../../xmvisual/src/ExtractionUtilities/ExtractionUtilities.h"
#include "../../ppbim/src/collectionbim.h"

bool item_bim_to_xml(char *buf, int bufsize, char *outFile);
ItemMpeg7 *get_item_bim(char *buf, int bufsize);
/*struct ImageItemDesc
{
	char *category;
	char *author;
	char *lastUpdate;
};
*/
class ImageItem
{
public:
	ImageItem();
	~ImageItem();
	unsigned int nId;
	ImageItem *next;
	char *name;
	unsigned char *bim;
	int bimlen;
	bool bExternalResource;
	int width;
	int height;
//	ImageItemDesc *desc;
};

// simple list
class ImageList
{
private:
	ColMpeg7 *m_pCollectionMeta;
	ImageItem *m_Head;
	ImageItem *m_Tail;
	int m_Count;

	bool InitXML();
	bool DeinitXML();
	bool LoadItemDesc(char *desc, unsigned char **bimbuf, int *bimlen, bool *bExternal, ImageIO *pImage);
	/*add at the end of list */
	bool AddFile(char *strid, char *filename, char *descfile);
	bool IsUniqueId(int id);

public:
	ImageList();
	~ImageList();


	int LoadImages(char *listfile);

	bool LoadCollectionDesc(char *desc, unsigned char **bimbuf, int *bimlen);

	int NumberOfElements();

	ImageItem *GetElement(int i);

	bool CheckImageList();

	// to do: move it to other place, e.g. BimXMLUtil
	static bool IsExternalResource(ItemMpeg7 *pItem, wchar_t **pResource);
};

#endif