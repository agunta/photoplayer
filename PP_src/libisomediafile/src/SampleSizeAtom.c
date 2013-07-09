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
	$Id: SampleSizeAtom.c,v 1.2 2000/05/17 08:01:30 francesc Exp $
*/

#include "MP4Atoms.h"
#include <stdlib.h>

static void destroy( MP4AtomPtr s )
{
	MP4Err err;
	MP4SampleSizeAtomPtr self;
	err = MP4NoErr;
	self = (MP4SampleSizeAtomPtr) s;
	if ( self == NULL )	BAILWITHERROR( MP4BadParamErr )
	if ( self->sizes )
	{
		free( self->sizes );
		self->sizes = NULL;
	}
	if ( self->super )
		self->super->destroy( s );
bail:
	TEST_RETURN( err );

	return;
}

static MP4Err addSamples( struct MP4SampleSizeAtom *self, u32 sampleCount, MP4Handle sizesH )
{
	MP4Err err;
	u32 *sampleSizes;
	u32 sizesCount;
	
	err = MP4NoErr;
	sampleSizes = (u32 *) *sizesH;
	err = MP4GetHandleSize( sizesH, &sizesCount ); if (err) goto bail;
	sizesCount /= sizeof(u32);
	
	if ( sizesCount > 1 || (*sampleSizes == 0) || ((self->sampleCount > 0) && (*sampleSizes != self->sampleSize)) )
	{
		u32 *newsizes;
		
		if ( (self->sizes == NULL) && self->sampleCount )
		{
			u32 i;
			self->sizes = (u32 *) calloc( self->sampleCount, sizeof(u32) );
			TESTMALLOC( self->sizes );
			for ( i = 0; i < self->sampleCount; i++ )
				self->sizes[ i ] = self->sampleSize;
		}
		newsizes = (u32*) calloc( sampleCount + self->sampleCount, sizeof(u32) );
		TESTMALLOC( newsizes );
		memcpy( newsizes, self->sizes, (self->sampleCount * sizeof(u32)) );
		if ( sizesCount > 1 )
			memcpy( newsizes + self->sampleCount, sampleSizes, sizesCount * sizeof(u32) );
		else
		{
			u32 i;
			u32 *p;
			p = newsizes + self->sampleCount;
			for ( i = 0; i < sampleCount; i++ )
				*p++ = *sampleSizes;
		}
		free( self->sizes );
		self->sizes = newsizes;
		self->sampleSize = 0;
	}
	else
	{
		self->sampleSize = *sampleSizes;
	}
	self->sampleCount += sampleCount;
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err getSampleSize( MP4AtomPtr s, u32 sampleNumber, u32 *outSize )
{
	MP4Err err;
	MP4SampleSizeAtomPtr self = (MP4SampleSizeAtomPtr) s;
	
	err = MP4NoErr;
	if ( (self == NULL) || (outSize == NULL) || (sampleNumber > self->sampleCount) || (sampleNumber == 0) )
		BAILWITHERROR( MP4BadParamErr )
	
	if ( self->sampleSize )
	{
		*outSize = self->sampleSize;
	}
	else
	{
		*outSize = self->sizes[sampleNumber - 1];
	}
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err getSampleSizeAndOffset( MP4AtomPtr s, u32 sampleNumber, u32 *outSize, u32 startingSampleNumber, u32 *outOffsetSize )
{
	MP4Err err;
	MP4SampleSizeAtomPtr self = (MP4SampleSizeAtomPtr) s;
	
	err = MP4NoErr;
	if ( (self == NULL) || (outSize == NULL) || (sampleNumber > self->sampleCount) || (sampleNumber == 0) )
		BAILWITHERROR( MP4BadParamErr )
	if ( (startingSampleNumber == 0) || (startingSampleNumber > sampleNumber) || (outOffsetSize == NULL) )
		BAILWITHERROR( MP4BadParamErr )
	
	if ( self->sampleSize )
	{
		*outSize = self->sampleSize;
		*outOffsetSize = (sampleNumber - startingSampleNumber) * self->sampleSize;
	}
	else
	{
		u32 i;
		u32 offset;
		offset = 0;
		*outSize = self->sizes[sampleNumber - 1];
		for ( i = startingSampleNumber - 1; i < sampleNumber - 1; i++ )
			offset += self->sizes[ i ];
		*outOffsetSize = offset;
	}
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err serialize( struct MP4Atom* s, char* buffer )
{
	MP4Err err;
	u32 i;
	MP4SampleSizeAtomPtr self = (MP4SampleSizeAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4SerializeCommonFullAtomFields( (MP4FullAtomPtr) s, buffer ); if (err) goto bail;
    buffer += self->bytesWritten;
	PUT32( sampleSize );
	PUT32( sampleCount );
	if ( self->sampleSize == 0 )
	{
		for ( i = 0; i < self->sampleCount; i++ )
		{
			PUT32_V( self->sizes[i] );
		}
	}
	assert( self->bytesWritten == self->size );
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err calculateSize( struct MP4Atom* s )
{
	MP4Err err;
	MP4SampleSizeAtomPtr self = (MP4SampleSizeAtomPtr) s;
	err = MP4NoErr;
	
	err = MP4CalculateFullAtomFieldSize( (MP4FullAtomPtr) s ); if (err) goto bail;
	self->size += 8;
	if ( self->sampleSize == 0 )
		self->size += (4*self->sampleCount);
bail:
	TEST_RETURN( err );

	return err;
}


static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
	MP4Err err;
	u32 entries;
	u32* p;
	MP4SampleSizeAtomPtr self = (MP4SampleSizeAtomPtr) s;
	
	err = MP4NoErr;
	if ( self == NULL )	BAILWITHERROR( MP4BadParamErr )
	err = self->super->createFromInputStream( s, proto, (char*) inputStream ); if ( err ) goto bail;
	
	GET32( sampleSize );
	GET32( sampleCount );
	
	if ( self->sampleSize == 0 )
	{
		self->sizes = (u32 *) calloc( self->sampleCount, sizeof(u32) );
		TESTMALLOC( self->sizes )
		for ( entries = 0, p = self->sizes; entries < self->sampleCount; entries++, p++ )
		{
			u32 size;
			GET32_V( size );
			*p = size;
		}
	}
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4CreateSampleSizeAtom( MP4SampleSizeAtomPtr *outAtom )
{
	MP4Err err;
	MP4SampleSizeAtomPtr self;
	
	self = (MP4SampleSizeAtomPtr) calloc( 1, sizeof(MP4SampleSizeAtom) );
	TESTMALLOC( self )

	err = MP4CreateFullAtom( (MP4AtomPtr) self );
	if ( err ) goto bail;
	self->type = MP4SampleSizeAtomType;
	self->name                = "sample size";
	self->createFromInputStream = (cisfunc) createFromInputStream;
	self->destroy             = destroy;
	self->getSampleSize       = getSampleSize;
	self->getSampleSizeAndOffset = getSampleSizeAndOffset;
	self->calculateSize         = calculateSize;
	self->serialize             = serialize;
	self->addSamples = addSamples;
	*outAtom = self;
bail:
	TEST_RETURN( err );

	return err;
}
