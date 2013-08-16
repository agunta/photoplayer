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
#include "ImageList.h"
#include <fstream>

//#include "../../ppbim/src/collectionbim.h"

//#include <xercesc/parsers/XercesDOMParser.hpp>
#include "XercesDOMParser.hpp"
#include <xercesc/util/OutOfMemoryException.hpp>
#include "ppdomerrorhandler.h"

static char*                    gXmlFile               = 0;
static bool                     gDoNamespaces          = true;
static bool                     gDoSchema              = true;
static bool                     gSchemaFullChecking    = false;
static bool                     gDoCreate              = false;

//static XercesDOMParser::ValSchemes    gValScheme       = XercesDOMParser::Val_Auto;
static XercesDOMParser::ValSchemes    gValScheme       = XercesDOMParser::Val_Always;


bool CheckReference(ContentCollectionType *pCollection, ImageList *pImageList);

/** Computes the Dominant Color description and inserts it into item tree
  *@param bForce if the descrription is present the new one is computed and inserted
  * if bForce is false the dominant color description is computed only if the item does not contain
  * such description.
  */
bool SetDominantColorDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce);

/** Computes the Color Layout description and inserts it into item tree
  *@param bForce if the descrription is present the new one is computed and inserted
  * if bForce is false the description is computed only if the item does not contain
  * such description.
  */
bool SetColorLayoutDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce);

/** Computes the Color Structure description and inserts it into item tree
  *@param bForce if the descrription is present the new one is computed and inserted
  * if bForce is false the description is computed only if the item does not contain
  * such description.
  */
bool SetColorStructureDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce);

/** Computes the Scalable Color description and inserts it into item tree
  *@param bForce if the descrription is present the new one is computed and inserted
  * if bForce is false the description is computed only if the item
  * does not contain such description.
  */
bool SetScalableColorDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce);

/** Computes the Edge Histogram description and inserts it into item tree
  *@param bForce if the descrription is present the new one is computed and inserted
  * if bForce is false the description is computed only if the item
  * does not contain such description.
  */
bool SetEdgeHistogramDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce);

/** Computes the Homogeneous Texture description and inserts it into item tree
  *@param bForce if the descrription is present the new one is computed and inserted
  * if bForce is false the description is computed only if the item
  * does not contain such description.
  */
bool SetHomogeneousTextureDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce);

ImageItem::ImageItem()
{
	name = NULL;
	bim = NULL;
}
ImageItem::~ImageItem()
{
	if(name) delete name;
	if(bim) delete bim;
	/*if(desc != NULL)
	{
		if(desc->category) delete desc->category;
		if(desc->author) delete desc->author;
		if(desc->lastUpdate) delete desc->lastUpdate;
		delete desc;
	}*/
}


ImageList::ImageList()
{
	m_Head = NULL;
	m_Tail = NULL;
	m_Count = 0;
	m_pCollectionMeta = NULL;
}

ImageList::~ImageList()
{
	ImageItem *item = m_Head;
	for(int i = 0; i < m_Count; i++)
	{
		if(!item) break;
		ImageItem *nextitem = item->next;
		if(item)
		{
			delete item;
		}
		item = nextitem;
	}
	if(m_pCollectionMeta) delete m_pCollectionMeta;
}


#define IIDMAX 256
#define INAMEMAX 255
#define IMETAMAX 255

int ImageList::LoadImages(char *imagelist )
{
	int ret_num = 0;
	FILE * file = fopen(imagelist, "rb");
	char buf_id[IMETAMAX];
	char buf_name[INAMEMAX];
	char buf_meta[IMETAMAX];
    if(!file) return false;
	bool bEnd = false;
	InitXML();

	while(!bEnd)
	{
		int itemlen = 0;
		bool bAdd = false;
		bool bFileName = false;
		bool bMeta = false;
		int iNameLen = 0;
		int iMetaLen = 0;
		int iIdLen = 0;
		for(int i = 0; i < (INAMEMAX + IMETAMAX + IIDMAX); i++)
		{
			int ch = fgetc(file);
			if(ch == EOF) 
			{
				bAdd = true;
				bEnd = true;
			}
			else if(ch == '\n' || ch == '\r')
			{
				bAdd = true;
			}
			else
			{
				//if(ch == ';' || ch == ':')
				if(ch == ';')
				{
					if(!bFileName)
					{
						bFileName = true;
					}
					else if(!bMeta)
					{
						bMeta = true;
					}
					else
					{
						bAdd = true;
					}
				}
				else if(bFileName && !bMeta)
				{
					if(iNameLen < INAMEMAX)
					{
						buf_name[iNameLen++] = (char)ch;
					}
					else
					{
						break;
					}
				}
				else if(bMeta)
				{
					if(iMetaLen < IMETAMAX)
					{
						buf_meta[iMetaLen++] = (char)ch;
					}
					else
					{
						break;
					}
				}
				else
				{
					if(iIdLen < IIDMAX)
					{
						buf_id[iIdLen++] = (char)ch;
					}
					else
					{
						break;
					}
				}
			}

			if(bAdd)
			{
				// image name can be empty
				if(iIdLen > 0 && iMetaLen > 0)
				{
					buf_id[iIdLen] = 0;
					buf_name[iNameLen] = 0;
					buf_meta[iMetaLen] = 0;
					if(!AddFile(buf_id, buf_name, buf_meta))
					{
						printf("Can not add item: %s\n", buf_id);
						//bEnd = true;
					}
					else
					{
						ret_num++;
					}
				}
				break;
			}
			if(bEnd) break;
		}
	}
	DeinitXML();
	return ret_num;
}

bool ImageList::AddFile(char *strid, char *filename, char *descfile)
{
	if(!strid || !filename || !descfile) return false;
	if((strlen(strid) <= 0) || (strlen(filename) <= 0) || (strlen(descfile) <= 0)) return false;
	if((strlen(strid) <= 0) || (strlen(descfile) <= 0)) return false;
	int nId = atoi(strid);
	if(!IsUniqueId(nId))
	{
		printf("Error: (repeated id: %s)\n", strid);
		return false;
	}

	ImageItem *item = NULL;
//	ImageItemDesc * myDesc = NULL;
	unsigned char *bimbuf = NULL;
	ImageIO *pImage = NULL;

	bool bOK = true;
	do
	{
		item = new ImageItem();
		if(!item)
		{
			bOK = false;
			break;
		}
		pImage = new ImageIO();
		if(pImage)
		{
			if(filename == NULL || !pImage->LoadImage(filename))
			{
				delete pImage;
				pImage = NULL;
			}
		}
		if(!pImage){ bOK = false; break; }

		int bimlen = 0;
		bool bExternal = false;
		bOK = LoadItemDesc(descfile, &bimbuf, &bimlen, &bExternal, pImage);
		if(bimlen <= 0) bOK = false;
		if(!bOK) break;

		item->nId = nId;
		item->name = _strdup(filename);
		item->bim = bimbuf;
		item->bimlen = bimlen;
		item->bExternalResource = bExternal;
		item->width = pImage->GetWidth();
		item->height = pImage->GetHeight();
		if(!item->bExternalResource && !pImage)
		{
			bOK = false;
			printf("Can not access internal resource: %d\n", item->nId);
			break;
		}

		item->next = NULL;
		if(m_Head == NULL)
		{
			m_Head = item;
		}
		else
		{
			m_Tail->next = item;
		}

		m_Tail = item;
		m_Count++;
	} while(false);

	if(pImage) delete pImage;
	if(!bOK)
	{
		if(item) delete item;
		if(bimbuf) delete bimbuf;

	}
	return bOK;
}

bool ImageList::LoadItemDesc(char *descfile, unsigned char **bimbuf, int *bimlen, bool *bExternal, ImageIO *pImage)
{
	if(!descfile || !bimbuf || !bimlen) return false;

	XercesDOMParser *parser = new XercesDOMParser();
    parser->setValidationScheme(gValScheme);
    parser->setDoNamespaces(gDoNamespaces);
    parser->setDoSchema(gDoSchema);
    parser->setValidationSchemaFullChecking(gSchemaFullChecking);
    parser->setCreateEntityReferenceNodes(gDoCreate);

    PPDOMErrorHandler errorHandler;
    parser->setErrorHandler(&errorHandler);

	bool bresult = true;
	try
    {
        parser->parse(descfile);
    }
    catch (const OutOfMemoryException&)
    {
		bresult = false;
	}
    catch (...)
    {
		bresult = false;
	}
	if(errorHandler.getSawErrors())
	{
		bresult = false;
	}

	unsigned char *buf = NULL;
	if(bresult)
	{
		ItemMpeg7 *mpegcol = new ItemMpeg7();
		do {
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *domdoc = parser->getDocument();
			if(!domdoc)
			{
				BimXMLUtil::ReportError("Missing or invalid XML document");
				bresult = false;
				break;
			}
			bresult = mpegcol->BuildFromXML((DOMNode*)domdoc->getDocumentElement());
			if(!bresult)
			{
				BimXMLUtil::ReportError("Building Mpeg7 Item failed");
				break;
			}
			bool bExt = IsExternalResource(mpegcol, NULL);
			*bExternal = bExt;
			if(!bExt && pImage && SetDominantColorDescription(mpegcol, pImage, true))
			{
				fprintf(stdout, "    + DominantColor\n");
			}
			if(!bExt && pImage && SetScalableColorDescription(mpegcol, pImage, true))
			{
				fprintf(stdout, "    + ScalableColor\n");
			}
			if(!bExt && pImage && SetColorStructureDescription(mpegcol, pImage, true))
			{
				fprintf(stdout, "    + ColorStructure\n");
			}
			if(!bExt && pImage && SetColorLayoutDescription(mpegcol, pImage, true))
			{
				fprintf(stdout, "    + ColorLayout\n");
			}
			if(!bExt && pImage && SetEdgeHistogramDescription(mpegcol, pImage, true))
			{
				fprintf(stdout, "    + EdgeHistogram\n");
			}
			if(!bExt && pImage && SetHomogeneousTextureDescription(mpegcol, pImage, true))
			{
				fprintf(stdout, "    + Homogeneous Texture\n");
			}
			if(bExt)
			{
				fprintf(stdout, "    External primary resource\n");
			}

			int bufsize = mpegcol->GetNumberOfBits();
			bufsize = (bufsize + 7)/8;
			if(bufsize <= 0)
			{
				BimXMLUtil::ReportError("Calculation of the BiM buffer size failed");
				bresult = false;
				break;
			}
#ifdef BIM_BYTE_0X1F
			bufsize+=1;
#endif
			buf = (unsigned char*)malloc(bufsize);
			if(!buf)
			{
				BimXMLUtil::ReportError("Out of memory error");
				bresult = false;
				break;
			}
			BitstreamWriter *writer = new BitstreamWriter(bufsize, buf);
#ifdef BIM_BYTE_0X1F
			bresult = BimXMLUtil::WriteDecodingMode(writer);
			if(!bresult)
			{
				BimXMLUtil::ReportError("Writing Item bim failed");
				delete writer;
				break;
			}
#endif

			bresult = mpegcol->WriteBitstream(writer);
			delete writer;
			if(!bresult)
			{
				BimXMLUtil::ReportError("Writing Item bim failed");
				break;
			}
			*bimbuf = buf;
			*bimlen = bufsize;
		}
		while(false);
		if(mpegcol) delete mpegcol;
	}
	if(!bresult)
	{
		if(buf) delete buf;
		*bimbuf = NULL;
		*bimlen = 0;
	}
	return bresult;
}

bool ImageList::LoadCollectionDesc(char *descfile, unsigned char **bimbuf, int *bimlen)
{
	if(!descfile || !bimbuf || !bimlen) return false;

	if(!InitXML()) return false;
	XercesDOMParser *parser = new XercesDOMParser();
    parser->setValidationScheme(gValScheme);
    parser->setDoNamespaces(gDoNamespaces);
    parser->setDoSchema(gDoSchema);
    parser->setValidationSchemaFullChecking(gSchemaFullChecking);
    parser->setCreateEntityReferenceNodes(gDoCreate);

    PPDOMErrorHandler errorHandler;
    parser->setErrorHandler(&errorHandler);

	bool bresult = true;
	try
    {
        parser->parse(descfile);
    }
    catch (const OutOfMemoryException&)
    {
		bresult = true;
	}
    catch (...)
    {
		bresult = true;
	}
	if(errorHandler.getSawErrors())
	{
		bresult = false;
	}

	unsigned char *buf = NULL;
	if(bresult)
	{
		m_pCollectionMeta = new ColMpeg7();
		do {
			XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *domdoc = parser->getDocument();
			if(!domdoc)
			{
				BimXMLUtil::ReportError("Missing or invalid XML document");
				bresult = false;
				break;
			}
			bresult = m_pCollectionMeta->BuildFromXML((DOMNode*)domdoc->getDocumentElement());
			if(!bresult)
			{
				BimXMLUtil::ReportError("Building Mpeg7 Collection failed");
				break;
			}

			int bufsize = m_pCollectionMeta->GetNumberOfBits();
			bufsize = (bufsize + 7)/8;
			if(bufsize <= 0)
			{
				BimXMLUtil::ReportError("Calculation of BiM buffer size failed");
				bresult = false;
				break;
			}
#ifdef BIM_BYTE_0X1F
			bufsize+=1;
#endif
			buf = new unsigned char[bufsize];
			if(!buf)
			{
				BimXMLUtil::ReportError("Out of memory error");
				bresult = false;
				break;
			}
			BitstreamWriter *writer = new BitstreamWriter(bufsize, buf);
#ifdef BIM_BYTE_0X1F
			bresult = BimXMLUtil::WriteDecodingMode(writer);
			if(!bresult)
			{
				BimXMLUtil::ReportError("Writing Collection bim failed");
				delete writer;
				break;
			}
#endif

			bresult = m_pCollectionMeta->WriteBitstream(writer);
			delete writer;
			if(!bresult)
			{
				BimXMLUtil::ReportError("Writing Collection bim failed");
				break;
			}
			*bimbuf = buf;
			*bimlen = bufsize;
		}
		while(false);
	}
	if(!bresult)
	{
		if(m_pCollectionMeta) delete m_pCollectionMeta;
		m_pCollectionMeta = NULL;
		if(buf) delete buf;
		*bimbuf = NULL;
		*bimlen = 0;
	}
	DeinitXML();
	return bresult;
}


int ImageList::NumberOfElements()
{
	return m_Count;
}


ImageItem *ImageList::GetElement(int num)
{
	if(num >= m_Count) return NULL;
	ImageItem *item = m_Head;
	for(int i = 0; i < num; i++)
	{
		item = item->next;
		if(item == NULL) break;
	}
	return item;
}


bool ImageList::IsUniqueId(int id)
{
	bool bUnique = true;
	ImageItem *item = m_Head;
	for(int i = 0; i < m_Count; i++)
	{
		if(!item) break;
		if(item->nId == id)
		{
			bUnique =false;
			break;
		}
		item = item->next;
	}
	return bUnique;
}

bool ImageList::InitXML()
{
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException&)
	{
		return false;
	}
	/*catch (const XMLException& toCatch)
	{
		return false;
	}*/
	return true;
}

bool ImageList::CheckImageList()
{
	bool bret = true;
	if(!m_pCollectionMeta
		|| !m_pCollectionMeta->m_pDescriptionUnit 
		|| !m_pCollectionMeta->m_pDescriptionUnit->m_ppReferences) bret = false;

	if(bret)
	{
		ContentCollectionType *pCollection = m_pCollectionMeta->m_pDescriptionUnit;
		bret = CheckReference(pCollection, this);
	}

	/*
	//to do: check subcollections
	int nColCount = m_pCollectionMeta->m_pDescriptionUnit->m_nCollectionCount;
	if(m_pCollectionMeta->m_pDescriptionUnit->m_ppCollections != NULL)
	{
		for(int i = 0; i < nColCount; i++)
	}
	*/
	return bret;
}

bool ImageList::DeinitXML()
{
	XMLPlatformUtils::Terminate();
	return true;
}

/** Checks if the metadata points to external jpeg resource
  * if pResource is not NULL and external resource exists pResource will point to the resource name.
  */
bool ImageList::IsExternalResource(ItemMpeg7 *pItem, wchar_t **pResource)
{
	if(!pItem || !pItem->m_pDescriptionUnit || !pItem->m_pDescriptionUnit->m_pImage
		|| !pItem->m_pDescriptionUnit->m_pImage->m_pChild
		|| !pItem->m_pDescriptionUnit->m_pImage->m_pChild->m_pMediaInfo
		|| !pItem->m_pDescriptionUnit->m_pImage->m_pChild->m_pMediaInfo->m_ppMediaProfile
		)
	{
		if(pResource) *pResource = NULL;
		return false;
	}
	MediaInformationType *pMediaInfo = pItem->m_pDescriptionUnit->m_pImage->m_pChild->m_pMediaInfo;
	int profileCount = pMediaInfo->m_nProfileCount;
	for(int i = 0; i < profileCount; i++)
	{
		MediaProfileType *pProfile = pMediaInfo->m_ppMediaProfile[i];
		if(!pProfile || !pProfile->m_pMediaFormat || !pProfile->m_pMediaFormat->m_pFileFormat)
		{
			continue;
		}
		if(!pProfile->m_pMediaFormat->m_pFileFormat->m_pTermDefinition
		|| !pProfile->m_pMediaFormat->m_pFileFormat->m_pTermDefinition->m_pName
		|| !pProfile->m_pMediaFormat->m_pFileFormat->m_pTermDefinition->m_pName->m_pValue)
		{
			BimXMLUtil::ReportWarning("<FileFormat> does not contain valid name");
			continue;
		}

		DescString *pData = pProfile->m_pMediaFormat->m_pFileFormat->m_pTermDefinition->m_pName->m_pValue;
		wchar_t *wsData = pData->GetData();
		//if(wsData && wcscmp(wsData, L"jpeg") == 0)
		if(wsData && BimXMLUtil::CompareStrings(wsData, L"jpeg")  == 0)
		{
			if(pProfile->m_pMediaInstance)
			{
				MediaLocatorType *pLocator = pProfile->m_pMediaInstance->m_pMediaLocator;
				if(!pLocator || !pLocator->m_pMediaURI || !pLocator->m_pMediaURI->m_pUri->m_pValue)
				{
					BimXMLUtil::ReportWarning("<MadiaInstance> do not have valid media URI");
				}
				else
				{
					// primary jpeg resource
					DescAnyURI *pURi = pProfile->m_pMediaInstance->m_pMediaLocator->m_pMediaURI->m_pUri;
					wchar_t *pURIData = pURi->GetURI();
					if(pURIData)
					{
						int buflen = (int)wcslen(pURIData);
						if(buflen > 0)
						{
							if(pResource != NULL)
							{
								*pResource = pURIData;
							}
						}
					}
				}
				//if <FileType> is jpeg, presence of <MediaInstance> indicates external primary resource
				return true;
			}
		}
	}
	if(pResource) *pResource = NULL;
	return false;
}

bool SetDominantColorDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce)
{
	if(!pImage || !pImage->GetImage()) return false;
	StillRegionFeatureType *pFeature = NULL;
	DominantColorType *pDC = NULL;
	DominantColorType *pOldDC = NULL;
	if(pItem)
	{
		if(pItem->m_pDescriptionUnit 
			&& pItem->m_pDescriptionUnit->m_pImage)
		{
			pFeature = pItem->m_pDescriptionUnit->m_pImage->m_pVisualDescriptionScheme;
			if(pFeature)
			{
				pDC = pFeature->m_pDominantColor;
			}
			else
			{
				pFeature = new StillRegionFeatureType();
			}
			if(!pFeature) return false;
			if(pDC)
			{
				if(bForce)
				{
					pOldDC = pDC;
					pFeature->m_pDominantColor = NULL;
					pDC = NULL;
				}
				else
				{
					// it is already present and we do not want to recompute it
					return true;
				}
			}
			if(!pDC)
			{
				pDC= new DominantColorType();
			}
		}
	}

	bool bret = true;
	XM::DominantColorExtractionTool *pDCDescriptor = NULL;
	do
	{
		if(!pDC) { bret = false; break; }
		pDCDescriptor = new XM::DominantColorExtractionTool();
		if(!pDCDescriptor) { bret = false;  break; }

		pDCDescriptor->InitExtracting();
		if(pDCDescriptor->StartExtracting(pImage) == 0)
		{
			const XM::DCF *pDcf = pDCDescriptor->GetDCF();
			const XM::dc_t *pdesc = pDCDescriptor->GetDCDescriptor();
			if(pDcf == NULL) { bret = false; break; }
			pDC->m_nSpatialCoherency = pDcf->getSC();
			pDC->m_nValueCount = pDcf->getSize();
			int nSize = pDC->m_nValueCount;
			if(nSize > 0)
			{
				pDC->m_ppValues = new DominantColorValueType*[nSize];
				for(int i = 0; i< nSize; i++)
				{
					pDC->m_ppValues[i] = new DominantColorValueType();
					if(!pDC->m_ppValues[i])
					{
						bret = false;
						break;
					}
					pDC->m_ppValues[i]->m_nPercentage = pdesc[i].weight;
					pDC->m_ppValues[i]->m_pIndex = new ValueListType(3, 12);
					//pDC->m_ppValues[i]->m_pColorVariance =  new ValueListType(3, 1);
					ValueListType *pIndex = pDC->m_ppValues[i]->m_pIndex;
					pIndex->m_pItems = new int[3];
					pIndex->m_pItems[0] = pdesc[i].centroid[0];
					pIndex->m_pItems[1] = pdesc[i].centroid[1];
					pIndex->m_pItems[2] = pdesc[i].centroid[2];
				}
			}
			else
			{
				bret = false;
				break;
			}
			if(bret) pFeature->m_pDominantColor = pDC;
		}
		else
		{
			bret = false;
			delete pDC;
		}
	}
	while(false);
	if(!bret && pFeature)
	{
		if(pOldDC) pFeature->m_pDominantColor = pOldDC;
	}
	else
	{
		if(pOldDC) delete pOldDC;
	}
	if(pDCDescriptor) delete pDCDescriptor;
	return bret;
}

bool SetColorLayoutDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce)
{
	if(!pImage || !pImage->GetImage()) return false;
	StillRegionFeatureType *pFeature = NULL;
	ColorLayoutType *pCL = NULL;
	ColorLayoutType *pOldCL = NULL;
	if(pItem)
	{
		if(pItem->m_pDescriptionUnit 
			&& pItem->m_pDescriptionUnit->m_pImage)
		{
			pFeature = pItem->m_pDescriptionUnit->m_pImage->m_pVisualDescriptionScheme;
			if(pFeature)
			{
				pCL = pFeature->m_pColorLayout;
			}
			else
			{
				pFeature = new StillRegionFeatureType();
			}
			if(!pFeature) return false;
			if(pCL)
			{
				if(bForce)
				{
					pOldCL = pCL;
					pFeature->m_pColorLayout = NULL;
					pCL = NULL;
				}
				else
				{
					// it is already present and we do not want to recompute it
					return true;
				}
			}
			if(!pCL)
			{
				pCL = new ColorLayoutType();
			}
		}
	}

	bool bret = true;
	XM::ColorLayoutExtractionTool *pCLDescriptor = NULL;
	do
	{
		if(!pCL) { bret = false; break; }
		pCLDescriptor = new XM::ColorLayoutExtractionTool();
		if(!pCLDescriptor) { bret = false; break; }

		pCLDescriptor->InitExtracting();
		if(pCLDescriptor->StartExtracting(6, 6, pImage) == 0)
		{
			int nYCoeffNum = 6;
			int nCCoeffNum = 6;
			int *ycoeff = pCLDescriptor->GetYCoeff();
			int *crcoeff = pCLDescriptor->GetCrCoeff();
			int *cbcoeff = pCLDescriptor->GetCbCoeff();
			// Y -> (1 + 5), Cr -> (1 + 5), Cb -> (1 + 5)
			pCL->m_nYDCCoeff = ycoeff[0];
			pCL->m_nCrDCCoeff= crcoeff[0];
			pCL->m_nCbDCCoeff= cbcoeff[0];

			pCL->m_pYACCoeff = new ValueListType(nYCoeffNum - 1, 5);
			pCL->m_pCrACCoeff = new ValueListType(nCCoeffNum - 1, 5);
			pCL->m_pCbACCoeff = new ValueListType(nCCoeffNum - 1, 5);
			if(!pCL->m_pYACCoeff || !pCL->m_pCrACCoeff || !pCL->m_pCbACCoeff) { bret = false; break; }
			pCL->m_pYACCoeff->m_pItems = new int[nYCoeffNum - 1];
			pCL->m_pCrACCoeff->m_pItems = new int[nCCoeffNum - 1];
			pCL->m_pCbACCoeff->m_pItems = new int[nCCoeffNum - 1];
			pCL->m_nYACType = CL_COEFF_5;
			pCL->m_nCbCrACType = CL_COEFF_5;
			for(int i = 0; i < (nYCoeffNum - 1); i++)
			{
				pCL->m_pYACCoeff->m_pItems[i] = ycoeff[i+1];
			}
			for(int i = 0; i < (nCCoeffNum - 1); i++)
			{
				pCL->m_pCrACCoeff->m_pItems[i] = crcoeff[i+1];
				pCL->m_pCbACCoeff->m_pItems[i] = cbcoeff[i+1];
			}
			if(bret) pFeature->m_pColorLayout = pCL;
		}
		else
		{
			bret = false;
			delete pCL;
		}
	}
	while(false);
	if(!bret && pFeature)
	{
		if(pOldCL) pFeature->m_pColorLayout = pOldCL;
	}
	else
	{
		if(pOldCL) delete pOldCL;
	}
	if(pCLDescriptor) delete pCLDescriptor;
	return bret;
}

bool SetColorStructureDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce)
{
	if(!pImage || !pImage->GetImage()) return false;
	StillRegionFeatureType *pFeature = NULL;
	ColorStructureType *pCS = NULL;
	ColorStructureType *pOldCS = NULL;
	if(pItem)
	{
		if(pItem->m_pDescriptionUnit 
			&& pItem->m_pDescriptionUnit->m_pImage)
		{
			pFeature = pItem->m_pDescriptionUnit->m_pImage->m_pVisualDescriptionScheme;
			if(pFeature)
			{
				pCS = pFeature->m_pColorStructure;
			}
			else
			{
				pFeature = new StillRegionFeatureType();
			}
			if(!pFeature) return false;
			if(pCS)
			{
				if(bForce)
				{
					pOldCS = pCS;
					pFeature->m_pColorStructure = NULL;
					pCS = NULL;
				}
				else
				{
					// it is already present and we do not want to recompute it
					return true;
				}
			}
			if(!pCS)
			{
				pCS = new ColorStructureType();
			}
		}
	}

	bool bret = true;
	XM::ColorStructureExtractionTool *pCSDescriptor = NULL;
	do
	{
		if(!pCS) { bret = false; break; }
		pCSDescriptor = new XM::ColorStructureExtractionTool();
		if(!pCSDescriptor) { bret = false; break; }

		int CS_DESC_SIZE = 64;
		pCSDescriptor->InitExtracting();
		if(pCSDescriptor->StartExtracting(CS_DESC_SIZE, pImage) == 0)
		{
			if(pCSDescriptor->GetSize() != CS_DESC_SIZE) { bret = false; break; }
			pCS->m_nColorQuant = ColorStructureType::CS_64_HMMD;
			pCS->m_pValues = new CSValues_anonymousType();
			if(!pCS->m_pValues) { bret = false; break; }
			pCS->m_pValues->m_pItems = new int[CS_DESC_SIZE];
			if(!pCS->m_pValues->m_pItems) { bret = false; break; }
			pCS->m_pValues->m_nMax = CS_DESC_SIZE;
			for(int i = 0; i < (int)pCSDescriptor->GetSize(); i++)
			{
				pCS->m_pValues->m_pItems[i] = pCSDescriptor->GetElement(i);
			}
			if(bret) pFeature->m_pColorStructure = pCS;
		}
		else
		{
			bret = false;
		}
	}
	while(false);
	if(!bret && pFeature)
	{
		if(pOldCS) pFeature->m_pColorStructure = pOldCS;
	}
	else
	{
		if(pOldCS) delete pOldCS;
	}
	if(!bret)
	{
		if(pCS) delete pCS;
	}
	if(pCSDescriptor) delete pCSDescriptor;
	return bret;
}

bool SetScalableColorDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce)
{
	if(!pImage || !pImage->GetImage()) return false;
	StillRegionFeatureType *pFeature = NULL;
	ScalableColorType *pSC = NULL;
	ScalableColorType *pOldSC = NULL;
	if(pItem)
	{
		if(pItem->m_pDescriptionUnit 
			&& pItem->m_pDescriptionUnit->m_pImage)
		{
			pFeature = pItem->m_pDescriptionUnit->m_pImage->m_pVisualDescriptionScheme;
			if(pFeature)
			{
				pSC = pFeature->m_pScalableColor;
			}
			else
			{
				pFeature = new StillRegionFeatureType();
			}
			if(!pFeature) return false;
			if(pSC)
			{
				if(bForce)
				{
					pOldSC = pSC;
					pFeature->m_pScalableColor = NULL;
					pSC = NULL;
				}
				else
				{
					// it is already present and we do not want to recompute it
					return true;
				}
			}
			if(!pSC)
			{
				pSC = new ScalableColorType();
			}
		}
	}

	bool bret = true;
	XM::ScalableColorExtractionTool *pSCDescriptor = NULL;
	do
	{
		if(!pSC) { bret = false; break; }
		pSCDescriptor = new XM::ScalableColorExtractionTool();
		if(!pSCDescriptor) { bret = false; break; }

		int nNumberOfBitplanesDiscarded = 0;
		int nNumOfCoeffs = 64;
		pSCDescriptor->SetNumberOfBitplanesDiscarded(nNumberOfBitplanesDiscarded);
		pSCDescriptor->SetOutputNumOfCoeffs(nNumOfCoeffs);
		pSCDescriptor->InitExtracting();
		if(pSCDescriptor->StartExtracting( pImage) == 0)
		{
			if(!pSC->SetNumberOfBitplanesDiscarded(pSCDescriptor->GetNumberOfBitplanesDiscarded()))
			{
				bret = false;
				break;
			}
			nNumOfCoeffs = pSCDescriptor->GetNumberOfCoefficients();
			if(!pSC->SetNumberOfCoefficients(nNumOfCoeffs))
			{
				bret = false;
				break;
			}
			pSC->m_pCoeffs = new MPEG7IntegerVector();
			if(!pSC->m_pCoeffs) { bret = false; break; }
			pSC->m_pCoeffs->m_nMax = nNumOfCoeffs;
			pSC->m_pCoeffs->m_pItems = new int[nNumOfCoeffs];
			if(!pSC->m_pCoeffs->m_pItems) { bret = false; break; }
			for(int i = 0; i < nNumOfCoeffs; i++)
			{
				pSC->m_pCoeffs->m_pItems[i] = pSCDescriptor->GetCoefficient(i);
			}
			if(bret) pFeature->m_pScalableColor = pSC;
		}
		else
		{
			bret = false;
		}
	}
	while(false);
	if(!bret && pFeature)
	{
		if(pOldSC) pFeature->m_pScalableColor = pOldSC;
	}
	else
	{
		if(pOldSC) delete pOldSC;
	}
	if(!bret)
	{
		if(pSC) delete pSC;
	}
	if(pSCDescriptor) delete pSCDescriptor;
	return bret;
}

bool SetEdgeHistogramDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce)
{
	if(!pImage || !pImage->GetImage()) return false;
	StillRegionFeatureType *pFeature = NULL;
	EdgeHistogramType *pEH = NULL;
	EdgeHistogramType *pOldEH = NULL;
	if(pItem)
	{
		if(pItem->m_pDescriptionUnit 
			&& pItem->m_pDescriptionUnit->m_pImage)
		{
			pFeature = pItem->m_pDescriptionUnit->m_pImage->m_pVisualDescriptionScheme;
			if(pFeature)
			{
				pEH = pFeature->m_pEdge;
			}
			else
			{
				pFeature = new StillRegionFeatureType();
			}
			if(!pFeature) return false;
			if(pEH)
			{
				if(bForce)
				{
					pOldEH = pEH;
					pFeature->m_pEdge = NULL;
					pEH = NULL;
				}
				else
				{
					// it is already present and we do not want to recompute it
					return true;
				}
			}
			if(!pEH)
			{
				pEH = new EdgeHistogramType();
			}
		}
	}

	bool bret = true;
	XM::EdgeHistogramExtractionTool *pEHDescriptor = NULL;
	do
	{
		if(!pEH) { bret = false; break; }
		pEHDescriptor = new XM::EdgeHistogramExtractionTool();
		if(!pEHDescriptor) { bret = false; break; }

		pEHDescriptor->InitExtracting();
		if(pEHDescriptor->StartExtracting( pImage) == 0)
		{
			int nEdgeCount = 80;
			pEH->m_pBinCounts = new ValueListType(nEdgeCount, 3, 20);
			if(!pEH->m_pBinCounts) { bret = false; break; }
			pEH->m_pBinCounts->m_pItems = new int[nEdgeCount];
			if(!pEH->m_pBinCounts->m_pItems) { bret = false; break; }
			char *eh_elems = pEHDescriptor->GetEdgeHistogramElement();
			if(!eh_elems) { bret = false; break; }
			for(int i = 0; i < nEdgeCount; i++)
			{
				pEH->m_pBinCounts->m_pItems[i] = eh_elems[i];
			}

			if(bret) pFeature->m_pEdge = pEH;
		}
		else
		{
			bret = false;
		}
	}
	while(false);
	if(!bret && pFeature)
	{
		if(pOldEH) pFeature->m_pEdge= pOldEH;
	}
	else
	{
		if(pOldEH) delete pOldEH;
	}
	if(!bret)
	{
		if(pEH) delete pEH;
	}
	if(pEHDescriptor) delete pEHDescriptor;
	return bret;
}

bool SetHomogeneousTextureDescription(ItemMpeg7 *pItem, ImageIO *pImage, bool bForce)
{
	if(!pImage || !pImage->GetImage()) return false;
	StillRegionFeatureType *pFeature = NULL;
	HomogeneousTextureType *pHT = NULL;
	HomogeneousTextureType *pOldHT = NULL;
	if(pItem)
	{
		if(pItem->m_pDescriptionUnit 
			&& pItem->m_pDescriptionUnit->m_pImage)
		{
			pFeature = pItem->m_pDescriptionUnit->m_pImage->m_pVisualDescriptionScheme;
			if(pFeature)
			{
				pHT = pFeature->m_pHomogeneousTexture;
			}
			else
			{
				pFeature = new StillRegionFeatureType();
			}
			if(!pFeature) return false;
			if(pHT)
			{
				if(bForce)
				{
					pOldHT = pHT;
					pFeature->m_pHomogeneousTexture = NULL;
					pHT = NULL;
				}
				else
				{
					// it is already present and we do not want to recompute it
					return true;
				}
			}
			if(!pHT)
			{
				pHT = new HomogeneousTextureType();
			}
		}
	}

	bool bret = true;
	XM::HomogeneousTextureExtractionTool *pHTDescriptor = NULL;
	do
	{
		if(!pHT) { bret = false; break; }
		pHTDescriptor = new XM::HomogeneousTextureExtractionTool();
		if(!pHTDescriptor) { bret = false; break; }

		pHTDescriptor->InitExtracting();
		if(pHTDescriptor->StartExtracting( pImage) == 0)
		{
			int nEnergyCount = 30;
			int *ht_data = pHTDescriptor->GetHomogeneousTextureFeature();
			if(!ht_data) { bret = false; break; }
			pHT->m_pEnergy = new ValueListType(nEnergyCount, 8, 0);
			if(!pHT->m_pEnergy) { bret = false; break; }
			pHT->m_pEnergy->m_pItems = new int[nEnergyCount];
			if(!pHT->m_pEnergy->m_pItems) { bret = false; break; }
			pHT->m_pEnergyDeviation = new ValueListType(30, 8, 0);
			if(!pHT->m_pEnergyDeviation) { bret = false; break; }
			pHT->m_pEnergyDeviation->m_pItems = new int[nEnergyCount];
			if(!pHT->m_pEnergyDeviation->m_pItems) { bret = false; break; }

			int i;
			pHT->m_nAverage = ht_data[0];
			pHT->m_nStandardDeviation = ht_data[1];

			for( i = 0; i < nEnergyCount; i++)
			{
				pHT->m_pEnergy->m_pItems[i] = ht_data[i+2];
			}

			for( i = 0; i < nEnergyCount; i++)
			{
				pHT->m_pEnergyDeviation->m_pItems[i] = ht_data[i + nEnergyCount + 2];
			}
			if(bret) pFeature->m_pHomogeneousTexture = pHT;
		}
		else
		{
			bret = false;
		}
	}
	while(false);
	if(!bret && pFeature)
	{
		if(pOldHT) pFeature->m_pHomogeneousTexture= pOldHT;
	}
	else
	{
		if(pOldHT) delete pOldHT;
	}
	if(!bret)
	{
		if(pHT) delete pHT;
	}
	if(pHTDescriptor) delete pHTDescriptor;
	return bret;
}

/** Checks if all track_IDs are present in image list */
bool CheckReference(ContentCollectionType *pCollection, ImageList *pImageList)
{
	bool bOK = true;
	int elements = pCollection->m_nReferenceCount;
	int imgNum = pImageList->NumberOfElements();
	if(pCollection->m_ppReferences)
	{
		for(int i = 0; i< elements; i++)
		{
			if(!pCollection->m_ppReferences[i]) continue;
			int trackId = pCollection->m_ppReferences[i]->m_nImgId;
			bOK = false;
			for(int j = 0; j < imgNum; j++)
			{
				if(pImageList->GetElement(j)->nId == trackId)
				{
					bOK = true;
					break;
				}
			}
			if(!bOK)
			{
				char *buf = new char[128];
				if(buf)
				{
					sprintf(buf, "track_Id = %d has no image data defined in the image list", trackId);
					BimXMLUtil::ReportError(buf);
					delete buf;
				}
				break;
			}
		}
	}
	return bOK;
}

bool item_bim_to_xml(char *buf, int bufsize, char *outFile)
{
	bool bresult = true;
	ItemMpeg7 *itemmpeg = new ItemMpeg7();
	BitstreamReader *reader = NULL;
	do
	{
		reader = new BitstreamReader((unsigned char*)buf, bufsize);
		if(!buf || !itemmpeg || !reader)
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
			bresult = itemmpeg->ReadBitstream(reader);
		}
		if(!bresult)
		{
			BimXMLUtil::ReportError("BiM reading failed");
			break;
		}

		//std::ostream *fout = &std::cout;
		std::ostream *fout = new std::ofstream(outFile);
		if(fout)
		{
			itemmpeg->PrintXML(fout, 0);
			fout->flush();
			delete fout;
		}
		else
		{
			bresult = false;
		}
	}
	while(false);

	if(itemmpeg) delete itemmpeg;
	if(reader) delete reader;

	return bresult;
}

ItemMpeg7 *get_item_bim(char *buf, int bufsize)
{
	bool bresult = true;
	ItemMpeg7 *itemmpeg = new ItemMpeg7();
	BitstreamReader *reader = NULL;

	do
	{
		reader = new BitstreamReader((unsigned char*)buf, bufsize);
		if(!buf || !itemmpeg || !reader)
		{
			BimXMLUtil::ReportError("Out of memory");
			bresult = false;
			break;
		}

#ifdef BIM_BYTE_0X1F
		bresult = BimXMLUtil::ReadDecodingMode(reader);
		if(!bresult)
		{
			break;
		}
#endif
		bresult = itemmpeg->ReadBitstream(reader);
		if(!bresult)
		{
			BimXMLUtil::ReportError("BiM reading failed");
			break;
		}
	}
	while(false);

	if(reader) delete reader;
	if(!bresult)
	{
		delete itemmpeg;
		itemmpeg = NULL;
	}
	return itemmpeg;
}
