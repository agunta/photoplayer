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

BitstreamWriter::BitstreamWriter(int size,  unsigned char *buf)
{
	if(size <= 0 || buf == NULL)
	{
		m_nBufferBitSize = 0;
	}
	else
	{
		m_pBuffer = buf;
		m_nBufferSize = size;
	}
	m_nBufferBitSize = 8 * m_nBufferSize;
	m_nBitPointer = 0;
}

BitstreamWriter::~BitstreamWriter()
{
}


bool BitstreamWriter::WriteBool(bool bdata)
{
	int num = bdata ? 1 : 0;
	return WriteBits(num, 1);
}

//void bitstream::write_bits(int value,unsigned int bits){ 
bool BitstreamWriter::WriteBits(int value, unsigned int bits)
{
	unsigned int i,filter=1<<(bits-1);
	unsigned char offset;
	int address;
	//MSB is written first in byte

	if(!m_pBuffer || (m_nBitPointer + (int)bits) > m_nBufferBitSize)
	{
		BimXMLUtil::ReportError("Bitstream buffer error");
		return false;
	}

	if(value<0){
/*
#ifdef WRITE_DEBUG_INFO
		report<<"write value<0!!!"<<endl<<flush;
		report<<"abort..."<<endl<<flush;
#endif
*/
	}

/*
#ifdef WRITE_DEBUG_INFO
	report<<"bitpointer: "<<m_nBitPointer;
	report<<", write_bits("<<value<<","<<bits<<")"<<endl<<flush;
#endif
*/

	for(i=0;i<bits;i++){
		address=m_nBitPointer/8;
		offset=(m_nBitPointer%8);
		m_nBitPointer++;
		if(!offset) m_pBuffer[address]=0;
		if(value&filter){
			m_pBuffer[address]+=1<<(7-offset);
		}
		filter>>=1;	
	}	 
	return true;
}

bool BitstreamWriter::WriteVarLenInt5(unsigned int value)
{
	int num_chunks=0;
	unsigned int test_length=value;
	int address;
	unsigned char offset;
	unsigned int filter;
	int i;
	//detect number of 4 bit chunks, necessary to represent the integer
	while(test_length){
		num_chunks++;
		test_length>>=4;
	}
	if(num_chunks == 0) num_chunks = 1; // KW: bug if the value is 0
	if(!m_pBuffer || (m_nBitPointer + 4 * num_chunks) > m_nBufferBitSize)
	{
		BimXMLUtil::ReportError("Bitstream buffer error");
		return false;
	}

	//write Ext bits field
	for(i=0;i<num_chunks;i++){
		address=m_nBitPointer/8;
		offset=m_nBitPointer%8;
		m_nBitPointer++;

		if(!offset) m_pBuffer[address]=0; //initialize new byte with zero
		if(i!=num_chunks-1) m_pBuffer[address]+=1<<(7-offset);//last bit of Ext bits is null!
	}

	//write integer itself, MSB first
	filter=1<<(num_chunks*4-1);
	for(i=0;i<num_chunks*4;i++){
		address=m_nBitPointer/8;
		offset=m_nBitPointer%8;
		m_nBitPointer++;

		if(!offset) m_pBuffer[address]=0; //initialize new byte with zero
		if(value&filter) m_pBuffer[address]+=1<<(7-offset);
		filter>>=1;
	} 
	return true;
}

bool BitstreamWriter::WriteIntegerVLI5(int value)
{
	unsigned int val = (value < 0) ? -value : value;
	int nNegative = (value < 0) ? 1 : 0;
	if(!WriteBits(nNegative, 1)) return false;
	return WriteVarLenInt5(val);
}

bool BitstreamWriter::WriteDouble(double value)
{
	unsigned char * cp=(unsigned char*)(&value);
	for(int i = 7; i >= 0; i--)
	{
		int nVal = (int)(*(cp + i));
		if(!WriteBits(nVal, 8)) return false;
	}
	return true;
}
