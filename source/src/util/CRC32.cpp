/*
 * 	Copyright (c) 2010-2011, Romuald CARI
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *		* Redistributions of source code must retain the above copyright
 *		  notice, this list of conditions and the following disclaimer.
 *		* Redistributions in binary form must reproduce the above copyright
 *		  notice, this list of conditions and the following disclaimer in the
 *		  documentation and/or other materials provided with the distribution.
 *		* Neither the name of the <organization> nor the
 *		  names of its contributors may be used to endorse or promote products
 *		  derived from this software without specific prior written permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL Romuald CARI BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <util/CRC32.hpp>
using namespace Kore::util;

#include <string.h>

#include <QtCore/QString>

CRC32::CRC32() {
    initialize();
}

/*
 * This function initializes "CRC Lookup Table". You only need to call it once
 * to initalize the table before using any of the other CRC32 calculation
 * functions.
*/
void CRC32::initialize()
{
    // 0x04C11DB7 is the official polynomial used by PKZip, WinZip and Ethernet.
    unsigned long ulPolynomial = 0x04C11DB7;

    // memset( &this->ulTable, 0, sizeof( this->ulTable ) );

    // 256 values representing ASCII character codes.
    for(int iCodes = 0; iCodes <= 0xFF; iCodes++)
    {
        this->ulTable[iCodes] = this->reflect(iCodes, 8) << 24;

        for(int iPos = 0; iPos < 8; iPos++)
        {
            this->ulTable[iCodes] = (this->ulTable[iCodes] << 1)
                ^ ((this->ulTable[iCodes] & (1 << 31)) ? ulPolynomial : 0);
        }

        this->ulTable[iCodes] = this->reflect(this->ulTable[iCodes], 32);
    }
}

/*
    Reflection is a requirement for the official CRC-32 standard.
    You can create CRCs without it, but they won't conform to the standard.
*/
kulong CRC32::reflect(kulong ulReflect, const kchar cChar) const
{
    unsigned long ulValue = 0;

    // Swap bit 0 for bit 7, bit 1 For bit 6, etc....
    for(int iPos = 1; iPos < (cChar + 1); iPos++)
    {
        if(ulReflect & 1)
        {
            ulValue |= (1 << (cChar - iPos));
        }
        ulReflect >>= 1;
    }

    return ulValue;
}

/*
    Calculates the CRC32 by looping through each of the bytes in sData.

    Note: For Example usage example, see FileCRC().
*/
void CRC32::partialCRC(kulong *ulCRC, const kuchar *sData, kulong ulDataLength) const
{
    while(ulDataLength--)
    {
        //If your compiler complains about the following line, try changing each
        // occurrence of *ulCRC with "((unsigned long)*ulCRC)" or "*(unsigned long *)ulCRC".
        *(unsigned long *)ulCRC = ((*(unsigned long *)ulCRC) >> 8)
                ^ this->ulTable[((*(unsigned long *)ulCRC) & 0xFF) ^ *sData++];
    }
}

/*
    Returns the calculated CRC32 for the given string.
*/

kulong CRC32::fullCRC(const kuchar *sData, kulong ulDataLength) const
{
    unsigned long ulCRC = 0xffffffff; //Initialize the CRC.
    partialCRC(&ulCRC, sData, ulDataLength);
    return (ulCRC ^ 0xffffffff); //Finalize the CRC and return.
}

khash CRC32::HashString(const kchar* data) {
    //qDebug("Hashing String for class %s", data);
    khash hash = Instance()->fullCRC( (kuchar*)data, strlen(data) );
    return hash;
}

const CRC32* CRC32::Instance() {
    if(_Instance == K_NULL) {
        _Instance = new CRC32();
    }
    return _Instance;
}

CRC32* CRC32::_Instance = K_NULL;
