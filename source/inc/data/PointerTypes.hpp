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

#include <QtCore/QMetaType>

#include <KoreExport.hpp>
#include <Types.hpp>

namespace Kore { namespace data {

class Block;

struct KoreExport Pointer
{
    inline Pointer() : data( K_NULL ) {}
    inline Pointer( kvoid* ptr ) : data( ptr ) {}

    kvoid* data;
};

struct KoreExport BlockPointer
{
    inline BlockPointer() : block( K_NULL ) {}
    inline BlockPointer( Block* block ) : block( block ) {}

    Block* block;
};

struct KoreExport ConstBlockPointer
{
    inline ConstBlockPointer() : block( K_NULL ) {}
    inline ConstBlockPointer( const Block* block ) : block( block ) {}

    const Block* block;
};

}}

Q_DECLARE_METATYPE( Kore::data::Pointer )
Q_DECLARE_METATYPE( Kore::data::BlockPointer )
Q_DECLARE_METATYPE( Kore::data::ConstBlockPointer )
