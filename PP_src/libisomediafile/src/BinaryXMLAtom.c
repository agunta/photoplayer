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
	$Id: BinaryXMLAtom.c,v 1.0 2006/07/20 16:20:00 KW $
*/

#include "MP4Atoms.h"
#include <stdlib.h>
#include <stdio.h>

static MP4Err calculateSize( struct MP4Atom* s );

static void destroy( MP4AtomPtr s )
{
	MP4BinaryXMLAtomPtr self;
	self = (MP4BinaryXMLAtomPtr) s;
	if ( self->data )
	{
		free( self->data );
		self->data = NULL;
	}
	if ( self->super )
		self->super->destroy( s );
}

static MP4Err serialize( struct MP4Atom* s, char* buffer )
{
	MP4Err err;
	MP4BinaryXMLAtomPtr self = (MP4BinaryXMLAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4SerializeCommonFullAtomFields( (MP4FullAtomPtr)s, buffer ); if (err) goto bail;
    buffer += self->bytesWritten;
    PUTBYTES( self->data, self->dataSize );
	assert( self->bytesWritten == self->size );
bail:
	TEST_RETURN( err );

	return err;
}

/* !!! to be removed */
static MP4Err writeToFile( struct MP4BinaryXMLAtom *self, FILE *fd )
{
	MP4Err err;
	size_t written;
	char buffer[ 16 ];

	err = MP4NoErr;
	if ( fd == NULL )
		BAILWITHERROR( MP4BadParamErr )
	err = MP4CalculateFullAtomFieldSize( (MP4FullAtomPtr) self ); if (err) goto bail;
	err = MP4SerializeCommonFullAtomFields( (MP4FullAtomPtr) self, buffer ); if (err) goto bail;
	written = fwrite( buffer, 1, self->bytesWritten, fd );
	if ( written != self->bytesWritten )
		BAILWITHERROR( MP4IOErr )
	written = fwrite( self->data, 1, self->dataSize, fd );
		if ( written != self->dataSize )
			BAILWITHERROR( MP4IOErr )
	self->bytesWritten += written;
bail:
	TEST_RETURN( err );

	return err;
}


static MP4Err calculateSize( struct MP4Atom* s )
{
	MP4Err err;
	MP4BinaryXMLAtomPtr self = (MP4BinaryXMLAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4CalculateFullAtomFieldSize( (MP4FullAtomPtr) s ); if (err) goto bail;
	self->size += self->dataSize;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err addData( struct MP4BinaryXMLAtom *self, MP4Handle dataH )
{
	MP4Err err;
	u32 size;
	u32 newSize;
	
	err = MP4NoErr;
	err = MP4GetHandleSize( dataH, &size ); if (err) goto bail;
	if ( size > 0 )
	{
    	newSize = (u32) self->dataSize + size;
    	self->data = realloc( self->data, newSize );
    	TESTMALLOC( self->data );
    	memcpy( self->data + self->dataSize, *dataH, size );
    	self->dataSize += size;
	}
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err getData( struct MP4BinaryXMLAtom *self, MP4Handle dataH, u32 *dataSize )
{
	MP4Err err;
	u32 size = self->dataSize;
	*dataSize = 0;
	if(self == NULL || dataH == NULL || dataSize == NULL)
	{
		BAILWITHERROR(MP4BadParamErr)
	}
	err = MP4SetHandleSize( dataH, size); if (err) goto bail;
	memcpy( *dataH , self->data, size );
	TESTMALLOC(*dataH)

	*dataSize = size;
	err = MP4NoErr;
bail:
	return err;
}

static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	MP4Err err;
	long bytesToRead;
	MP4BinaryXMLAtomPtr self = (MP4BinaryXMLAtomPtr) s;
	
	err = MP4NoErr;
	if ( self == NULL )
		BAILWITHERROR( MP4BadParamErr )
	err = self->super->createFromInputStream( s, proto, (char*) inputStream );

	bytesToRead = s->size - s->bytesRead;
	self->data = calloc( 1, bytesToRead );
	TESTMALLOC( self->data )
	
	err = inputStream->readData( inputStream, bytesToRead, self->data, "data" );
	if ( err ) goto bail;

	s->bytesRead += bytesToRead;
	self->dataSize = bytesToRead;
bail:
	TEST_RETURN( err );

	if ( err && self->data )
	{
		free( self->data );
	}
	return err;
}

MP4Err MP4CreateBinaryXMLAtom( MP4BinaryXMLAtomPtr *outAtom )
{
	MP4Err err;
	MP4BinaryXMLAtomPtr self;
	
	self = (MP4BinaryXMLAtomPtr) calloc( 1, sizeof(MP4BinaryXMLAtom) );
	TESTMALLOC( self )

	err = MP4CreateFullAtom( (MP4AtomPtr) self );
	if ( err ) goto bail;
	self->type = MP4BinaryXMLAtomType;
	self->name                = "binary xml";
	self->destroy             = destroy;
	self->createFromInputStream = (cisfunc) createFromInputStream;
	self->data                = NULL;
	self->calculateSize       = calculateSize;
	self->serialize           = serialize;
	self->addData             = addData;
	self->getData             = getData;
	self->writeToFile = writeToFile;
	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}
