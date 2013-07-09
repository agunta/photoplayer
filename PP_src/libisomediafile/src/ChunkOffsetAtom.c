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
	$Id: ChunkOffsetAtom.c,v 1.2 2000/05/17 08:01:27 francesc Exp $
*/

#include "MP4Atoms.h"
#include <stdlib.h>

static void destroy( MP4AtomPtr s )
{
	MP4Err err;
	MP4ChunkOffsetAtomPtr self;
	err = MP4NoErr;
	self = (MP4ChunkOffsetAtomPtr) s;
	if ( self == NULL )
		BAILWITHERROR( MP4BadParamErr )
	if ( self->offsets )
	{
		free( self->offsets );
		self->offsets = NULL;
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
	u32 i;
	MP4ChunkOffsetAtomPtr self = (MP4ChunkOffsetAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4SerializeCommonFullAtomFields( (MP4FullAtomPtr) s, buffer ); if (err) goto bail;
    buffer += self->bytesWritten;
	PUT32( entryCount );
	for ( i = 0; i < self->entryCount; i++ )
	{
		PUT32_V( self->offsets[i] );
	}
	assert( self->bytesWritten == self->size );
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err addOffset(struct MP4ChunkOffsetAtom *self, u32 offset )
{
	MP4Err err;
	err = MP4NoErr;

	self->entryCount++;
	self->offsets = realloc( self->offsets, self->entryCount * sizeof(u32) );
	TESTMALLOC( self->offsets );
	self->offsets[ self->entryCount - 1 ] = offset;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err calculateSize( struct MP4Atom* s )
{
	MP4Err err;
	MP4ChunkOffsetAtomPtr self = (MP4ChunkOffsetAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4CalculateFullAtomFieldSize( (MP4FullAtomPtr) s ); if (err) goto bail;
	self->size += 4 + (4 * self->entryCount);
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err getChunkOffset( MP4AtomPtr s, u32 chunkIndex, u64 *outOffset )
{
	MP4Err err;
	MP4ChunkOffsetAtomPtr self = (MP4ChunkOffsetAtomPtr) s;
	
	err = MP4NoErr;
	if ( (self == NULL) || (outOffset == NULL) || (chunkIndex == 0) || (chunkIndex > self->entryCount) )
		BAILWITHERROR( MP4BadParamErr )
	*outOffset = self->offsets[ chunkIndex - 1 ];
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	MP4Err err;
	u32 entries;
	u32* p;
	MP4ChunkOffsetAtomPtr self = (MP4ChunkOffsetAtomPtr) s;
	
	err = MP4NoErr;
	if ( self == NULL )	BAILWITHERROR( MP4BadParamErr )
	err = self->super->createFromInputStream( s, proto, (char*) inputStream ); if ( err ) goto bail;

	GET32( entryCount );

	self->offsets = (u32 *) calloc( self->entryCount, sizeof(u32) );
	TESTMALLOC( self->offsets );
	for ( entries = 0, p = self->offsets; entries < self->entryCount; entries++, p++ )
	{
		u32 offset;
		GET32_V( offset );
		*p = offset;
	}
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4CreateChunkOffsetAtom( MP4ChunkOffsetAtomPtr *outAtom )
{
	MP4Err err;
	MP4ChunkOffsetAtomPtr self;
	
	self = (MP4ChunkOffsetAtomPtr) calloc( 1, sizeof(MP4ChunkOffsetAtom) );
	TESTMALLOC( self );

	err = MP4CreateFullAtom( (MP4AtomPtr) self );
	if ( err ) goto bail;
	self->type = MP4ChunkOffsetAtomType;
	self->name                = "chunk offset";
	self->createFromInputStream = (cisfunc) createFromInputStream;
	self->destroy             = destroy;
	self->getChunkOffset      = getChunkOffset;
	self->calculateSize         = calculateSize;
	self->serialize             = serialize;
	self->addOffset = addOffset;
	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}
