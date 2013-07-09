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
	$Id: MediaDataAtom.c,v 1.3 2000/05/17 08:01:30 francesc Exp $
*/

#include "MP4Atoms.h"
#include <stdlib.h>
#include <stdio.h>

static MP4Err calculateSize( struct MP4Atom* s );

static void destroy( MP4AtomPtr s )
{
	MP4MediaDataAtomPtr self;
	self = (MP4MediaDataAtomPtr) s;
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
	MP4MediaDataAtomPtr self = (MP4MediaDataAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4SerializeCommonBaseAtomFields( s, buffer ); if (err) goto bail;
    buffer += self->bytesWritten;
    PUTBYTES( self->data, (u32) self->dataSize );
	assert( self->bytesWritten == self->size );
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err writeToFile( struct MP4MediaDataAtom *self, FILE *fd )
{
	MP4Err err;
	size_t written;
	char buffer[ 16 ];

	err = MP4NoErr;
	if ( fd == NULL )
		BAILWITHERROR( MP4BadParamErr )
	err = calculateSize( (MP4AtomPtr) self ); if (err) goto bail;
	err = MP4SerializeCommonBaseAtomFields( (MP4AtomPtr) self, buffer ); if (err) goto bail;
	written = fwrite( buffer, 1, self->bytesWritten, fd );
	if ( written != self->bytesWritten )
		BAILWITHERROR( MP4IOErr )
	written = fwrite( self->data, 1, (u32) self->dataSize, fd );
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
	MP4MediaDataAtomPtr self = (MP4MediaDataAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4CalculateBaseAtomFieldSize( s ); if (err) goto bail;
	self->size += (u32) self->dataSize;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err addData( struct MP4MediaDataAtom *self, MP4Handle dataH )
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
    	memcpy( self->data + (u32) self->dataSize, *dataH, size );
    	self->dataSize += size;
	}
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	MP4Err err;
	long bytesToRead;
	MP4MediaDataAtomPtr self = (MP4MediaDataAtomPtr) s;
	
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

MP4Err MP4CreateMediaDataAtom( MP4MediaDataAtomPtr *outAtom )
{
	MP4Err err;
	MP4MediaDataAtomPtr self;
	
	self = (MP4MediaDataAtomPtr) calloc( 1, sizeof(MP4MediaDataAtom) );
	TESTMALLOC( self )

	err = MP4CreateBaseAtom( (MP4AtomPtr) self );
	if ( err ) goto bail;
	self->type = MP4MediaDataAtomType;
	self->name                = "media data";
	self->destroy             = destroy;
	self->createFromInputStream = (cisfunc) createFromInputStream;
	self->data                = NULL;
	self->calculateSize         = calculateSize;
	self->serialize             = serialize;
	self->addData   = addData;
	self->writeToFile = writeToFile;
	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}
