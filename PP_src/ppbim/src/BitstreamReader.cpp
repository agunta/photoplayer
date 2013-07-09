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

/**This work is based in part on file 'bitstream.cpp' from MPEG-7 reference software.
   The copyright notice of 'bitstream.cpp" is the following.
*/
/***********************************************************************
This software module was originally developed by Siemens AG and Munich 
University of Technology in the course of development of the MPEG-7 
Systems (ISO/IEC 15938-1) standard. 

This software module is an implementation of a part of one or more 
MPEG-7 Systems (ISO/IEC 15938-1) tools as specified by the 
MPEG-7 Systems (ISO/IEC 15938-1) standard. 

ISO/IEC gives users of the MPEG-7 Systems (ISO/IEC 15938-1) free license
to this software module or modifications thereof for use in hardware or
software products claiming conformance to the MPEG-7 Systems 
(ISO/IEC 15938-1). 

Those intending to use this software module in hardware or software 
products are advised that its use may infringe existing patents.

The original developer of this software module and his/her company, the
subsequent editors and their companies, and ISO/IEC have no liability
for use of this software module or modifications thereof in an
implementation. 

Copyright is not released for non MPEG-7 Systems (ISO/IEC 15938-1) 
conforming products. 

The organizations named above retain full right to use the code for 
their own purpose, assign or donate the code to a third party and to 
inhibit third parties from using the code for non MPEG-7 Systems 
(ISO/IEC 15938-1) conforming products. 

This copyright notice must be included in all copies or derivative works.


************************************************************************/

#include "collectionbim.h"

BitstreamReader::BitstreamReader(unsigned char *buf, int bufsize)
{
	m_pBuffer = buf;
	m_nBufferSize = bufsize;
	m_nBufferBitSize = m_nBufferSize * 8;
	m_nBitPointer = 0;
}

BitstreamReader::~BitstreamReader()
{
}

void BitstreamReader::Reset()
{
	m_nBitPointer = 0;
}


bool BitstreamReader::ReadBool(bool *bdata)
{
	int num;
	bool result = ReadBits(&num, 1);
	if(num == 0)
	{
		*bdata = false;
	}
	else
	{
		*bdata = true;
	}
	return result;
}

bool BitstreamReader::ReadBits(int *value, unsigned int bits)
{
	int i,tvalue=0;
	int address;
	unsigned char offset;
	if(!m_pBuffer || (m_nBitPointer + (int)bits) > m_nBufferBitSize)
	{
		BimXMLUtil::ReportError("Bitstream buffer error");
		return false;
	}

/*
#ifdef WRITE_DEBUG_INFO
	report<<"bitpointer: "<<bitpointer;
#endif
*/
	for(i=0;i<(int)bits;i++){
		address=m_nBitPointer/8;
		offset=1<<(7-m_nBitPointer%8);
		if(m_pBuffer[address]&offset) 
			tvalue+=(1<<(bits-i-1));
		m_nBitPointer++;
	}
/*
#ifdef WRITE_DEBUG_INFO
	report<<", take "<<num_bits<<" bits, value="<<value<<endl<<flush;
#endif
*/
	*value = tvalue; 
	return true;
}

bool BitstreamReader::ReadVarLenInt5(unsigned int *value)
{
	int i;
	int address;
	unsigned char mask;
	int length=1; // minimum length of integer is one chunk of 4 bits
	unsigned int tvalue=0;

/*
#ifdef WRITE_DEBUG_INFO
	report<<"bitpointer: "<<bitpointer;
#endif
*/

	for(;;){
		address=m_nBitPointer/8;
		if(address > m_nBufferSize)
		{
			BimXMLUtil::ReportError("Bitstream buffer error");
			return false;
		}
		mask=1<<(7-m_nBitPointer%8);
		m_nBitPointer++;
		if(m_pBuffer[address]&mask) length++;
		else break;	//zero in bitstream indicates end of "Ext bits" field
	}

	length=length*4; //convert number of chunks to number of bits
	for(i=0;i<length;i++){
		address=m_nBitPointer/8;
		if(address > m_nBufferSize)
		{
			BimXMLUtil::ReportError("Bitstream buffer error");
			return false;
		}
		mask=1<<(7-m_nBitPointer%8);
		m_nBitPointer++;
		if(m_pBuffer[address]&mask) tvalue+=1<<(length-1-i);
	}
/*
#ifdef WRITE_DEBUG_INFO
	report<<" read_var_len_int, value: "<<value<<endl<<flush;
#endif
*/
	*value = tvalue; 
	return true;
}

bool BitstreamReader::ReadIntegerVLI5(int *value)
{
	int nNegative;
	unsigned int val;
	if(!ReadBits(&nNegative, 1)) return false;
	nNegative = (nNegative == 0) ? 1 : -1;
	if(!ReadVarLenInt5(&val)) return false;
	*value = nNegative * (int)val;
	return true;
}


bool BitstreamReader::ReadDouble(double *value)
{
	bool bret = true;
	unsigned char * cp=(unsigned char*)value;
	for(int i = 7; i >= 0; i--)
	{
		int nVal;
		bret = ReadBits(&nVal, 8);
		if(!bret) break;
		*(cp + i) = (unsigned char)nVal;
	}
	return bret;
}
