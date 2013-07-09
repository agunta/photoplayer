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

/*
	$Id: MP4Metadata.c,v 1.0 2006/07/25 17:17:47 KW $
*/


#include "MP4Movies.h"
#include "MP4Atoms.h"


MP4_EXTERN ( MP4Err )
MP4GetBiMMetadata( MP4Metadata theMetaData, MP4Handle dataH, u32* dataSize )
{
	MP4Err err;
	MP4MetadataAtomPtr mta;
	MP4BinaryXMLAtomPtr bi;
	err = MP4NoErr;
	if ( (theMetaData == NULL) || (dataH == NULL) || (dataSize == NULL) )
	{
	    BAILWITHERROR( MP4BadParamErr );
	}
	mta = (MP4MetadataAtomPtr) theMetaData;
	if ( mta->bim == NULL )
	{
		BAILWITHERROR( MP4BadDataErr );
	}
	bi = (MP4BinaryXMLAtomPtr) mta->bim;

	err = bi->getData( bi, dataH, dataSize); if (err) goto bail;
bail:
	TEST_RETURN( err );
	
	return err;
}

