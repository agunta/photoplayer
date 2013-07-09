/*
	$Id: MacFileMappingObject.h,v 1.1 1999/01/30 00:42:39 mc Exp $
*/
#ifndef INCLUDED_MAC2FILEMAPPING_OBJECT_H
#define INCLUDED_MAC2FILEMAPPING_OBJECT_H

#include "FileMappingObject.h"

typedef struct MacFileMappingObjectRecord
{
	COMMON_FILEMAPPING_OBJECT_FIELDS
	char *fileName;
	int fd;
} MacFileMappingObjectRecord, *MacFileMappingObject;

MP4Err MP4CreateMacFileMappingObject( char *filename, FileMappingObject *outObject );

#endif
