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

#include <serialization/Loader.hpp>
using namespace Kore::serialization;

Loader::Loader(QIODevice* device)
{
	memset(&_header, 0x00, sizeof(FileHeader));
	device->read(reinterpret_cast<char*>(&_header), sizeof(FileHeader));

	_header.signature = qFromLittleEndian(_header.signature);
	_header.version = qFromLittleEndian(_header.version);
	_header.QtDataStreamVersion = qFromLittleEndian(_header.QtDataStreamVersion);
	_header.subProfileSignature = qFromLittleEndian(_header.subProfileSignature);
	_header.subProfileVersion = qFromLittleEndian(_header.subProfileVersion);
	_header.metaDataSize = qFromLittleEndian(_header.metaDataSize);
}

const BlockInflater* Loader::inflater() const
{
	switch(_header.version)
	{
	case Version1_0:
		return &_koreVersion1_0;
	default:
		return K_NULL;
	}
}

kbool Loader::isValid() const
{
	return _header.signature == K_FOURCC('K','O','R','E');
}

const FileHeader& Loader::header() const
{
	return _header;
}
