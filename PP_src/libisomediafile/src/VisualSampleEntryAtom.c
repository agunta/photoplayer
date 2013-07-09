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
	$Id: VisualSampleEntryAtom.c,v 1.2 2000/05/17 08:01:32 francesc Exp $
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

#include "MP4Atoms.h"
#include <stdlib.h>

static void destroy( MP4AtomPtr s )
{
	MP4Err err;
	MP4VisualSampleEntryAtomPtr self;
	err = MP4NoErr;
	self = (MP4VisualSampleEntryAtomPtr) s;
	if ( self == NULL ) BAILWITHERROR( MP4BadParamErr )
	if ( self->ESDAtomPtr )
	{
		self->ESDAtomPtr->destroy( self->ESDAtomPtr );
		self->ESDAtomPtr = NULL;
	}
	if ( self->super )
		self->super->destroy( s );
bail:
	TEST_RETURN( err );

	return;
}

static MP4Err serialize( struct MP4Atom* s, char* buffer )
{
	MP4Err err;
	MP4VisualSampleEntryAtomPtr self = (MP4VisualSampleEntryAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4SerializeCommonBaseAtomFields( s, buffer ); if (err) goto bail;
    buffer += self->bytesWritten;
	PUTBYTES( self->reserved1, 6 );
	PUT16( dataReferenceIndex );
	PUTBYTES( self->reserved2, 16 );
	PUT16( width );
	PUT16( height );
	//PUT32( reserved3 );
	PUT32( reserved4 );
	PUT32( reserved5 );
	PUT32( reserved6 );
	PUT16( reserved7 );
	PUT8( nameLength );
	PUTBYTES(self->name31, 31 );
	PUT16( reserved8 );
	PUT16( reserved9 );
	SERIALIZE_ATOM( ESDAtomPtr );
	assert( self->bytesWritten == self->size );
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err calculateSize( struct MP4Atom* s )
{
	MP4Err err;
	MP4VisualSampleEntryAtomPtr self = (MP4VisualSampleEntryAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4CalculateBaseAtomFieldSize( s ); if (err) goto bail;
	self->size += (6 + 16 + 31 + (4*2)+(1*1)+(4*4));
	ADD_ATOM_SIZE( ESDAtomPtr );
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	MP4Err err;
	MP4VisualSampleEntryAtomPtr self = (MP4VisualSampleEntryAtomPtr) s;
	
	err = MP4NoErr;
	if ( self == NULL )	BAILWITHERROR( MP4BadParamErr )
	err = self->super->createFromInputStream( s, proto, (char*) inputStream ); if ( err ) goto bail;

	GETBYTES( 6, reserved1 );
	GET16( dataReferenceIndex );
	GETBYTES( 16, reserved2 );
	//GET32( reserved3 );
	GET16( width );
	GET16( height );
	GET32( reserved4 );
	GET32( reserved5 );
	GET32( reserved6 );
	GET16( reserved7 );
	GET8( nameLength );
	GETBYTES( 31, name31 );
	GET16( reserved8 );
	GET16( reserved9 );
	GETATOM( ESDAtomPtr );

bail:
	TEST_RETURN( err );

	return err;
}

// KW: added 2007
static MP4Err setDimensions( struct MP4VisualSampleEntryAtom *self , u16 width, u16 height)
{
	MP4Err err = MP4NoErr;
	if ( self == NULL )	BAILWITHERROR( MP4BadParamErr );
	self->width = width;
	self->height = height;

bail:
	TEST_RETURN( err );

	return err;
}
// added end

MP4Err MP4CreateVisualSampleEntryAtom( MP4VisualSampleEntryAtomPtr *outAtom )
{
	MP4Err err;
	MP4VisualSampleEntryAtomPtr self;
	
	self = (MP4VisualSampleEntryAtomPtr) calloc( 1, sizeof(MP4VisualSampleEntryAtom) );
	TESTMALLOC( self )

	err = MP4CreateBaseAtom( (MP4AtomPtr) self );
	if ( err ) goto bail;
	self->setDimensions = setDimensions;
	self->type = MP4VisualSampleEntryAtomType;
	self->name                = "visual sample entry";
	self->width		= 320; // KW: test
	self->height	= 240;
	self->createFromInputStream	 = (cisfunc) createFromInputStream;
	self->destroy             = destroy;
	self->calculateSize         = calculateSize;
	self->serialize             = serialize;
	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}
