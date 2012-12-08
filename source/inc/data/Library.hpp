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

#include <data/Block.hpp>

#include <QtCore/QList>
#include <QtCore/QString>

namespace Kore { namespace data {

class KoreExport Library : public Block {

    Q_OBJECT
    K_BLOCK

public:
    Library( kuint extraFlags = 0 );

    virtual bool destroy();

    void clear();

    template< typename T >
    inline const T* at( kint i ) const
        { return static_cast< T* >( _blocks.at( i ) ); }
    inline const Block* at( kint i ) const
        { return _blocks.at( i ); }

    template< typename T >
    inline T* at( kint i ) { return static_cast< T* >( _blocks.at( i ) ); }
    inline Block* at( kint i ) { return _blocks.at( i ); }

    inline kint size() const { return _blocks.size(); }
    kint totalSize() const;
    inline kbool isEmpty() const { return _blocks.empty(); }

    template< typename T >
    QList< T* > findChildren( int maxDepth = -1 );

    template< typename T >
    QList< const T* > findChildrenConst( int maxDepth = -1 ) const;

    virtual void optimize();
    void optimizeTree();

    virtual kbool acceptsBlock( Block* b ) const;
    virtual void addBlock( Block* b );
    virtual void removeBlock( Block* b );
    virtual void insertBlock( Block* b, kint index );
    virtual void swapBlocks( Block* a, Block* b );
    virtual void moveBlock( Block* block, kint to );

    kbool isBrowsable() const;
    virtual kbool isLibrary() const { return true; }

    static QVariant LibraryProperty( kint property );

protected:
    void indexBlocks( kint startOffset = 0 );

signals:
    void addingBlock( kint index );
    void blockAdded( kint index );
    void removingBlock( kint index );
    void blockRemoved( kint index );
    void swappingBlocks( kint index1, kint index2 );
    void blocksSwapped( kint index1, kint index2 );
    void movingBlock( kint from, kint to );
    void blockMoved( kint from, kint to );
    void clearing();
    void cleared();

private:
    QList< Block* > _blocks;
};

} /* namespace data */ } /* namespace Kore */

#include <src/data/Library.cxx>
