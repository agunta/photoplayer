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

#include "bitstreamwriter.h"
#include "bitstreamreader.h"

#include <xercesc/dom/DOM.hpp>
#include <iostream>

#define BIM_BYTE_0X1F

XERCES_CPP_NAMESPACE_USE

#include "ppmpeg7desc.h"
#include "descstring.h"
#include "descanyuri.h"
#include "descanyuri_typecast.h"
#include "timepointtype.h"
#include "textualtype.h"
#include "uniqueidtype.h"
#include "descstring_typecast.h"
#include "profileandlevelindication.h"
#include "descriptionprofiletype.h"
#include "termreferencetype.h"
#include "inlinetermdefinitiontype.h"
#include "controlledtermusetype.h"
#include "geographicpointtype.h"
#include "geographicposition_anonymoustype.h"
#include "postaladdress_anonymoustype.h"
#include "placetype.h"
#include "electronicaddresstype.h"
#include "personnametype.h"
#include "organizationtype.h"
#include "affiliation_anonymoustype.h"
#include "persontype.h"
#include "agenttype.h"
#include "mediaagenttype_content.h"
#include "creatortype.h"
#include "descriptionmetadatatype.h"
#include "mpeg7type_content.h"
#include "hexbinary.h"
#include "base64binary.h"
#include "inlinemediatype.h"
#include "imagelocatortype.h"
#include "titlemediatype.h"
#include "timetype.h"
#include "creationcoord_anonymustype.h"
#include "colcreationtype.h"
#include "colcreationinformationtype.h"
#include "colkeyword_anonymoustype.h"
#include "colkeywordannotationtype.h"
#include "coltextannotationtype.h"
#include "collectiontype_attributes.h"
#include "collectiontype_content.h"
#include "referencetype.h"
#include "contentcollectiontype.h"
#include "colmpeg7.h"

#include "csvalues_anonymoustype.h"
#include "mpeg7integervector.h"
#include "valuelisttype.h"
#include "colorlayouttype.h"
#include "colorstructuretype.h"
#include "dominantcolorvaluetype.h"
#include "dominantcolortype.h"
#include "edgehistogramtype.h"
#include "homogeneoustexturetype.h"
#include "scalablecolortype.h"

#include "stillregionfeaturetype.h"
#include "itemtimetype.h"
#include "itemcreationcoord_anonymustype.h"
#include "frame_anonymoustype.h"
#include "medialocatortype.h"
//#include "uniqueidtype.h"
#include "itemcreationtype.h"
#include "itemcreationinformationtype.h"
#include "visualcoding_anonymoustype.h"
#include "nonnegativeinteger.h"
#include "nonnegativeinteger_typecast.h"
#include "mediainstancetype.h"
#include "mediaformattype.h"
#include "mediaprofiletype.h"
#include "mediainformationtype.h"
#include "segmenttype_content.h"
#include "stillregiontype.h"
#include "imagetype.h"
#include "itemmpeg7.h"

#include "bimxmlutil.h"

#include <stdio.h>

static char* ELEM_NAME_MPEG7                 = "Mpeg7";
static char* ELEM_NAME_DESCRIPTION_PROFILE   = "DescriptionProfile";
static char* ELEM_NAME_DESCRIPTION_METADATA  = "DescriptionMetadata";
static char* ELEM_NAME_DESCRIPTION_UNIT      = "DescriptionUnit";
static char* ELEM_NAME_LAST_UPDATE           = "LastUpdate";
static char* ELEM_NAME_CREATOR               = "Creator";
static char* ELEM_NAME_AGENT                 = "Agent";
static char* ELEM_NAME_PERSON_TYPE           = "PersonType";
static char* ELEM_NAME_ORGANIZATION_TYPE     = "OrganizationType";
static wchar_t* ELEM_UNINAME_PERSON_TYPE        = L"PersonType";
static wchar_t* ELEM_UNINAME_ORGANIZATION_TYPE  = L"OrganizationType";
static char* ELEM_NAME_FAMILY_NAME           = "FamilyName";
static char* ELEM_NAME_GIVEN_NAME            = "GivenName";
static char* ELEM_NAME_LINKING_NAME          = "LinkingName";
static char* ELEM_NAME_NUMERATION            = "Numeration";
static char* ELEM_NAME_SALUTATION            = "Salutation";
static char* ELEM_NAME_ADDRESS               = "Address";
static char* ELEM_NAME_AFFILIATION           = "Affiliation";
static char* ELEM_NAME_ELECTRONIC_ADDRESS    = "ElectronicAddress";
static char* ELEM_NAME_ORGANIZATION          = "Organization";
static char* ELEM_NAME_CREATION_TIME         = "CreationTime";
static char* ELEM_NAME_ROLE                  = "Role";
// <Name> is used in: OrganizationType, InlineTermDefinition, PlaceType, ...
static char* ELEM_NAME_NAME                  = "Name";
static char* ELEM_NAME_CONTENT_REF           = "ContentRef";
static char* ELEM_NAME_CONTENT_COLLECTION    = "ContentCollection";
static char* ELEM_NAME_CREATION_INFORMATION  = "CreationInformation";
static char* ELEM_NAME_CREATION              = "Creation";
static char* ELEM_NAME_TEXT_ANNOTATION       = "TextAnnotation";
static char* ELEM_NAME_KEYWORD_ANNOTATION    = "KeywordAnnotation";
static char* ELEM_NAME_KEYWORD               = "Keyword";
static char* ELEM_NAME_FREE_TEXT             = "FreeTextAnnotation";
// <Title>  is used in: <PersonType>, <CreationType>,
static char* ELEM_NAME_TITLE                 = "Title";
static char* ELEM_NAME_TITLE_MEDIA           = "TitleMedia";
static char* ELEM_NAME_TITLE_IMAGE           = "TitleImage";
static char* ELEM_NAME_INLINE_MEDIA          = "InlineMedia";
static char* ELEM_NAME_MEDIA_DATA16          = "MediaData16";
static char* ELEM_NAME_MEDIA_DATA64          = "MediaData64";
static char* ELEM_NAME_LOCATION              = "Location";
static char* ELEM_NAME_PLACE_DESCRIPTION     = "PlaceDescription";
static char* ELEM_NAME_GEOGRAPHIC_POSITION   = "GeographicPosition";
static char* ELEM_NAME_POINT                 = "Point";
static char* ELEM_NAME_POSTAL_ADDRESS        = "PostalAddress";
static char* ELEM_NAME_ADDRESS_LINE          = "AddressLine";
static char* ELEM_NAME_POSTING_IDENTIFIER    = "PostingIdentifier";
static char* ELEM_NAME_DATE                  = "Date";
static char* ELEM_NAME_TIME_POINT            = "TimePoint";
static char* ELEM_NAME_DURATION              = "Duration";
static char* ELEM_NAME_CREATION_COORD        = "CreationCoordinates";
static char* ELEM_NAME_TELEPHONE             = "Telephone";
static char* ELEM_NAME_FAX                   = "Fax";
static char* ELEM_NAME_EMAIL                 = "Email";
static char* ELEM_NAME_URL                   = "Url";

// Image level specific
static char* ELEM_NAME_IMAGE                 = "Image";
static char* ELEM_NAME_VISUAL_DESC_SCHEME    = "VisualDescriptionScheme";
static char* ELEM_NAME_MEDIA_INFORMATION     = "MediaInformation";
static char* ELEM_NAME_MEDIA_PROFILE         = "MediaProfile";
static char* ELEM_NAME_MEDIA_FORMAT          = "MediaFormat";
static char* ELEM_NAME_MEDIA_INSTANCE        = "MediaInstance";
static char* ELEM_NAME_CONTENT               = "Content";
static char* ELEM_NAME_FILE_FORMAT           = "FileFormat";
static char* ELEM_NAME_FILE_SIZE             = "FileSize";
static char* ELEM_NAME_VISUAL_CODING         = "VisualCoding";
static char* ELEM_NAME_INSTANCE_IDENTIFIER   = "InstanceIdentifier";
static char* ELEM_NAME_LOCATION_DESC         = "LocationDescription";
static char* ELEM_NAME_MEDIA_LOCATOR         = "MediaLocator";
static char* ELEM_NAME_MEDIA_URI             = "MediaUri";
static char* ELEM_NAME_FRAME                 = "Frame";

static char* ELEM_NAME_DOMINANT_COLOR        = "DominantColor";
static char* ELEM_NAME_SCALABLE_COLOR        = "ScalableColor";
static char* ELEM_NAME_COLOR_STRUCTURE       = "ColorStructure";
static char* ELEM_NAME_COLOR_LAYOUT          = "ColorLayout";
static char* ELEM_NAME_EDGE                  = "Edge";
static char* ELEM_NAME_HOMOGENEOUS_PATTERN   = "HomogeneousPattern";
// Edge histogram
static char* ELEM_NAME_BIN_COUNTS            = "BinCounts";
// Homogeneous texture
static char* ELEM_NAME_AVERAGE               = "Average";
static char* ELEM_NAME_STANDARD_DEVIATION    = "StandardDeviation";
static char* ELEM_NAME_ENERGY                = "Energy";
static char* ELEM_NAME_ENERGY_DEVIATION      = "EnergyDeviation";
// Dominant color
static char* ELEM_NAME_SPATIAL_COHERENCY     = "SpatialCoherency";
static char* ELEM_NAME_VALUE                 = "Value";
static char* ELEM_NAME_PERCENTAGE            = "Percentage";
static char* ELEM_NAME_INDEX                 = "Index";
static char* ELEM_NAME_COLOR_VARIANCE        = "ColorVariance";
// ScalableColor
static char* ELEM_NAME_COEFF                 = "Coeff";
// Color Structure
static char* ELEM_NAME_VALUES                = "Values";
// ColorLayout
static char* ELEM_NAME_YDCCOEFF              = "YDCCoeff";
static char* ELEM_NAME_CBDCCOEFF             = "CbDCCoeff";
static char* ELEM_NAME_CRDCCOEFF             = "CrDCCoeff";
static char* ELEM_NAME_YACCOEFF14            = "YACCoeff14";
static char* ELEM_NAME_YACCOEFF20            = "YACCoeff20";
static char* ELEM_NAME_YACCOEFF27            = "YACCoeff27";
static char* ELEM_NAME_YACCOEFF5             = "YACCoeff5";
static char* ELEM_NAME_YACCOEFF63            = "YACCoeff63";
static char* ELEM_NAME_YACCOEFF9             = "YACCoeff9";
static char* ELEM_NAME_CBACCOEFF14            = "CbACCoeff14";
static char* ELEM_NAME_CBACCOEFF20            = "CbACCoeff20";
static char* ELEM_NAME_CBACCOEFF27            = "CbACCoeff27";
static char* ELEM_NAME_CBACCOEFF5             = "CbACCoeff5";
static char* ELEM_NAME_CBACCOEFF63            = "CbACCoeff63";
static char* ELEM_NAME_CBACCOEFF9             = "CbACCoeff9";
static char* ELEM_NAME_CRACCOEFF14            = "CrACCoeff14";
static char* ELEM_NAME_CRACCOEFF20            = "CrACCoeff20";
static char* ELEM_NAME_CRACCOEFF27            = "CrACCoeff27";
static char* ELEM_NAME_CRACCOEFF5             = "CrACCoeff5";
static char* ELEM_NAME_CRACCOEFF63            = "CrACCoeff63";
static char* ELEM_NAME_CRACCOEFF9             = "CrACCoeff9";

static char* ATTR_NAME_XMLNS = "xmlns";
static char* ATTR_NAME_XMLNS_XSI = "xmlns:xsi";
//static char* ATTR_NAME_LANG = "lang";
static char* ATTR_NAME_XML_LANG = "xml:lang";
static char* ATTR_NAME_ID   = "id";
static char* ATTR_NAME_HREF = "href";
static char* ATTR_NAME_NAME = "name";
static char* ATTR_NAME_IDREF = "idref";
static char* ATTR_NAME_XPATH = "xpath";
static char* ATTR_NAME_TYPE = "type";
static char* ATTR_NAME_XSI_TYPE = "xsi:type";
static char* ATTR_NAME_DATUM = "datum";
static char* ATTR_NAME_ALTITUDE = "altitude";
static char* ATTR_NAME_LATITUDE = "latitude";
static char* ATTR_NAME_LONGITUDE = "longitude";
static char* ATTR_NAME_PROFILEANDLEVEL = "profileAndLevelIndication";
static char* ATTR_NAME_ENCODING = "encoding";
static char* ATTR_NAME_HEIGHT = "height";
static char* ATTR_NAME_WIDTH = "width";
static char* ATTR_NAME_NUM_OF_COEFF = "numOfCoeff";
static char* ATTR_NAME_NUM_OF_BITPLANES_DISCARDED = "numOfBitplanesDiscarded";
static char* ATTR_NAME_COLOR_QUANT = "colorQuant";

// string-cast
static wchar_t* ELEM_UNINAME_NORMALIZEDSTRING = L"normalizedString";
static wchar_t* ELEM_UNINAME_TOKEN = L"token";
static wchar_t* ELEM_UNINAME_NMTOKEN = L"NMTOKEN";
static wchar_t* ELEM_UNINAME_NMTOKENS = L"NMTOKENS";
static wchar_t* ELEM_UNINAME_TERMALIASREF = L"termAliasReferenceType";
static wchar_t* ELEM_UNINAME_NAME = L"Name";
static wchar_t* ELEM_UNINAME_NCNAME = L"NCName";
static wchar_t* ELEM_UNINAME_ENTITY = L"ENTITY";
static wchar_t* ELEM_UNINAME_ENTITIES = L"ENTITIES";
static wchar_t* ELEM_UNINAME_ID = L"ID";
static wchar_t* ELEM_UNINAME_IDREF = L"IDREF";
static wchar_t* ELEM_UNINAME_IDREFS = L"IDREFS";
static wchar_t* ELEM_UNINAME_LANGUAGE = L"language";
static wchar_t* ELEM_UNINAME_TEXTUALBASETYPE = L"TextualBaseType";
static wchar_t* ELEM_UNINAME_NAMECOMPONENTTYPE = L"NameComponentType";
static wchar_t* ELEM_UNINAME_TEXTUALTYPE = L"TextualType";
static wchar_t* ELEM_UNINAME_TITLETYPE = L"TitleType";
static wchar_t* ELEM_UNINAME_UNIQUEIDTYPE = L"UniqueIDType";
//static wchar_t* ELEM_UNINAME_BASICDURATIONTYPE = L"basicDurationType";
//static wchar_t* ELEM_UNINAME_DURATIONTYPE = L"durationType";
static wchar_t* ELEM_UNINAME_BASICTIMEPOINTTYPE = L"basicTimePointType";
static wchar_t* ELEM_UNINAME_TIMEPOINTTYPE = L"timePointType";
static wchar_t* ELEM_UNINAME_MIMETYPE = L"mimeType";
// to do: define other string-typecast names

static const char* MPEG7_ITEM_SCHEMA_NS = "urn:mpeg:maf:schema:photoplayer:item:2006";
static const char* MPEG7_COL_SCHEMA_NS = "urn:mpeg:maf:schema:photoplayer:collection:2006";
static const char* MPEG7_SCHEMA_XMLNS_XSI = "http://www.w3.org/2001/XMLSchema-instance";
static const char* MPEG7_ITEM_SCHEMA_LOCATION = "urn:mpeg:maf:schema:photoplayer:item:2006 ppmaf_item_schema.xsd";
static const char* MPEG7_COL_SCHEMA_LOCATION = "urn:mpeg:maf:schema:photoplayer:collection:2006 ppmaf_collection_schema.xsd";

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

// daletes table of pointers p, the number of pointers - n
#define SAFE_DELETEPP(p,n)  { if(p) { for( int i = 0; i < (n); i++){ if((p)[i])\
{ delete (p)[i]; (p)[i] = NULL; } } delete [] (p); (p) = NULL; } }

#define BREAK_IFNULL(p) { if((p) == NULL) { bret = false; break; } }
#define RETURN_IFNULL(p) { if((p) == NULL) { return false; } }
#define ZEROP(p,n) { for( int i = 0; i < (n); i++){ (p)[i] = NULL; } }
