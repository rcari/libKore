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
 *    Original: http://www.networkdls.com/Software.Asp?Review=22
 */

#pragma once

#include <KoreExport.hpp>
#include <Macros.hpp>
#include <Types.hpp>

namespace Kore { namespace util {

/*!
 * @class CRC32
 * @brief This class calculates a CRC32 (Cyclic Redundancy Check or Polynomial Code Checksum) to detect accidental changes to raw data.
 */
class KoreExport CRC32 {
private:
	CRC32();
	void initialize();
	void partialCRC(kulong *ulCRC, const kuchar *sData, kulong ulDataLength) const;
	kulong fullCRC(const kuchar *sData, kulong ulDataLength) const;
	kulong reflect(kulong ulReflect, const kchar cChar) const;

private:
	kulong ulTable[256]; // CRC lookup table array.
	static CRC32* _Instance;
	static const CRC32* Instance();

public:
	/*!
	 * @brief Returns the hash string (CRC) for the data.
	 * @param[in] data Data to compute.
	 */
	static khash HashString(const kchar* data);
};

}}

