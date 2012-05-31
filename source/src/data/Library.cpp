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
 *	DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <data/Library.hpp>
using namespace Kore::data;

#define K_BLOCK_TYPE Kore::data::Library
#include <BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON("kore/images/icons/library.png")
	K_BLOCK_VIRTUAL
	K_BLOCK_PROPERTY_METHOD( Kore::data::Library::BlockProperty )
K_BLOCK_END

Library::Library(kuint extraFlags)
{
	addFlag(Browsable);
	addFlag(extraFlags);
}

bool Library::destroy()
{
	// This library is being deleted, this flag will be checked when destroying the children.
	addFlag(IsBeingDeleted);

	clear(); // Destroy all child blocks.

	return Block::destroy();
}

void Library::serialize(kbool b)
{
	b ? addFlag(Serializable) : removeFlag(Serializable);
}

void Library::clear()
{
	QMutexLocker locker(&_mutex);

	if(isEmpty())
	{
		return; // Nothing to clear !
	}

	emit clearing();

	for(int i = 0; i < this->size(); i++)
	{
		_blocks.at(i)->destroy();
	}

	_blocks.clear();

	emit cleared();
}

kint Library::totalSize() const
{
	kint size = _blocks.size();
	for(kint i = 0; i < _blocks.size(); i++)
	{
		if(_blocks.at(i)->isLibrary())
		{
			size += static_cast<Library*>(_blocks.at(i))->totalSize();
		}
	}
	return size;
}

kbool Library::isEmpty() const
{
	return _blocks.isEmpty();
}

void Library::optimize()
{
	QMutexLocker locker(&_mutex);
	QList<Block*> list;
	list.reserve(_blocks.size());
	list.append(_blocks);
	_blocks = list;
}

void Library::optimizeTree()
{
	// Optimize this library
	optimize();
	// Optimize the tree
	for(kint i = 0; i < _blocks.size(); i++)
	{
		Block* b = _blocks.at(i);
		b->isLibrary()
			?	static_cast<Library*>(b)->optimizeTree()
			:	b->optimize();
	}
}

void Library::freeze(kbool frozen)
{
	QMutexLocker locker(&_mutex);
	if(_frozen != frozen)
	{
		_frozen = frozen;
		emit freezeChanged(_frozen);
		for(kint i = 0; i < _blocks.size(); i++)
		{
			_blocks.at(i)->freeze(_frozen);
		}
	}
}

kbool Library::acceptsBlock(Block* b) const
{
	Q_UNUSED(b);
	return false;
}

void Library::addBlock(Block* b)
{
	QMutexLocker locker(&_mutex);

	K_ASSERT( !_blocks.contains(b) )

	const kint index = _blocks.size();
	emit addingBlock(index);
	_blocks.append(b);
	// Set the new block index first
	b->index(index);
	// Set its library !
	b->library(this);
	emit blockAdded(index);
}

void Library::removeBlock(Block* b)
{
	QMutexLocker locker(&_mutex);

	K_ASSERT( _blocks.contains(b) )

	const kint index = _blocks.indexOf(b); // We cannot rely on the b->index() !
	emit removingBlock(index);
	_blocks.removeAt(index);
	indexBlocks(index); // Reindex the blocks...
	b->index(-1); // Important to do that first, to indicate to the block that WE are removing it.
	b->library(K_NULL);
	emit blockRemoved(index);
}

void Library::insertBlock(Block* b, kint index)
{
	QMutexLocker locker(&_mutex);

	K_ASSERT( !_blocks.contains(b) )

	emit insertingBlock(index);
	_blocks.insert(index, b);
	// Set the new block index first
	b->index(index);
	// Set its library !
	b->library(this);
	indexBlocks(index);
	emit blockInserted(index);
}

void Library::swapBlocks(Block* a, Block* b)
{
	QMutexLocker locker(&_mutex);

	K_ASSERT( _blocks.contains(a) && _blocks.contains(b) )

	emit swappingBlocks(a->index(), b->index());
	// Swap in the list
	_blocks.swap(a->index(), b->index());
	// Swap the respective indexes
	int index = a->index();
	a->index(b->index());
	b->index(index);
	emit blocksSwapped(a->index(), b->index());
}

void Library::moveBlock(Block* block, kint to)
{
	QMutexLocker locker(&_mutex);

	kint from = block->index() == -1 ? _blocks.indexOf(block) : block->index();
	K_ASSERT( _blocks.at(from) == block )

	if(from != to)
	{
		emit movingBlock(from, to);
		_blocks.move(block->index(), to);
		indexBlocks( K_MIN(from,to) );
		emit blockMoved(from, to);
	}
}

void Library::indexBlocks(kint startOffset)
{
	QMutexLocker locker(&_mutex);
	for( ; startOffset < _blocks.size(); startOffset++) {
		_blocks.at(startOffset)->index(startOffset);
	}
}

void Library::notifyInserted()
{
	Block::notifyInserted();
	// Recursive calls to alert all observers of the subtree.
	for(int i = 0; i < _blocks.size(); i++)
	{
		_blocks.at(i)->notifyInserted();
	}
}

void Library::notifyRemoved()
{
	Block::notifyRemoved();
	// Recursive calls to alert all observers of the subtree.
	for(int i = 0; i < _blocks.size(); i++)
	{
		_blocks.at(i)->notifyRemoved();
	}
}

void Library::notifyDeleted()
{
	Block::notifyDeleted();
	// Recursive calls to alert all observers of the subtree.
	if(checkFlag(IsBeingDeleted))
	{
		return; // No further processing, the child blocks will be deleted for real and the signal will be emitted !
	}

	for(int i = 0; i < _blocks.size(); i++)
	{
		_blocks.at(i)->notifyDeleted();
	}
}

kbool Library::isBrowsable() const
{
	return checkFlag(Browsable);
}

QVariant Library::BlockProperty(kint property)
{
	switch(property)
	{
	case BlockTypeName:
		return tr("Basic data library");
	default:
		return Block::DefaultBlockProperty(property);
	}
}
