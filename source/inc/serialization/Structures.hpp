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

#pragma once

#include <KoreExport.hpp>

#include <QtCore/QDataStream>
#include <QtCore/QtEndian>

namespace Kore { namespace serialization {

enum KoreStreamVersions
{
	Initial = 0x1,
	Version1_0 = Initial,
	Current = Version1_0,
};

struct KoreExport FileHeader
{
	FileHeader()
	{
		signature = qToLittleEndian( K_FOURCC('K', 'O', 'R', 'E') );
		version = qToLittleEndian( Current );
		QDataStream stream;
		QtDataStreamVersion = qToLittleEndian( stream.version() ); // Current / latest Qt version ?
		subProfileSignature = 0x0;
		subProfileVersion = 0x0;
		metaDataSize = 0;
	}
	kuint signature;
	kuint version;
	kuint QtDataStreamVersion;
	kuint subProfileSignature;
	kuint subProfileVersion;
	kuint metaDataSize;
};

struct KoreExport BlockHeader
{
	kuint size;
	khash blockType;
	kuint propertiesNb;
	kuint childrenNb;
};

}}
