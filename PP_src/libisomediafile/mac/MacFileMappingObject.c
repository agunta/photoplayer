/*
	$Id: MacFileMappingObject.c,v 1.8 1999/11/19 07:50:48 mc Exp $
*/
#include "MacFileMappingObject.h"
#include <string.h>
#include <TextUtils.h>
#include <Files.h>

static MP4Err doClose( struct FileMappingObjectRecord *s );
static MP4Err doOpen( struct FileMappingObjectRecord *s, const char *filename );
static MP4Err isYourFile( struct FileMappingObjectRecord *s, const char *filename, u32 *outSameFile );

static MP4Err  destroy( struct FileMappingObjectRecord *s )
{
	MP4Err err;
	MacFileMappingObject self = (MacFileMappingObject) s;
	
	err = MP4NoErr;
	if ( self->data )
	{
		err = doClose( s ); if (err) goto bail;
	}
	if ( self->parent )
	{
	    free( self->parent );
	}
	free( s );
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err doOpen( struct FileMappingObjectRecord *s, const char *pathName )
{
	MP4Err err;
	FSSpec spec;
	Str255 fname;
	long count;
	short refNum;
	char* separator;

	MacFileMappingObject self = (MacFileMappingObject) s;
	err = MP4NoErr;
	strcpy( (char*) fname, pathName );
	c2pstr( (char*) fname );
	err = FSMakeFSSpec(0, 0, fname, &spec );
	if ( err )
		BAILWITHERROR( MP4IOErr )
	if ( self->data )
		doClose( s );
	
	err = FSpOpenDF( &spec,  fsCurPerm, &refNum );
	if ( err )
		BAILWITHERROR( MP4FileNotFoundErr )
	self->fd = refNum;
	self->fileName = (char *) calloc( 1, strlen(pathName)+1 );
	if ( self->fileName == NULL )
		BAILWITHERROR( MP4NoMemoryErr )
	strcpy( self->fileName, pathName );
	
	err = GetEOF( self->fd, (long *) &self->size );
	self->data = (char *) malloc( self->size );
	if ( self->data == NULL )
		BAILWITHERROR( MP4IOErr )
	count = self->size;
	err = FSRead(self->fd, &count, self->data );
	
	/* make parent */
	if ( (separator = strrchr(pathName, '\\')) || (separator = strrchr(pathName, '/')) )
	{
	    int len = separator - pathName;
	    self->parent = (char *) calloc( 1, len + 2 );
	    TESTMALLOC( self->parent );
	    strncpy( self->parent, pathName, len );
	    strcat( self->parent, "/" );
	}
bail:
	TEST_RETURN( err );

	return err;
}

static MP4Err doClose( struct FileMappingObjectRecord *s )
{
	MP4Err err;
	MacFileMappingObject self = (MacFileMappingObject) s;
	
	err = MP4NoErr;
	
	if ( self->data )
		free( self->data );
	self->data = NULL;
	if ( self->fileName )
		free( self->fileName );
	FSClose( self->fd );
	return err;
}

static MP4Err isYourFile( struct FileMappingObjectRecord *s, const char *filename, u32 *outSameFile )
{
	MP4Err err;
	MacFileMappingObject self = (MacFileMappingObject) s;
	int result;
	err = MP4NoErr;
#if defined(_MSC_VER)
	result = _stricmp( self->fileName, filename );
#else
	result = strcmp( self->fileName, filename );
#endif
	*outSameFile = result ? 0 : 1;
	return err;
}


MP4Err MP4CreateMacFileMappingObject( char *urlString, FileMappingObject *outObject )
{
	MP4Err err;
	MacFileMappingObject self;
	
	err = MP4NoErr;
	self = (MacFileMappingObject) calloc( 1, sizeof(MacFileMappingObjectRecord) );
	TESTMALLOC( self );
	self->destroy    = destroy;
	self->open       = doOpen;
	self->close      = doClose;
	self->isYourFile = isYourFile;
	err = doOpen( (FileMappingObject) self, urlString ); if (err) goto bail;
	*outObject = (FileMappingObject) self;
bail:
	TEST_RETURN( err );

	return err;
}

MP4Err MP4CreateFileMappingObject( char *urlString, FileMappingObject *outObject )
{
    char *pathname = urlString;
    if ( (strncmp(urlString, "file://", 7) == 0) || (strncmp(urlString, "file|//", 7) == 0) )
    {
        pathname += 7;
    }
	return MP4CreateMacFileMappingObject( pathname, outObject );
}

MP4Err MP4AssertFileExists( char *pathName )
{
	MP4Err err;
	FSSpec spec;
	Str255 fname;
	short refNum;

	err = MP4NoErr;
	strncpy( (char*) fname, pathName, 255 );
	c2pstr( (char*) fname );
	err = FSMakeFSSpec(0, 0, fname, &spec );
	if ( err )
		BAILWITHERROR( MP4IOErr )
	err = FSpOpenDF( &spec,  fsCurPerm, &refNum );
	if ( err )
		BAILWITHERROR( MP4FileNotFoundErr )
	FSClose( refNum );
bail:
    return err;
}
