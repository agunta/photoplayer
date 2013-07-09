/*
	$Id: MP4OSMacros.h,v 1.12 2000/01/22 07:13:21 mc Exp $
*/

#ifndef INCLUDED_MP4OSMACROS_H
#define INCLUDED_MP4OSMACROS_H

#define assert(condition) ((condition) ? ((void) 0) : (void) Debugger())

#ifndef TEST_RETURN
#define TEST_RETURN(err)
/*#define TEST_RETURN(err) assert((err)==0)*/
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRAGMA_EXPORT 1

#if defined(__MWERKS__)
#define MP4_EXTERN(v) __declspec(export) extern v
#else
#define MP4_EXTERN(v) extern v
#endif

#if defined(_MSC_VER) && !defined(__MWERKS__)
typedef _int64 u64;
typedef _int64 s64;
#else
typedef long long u64;
typedef long long s64;
#endif
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef int s32;
typedef short s16;
typedef char s8;

#define MP4_FOUR_CHAR_CODE( a, b, c, d ) (((a)<<24)|((b)<<16)|((c)<<8)|(d))

#define U64_PRINTF_FMT "%lld"

#define PATHNAMES_NEED_MUNGING

#define HAS_PRAGMA_UNUSED

#endif
