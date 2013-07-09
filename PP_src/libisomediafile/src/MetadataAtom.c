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
	$Id: MetadataAtom.c,v 1.0 2006/07/20 17:50:27 KW $
*/
#include "MP4Atoms.h"
#include "MP4Descriptors.h"
#include <stdlib.h>


static void destroy( MP4AtomPtr s )
{
	MP4Err err;
	MP4MetadataAtomPtr self;
	u32 i;
	err = MP4NoErr;
	self = (MP4MetadataAtomPtr) s;
	if ( self == NULL )
		BAILWITHERROR( MP4BadParamErr )
	DESTROY_ATOM_LIST
	
	if ( self->super )
		self->super->destroy( s );
  bail:
	TEST_RETURN( err );

	return;
}


static MP4Err serialize( struct MP4Atom* s, char* buffer )
{
   MP4Err err;
   MP4MetadataAtomPtr self = (MP4MetadataAtomPtr) s;
   err = MP4NoErr;
	
   err = MP4SerializeCommonFullAtomFields( (MP4FullAtom*) s, buffer ); if (err) goto bail;
   buffer += self->bytesWritten;
   SERIALIZE_ATOM_LIST( atomList );
   assert( self->bytesWritten == self->size );
  bail:
   TEST_RETURN( err );

   return err;
}


static MP4Err calculateSize( struct MP4Atom* s )
{
   MP4Err err;
   MP4MetadataAtomPtr self = (MP4MetadataAtomPtr) s;
   err = MP4NoErr;
	
   err = MP4CalculateFullAtomFieldSize( (MP4FullAtom*) s ); if (err) goto bail;
   ADD_ATOM_LIST_SIZE( atomList );
  bail:
   TEST_RETURN( err );

   return err;
}

static MP4Err addAtom( MP4MetadataAtomPtr self, MP4AtomPtr atom )
{
   MP4Err err;
   err = MP4NoErr;
   if ( atom == NULL )
      BAILWITHERROR( MP4BadParamErr );
   err = MP4AddListEntry( atom, self->atomList ); if (err) goto bail;
   switch( atom->type )
   {
      /*case MP4MediaHeaderAtomType:
		if ( self->mediaHeader )
	  	  BAILWITHERROR( MP4BadDataErr )
		self->mediaHeader = atom;
	 break;*/
			
      case MP4HandlerAtomType:
		if ( self->handler )
			BAILWITHERROR( MP4BadDataErr )
		self->handler = atom;
	 break;
			
	  case MP4BinaryXMLAtomType:
		if ( self->bim )
			BAILWITHERROR( MP4BadDataErr )
		self->bim = atom;
	 break;

	  /*case MP4DataInformationAtomType:
		  BAILWITHERROR( MP4NotImplementedErr )
	 break;

	  case MP4DataInformationAtomType:
		  BAILWITHERROR( MP4NotImplementedErr )
	 break;

	  case MP4PrimaryItemAtomType:
		  BAILWITHERROR( MP4NotImplementedErr )
	 break;

	  case MP4ItemLocationAtomType:
		  BAILWITHERROR( MP4NotImplementedErr )
	 break;

	  case MP4ItemProtectionAtomType:
		  BAILWITHERROR( MP4NotImplementedErr )
	 break;

	  case MP4ItemInfoAtomType:
		  BAILWITHERROR( MP4NotImplementedErr )
	 break;*/

   }
  bail:
   TEST_RETURN( err );

   return err;
}

static MP4Err setupNew( struct MP4MetadataAtom *self, u32 metaType, MP4Handle data )
{
   MP4Err MP4CreateHandlerAtom( MP4HandlerAtomPtr *outAtom );
   MP4Err MP4CreateBinaryXMLAtom( MP4BinaryXMLAtomPtr *outAtom );

   char name[ 8 ];
   MP4Err err;
   MP4HandlerAtomPtr hdlr;
	
   err = MP4NoErr;
   err = MP4CreateHandlerAtom( &hdlr ); if (err) goto bail;

   hdlr->handlerType = metaType;
   MP4TypeToString( metaType, name );
   hdlr->nameLength = 5;
   hdlr->nameUTF8 = calloc( 1, hdlr->nameLength );
   TESTMALLOC( hdlr->nameUTF8 )
   memcpy( hdlr->nameUTF8, name, hdlr->nameLength );
	
   err = addAtom( self, (MP4AtomPtr) hdlr ); if (err) goto bail;
   if(data != NULL)
   {
	   MP4BinaryXMLAtomPtr bimp;
	   err = MP4CreateBinaryXMLAtom( &bimp ); if (err) goto bail;
	   err = bimp->addData( bimp, data); if (err) goto bail;
	   err = addAtom( self, (MP4AtomPtr) bimp ); if (err) goto bail;
   }
	
bail:
   TEST_RETURN( err );

   return err;
}

static MP4Err createFromInputStream( MP4AtomPtr s, MP4AtomPtr proto, MP4InputStreamPtr inputStream )
{
   PARSE_ATOM_LIST(MP4MetadataAtom)
  bail:
   TEST_RETURN( err );

   return err;
}

MP4Err MP4CreateMetadataAtom( MP4MetadataAtomPtr *outAtom )
{
   MP4Err err;
   MP4MetadataAtomPtr self;
	
   self = (MP4MetadataAtomPtr) calloc( 1, sizeof(MP4MetadataAtom) );
   TESTMALLOC( self )

   err = MP4CreateFullAtom( (MP4AtomPtr) self );
   if ( err ) goto bail;
   self->type = MP4MetadataAtomType;
   self->name                = "metadata";
   self->createFromInputStream = (cisfunc) createFromInputStream;
   self->destroy             = destroy;
   err = MP4MakeLinkedList( &self->atomList ); if (err) goto bail;
   self->calculateSize         = calculateSize;
   self->serialize             = serialize;
   self->addAtom = addAtom;
   //self->calculateDuration = calculateDuration;
   self->setupNew = setupNew;
   //self->mdatMoved = mdatMoved;
   //self->addSampleReference = addSampleReference;
   *outAtom = self;
  bail:
   TEST_RETURN( err );

   return err;
}
