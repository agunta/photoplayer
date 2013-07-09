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
	$Id: MJ2BitsPerComponentAtom.c,v 1.0 2000/10/05 14:48:00 tm Exp $
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
// Copyright © 2007

#include "MJ2Atoms.h"
#include <stdlib.h>

static void destroy( MP4AtomPtr s )
{
	ISOErr err = ISONoErr;
	MJ2BitsPerComponentAtomPtr self = (MJ2BitsPerComponentAtomPtr) s;
	
	if ( self == NULL )
		BAILWITHERROR( ISOBadParamErr )
		
	if ( self->bitsPerComponent )
	{
		free( self->bitsPerComponent );
		self->bitsPerComponent = NULL;
	}

	if ( self->super )
		self->super->destroy( s );
bail:
	TEST_RETURN( err );

	return;
}

static ISOErr serialize( struct MP4Atom* s, char* buffer )
{
	ISOErr err;
	u32 i;
	MJ2BitsPerComponentAtomPtr self = (MJ2BitsPerComponentAtomPtr) s;
	
	err = ISONoErr;
	
	err = MP4SerializeCommonBaseAtomFields( s, buffer ); if (err) goto bail;
    buffer += self->bytesWritten; 
    
	for ( i = 0; i < self->bpcCount; i++ )
	{
  	  PUTBYTES( &self->bitsPerComponent[i], sizeof(u8) );
	}
    
	assert( self->bytesWritten == self->size );
bail:
	TEST_RETURN( err );

	return err;
}

static ISOErr calculateSize( struct MP4Atom* s )
{
	ISOErr err;
	MJ2BitsPerComponentAtomPtr self = (MJ2BitsPerComponentAtomPtr) s;
	err = ISONoErr;
	
	err = MP4CalculateBaseAtomFieldSize( s ); if (err) goto bail;
	self->size += self->bpcCount * sizeof(u8);
bail:
	TEST_RETURN( err );

	return err;
}

/* add a bits-per-component item to the list */
static ISOErr addbpcItem(struct MJ2BitsPerComponentAtom *self, u8 bpc )
{
	ISOErr err;
    err = ISONoErr;
	self->bpcCount++;
	self->bitsPerComponent = realloc( self->bitsPerComponent, self->bpcCount * sizeof(u8) );
	TESTMALLOC( self->bitsPerComponent );
	self->bitsPerComponent[ self->bpcCount - 1 ] = (u8) bpc;
bail:
	TEST_RETURN( err );

	return err;
}

static ISOErr createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	ISOErr err;
	u32 items = 0;
	long bytesToRead;
	MJ2BitsPerComponentAtomPtr self = (MJ2BitsPerComponentAtomPtr) s;
	
	err = ISONoErr;
	if ( self == NULL )
		BAILWITHERROR( ISOBadParamErr )
	err = self->super->createFromInputStream( s, proto, (char*) inputStream ); if ( err ) goto bail;

	bytesToRead = self->size - self->bytesRead;
	if ( bytesToRead < sizeof( u8 ) )							/* there must be at least one item in the list */
		BAILWITHERROR( ISOBadDataErr )
	
	if ( self->bitsPerComponent )
		free( self->bitsPerComponent );

	self->bitsPerComponent = (u32 *) calloc( 1, bytesToRead );
	TESTMALLOC( self->bitsPerComponent );

	while ( bytesToRead > 0 )
	{
		if ( bytesToRead < sizeof( u8 ) )						/* we need to read a full u8 */
			BAILWITHERROR( ISOBadDataErr )
	
		GET8( bitsPerComponent[items] );
		items++;
		bytesToRead = self->size - self->bytesRead;
	}		

	self->bpcCount = items;
bail:
	TEST_RETURN( err );

	return err;
}

ISOErr MJ2CreateBitsPerComponentAtom( MJ2BitsPerComponentAtomPtr *outAtom )
{
	ISOErr err;
	MJ2BitsPerComponentAtomPtr self;
	
	self = (MJ2BitsPerComponentAtomPtr) calloc( 1, sizeof( MJ2BitsPerComponentAtom ) );
	TESTMALLOC( self );

	err = MP4CreateBaseAtom( (MP4AtomPtr) self );
	if ( err ) goto bail;
	
	self->type					= MJ2BitsPerComponentAtomType;
	self->name					= "JPEG 2000 bits-per-component atom";
	self->destroy				= destroy;
	self->createFromInputStream = (cisfunc) createFromInputStream;
	self->calculateSize			= calculateSize;
	self->serialize				= serialize;	
	self->addbpcItem			= addbpcItem;	
	
	// KW: changed due to memory error: self->bitsPerComponent		= (u32 *) calloc( 1, sizeof( u8 ) );
	self->bitsPerComponent		= (u32 *) calloc( 1, sizeof( u32 ) );
	TESTMALLOC( self->bitsPerComponent );
	
	self->bitsPerComponent[0]	= 0;
	self->bpcCount				= (u32) 1;

	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}

