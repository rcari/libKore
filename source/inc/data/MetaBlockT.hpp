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

#pragma once

#include <KoreExport.hpp>

#include <plugin/Loadable.hpp>
#include "BlockFactory.hpp"

#include <QtCore/QAtomicInt>
#include <QtCore/QMetaClassInfo>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>

#include <QtCore/QMultiHash>
#include <QtCore/QVector>

#include "DefaultBlockAllocator.hpp"

namespace Kore { namespace data {

class BlockExtension;

template<typename BlockType, typename AllocatorType=DefaultBlockAllocator>
class KoreExport MetaBlockT : public Kore::plugin::Loadable
{
	Q_OBJECT

	friend class Block;
	friend class BlockExtension;
	friend class BlockFactory;

protected:
	MetaBlockT()
	:	_blockClassID(0)
	{
	}

	virtual void library(Kore::data::Library* lib)
	{
		Block::library(lib);

		if(hasParent())
		{
			Kore::KoreEngine::RegisterMetaBlock(this);
			createPropertiesCache(); // The metablock is fully built now!
		}
		else
		{
			clearExtensions();
			Kore::KoreEngine::UnregisterMetaBlock(this);
		}
	}

public:
	virtual bool canUnload() const
	{
		return _instancesCount == 0;
	}

	QString iconPath() const { return QString(); }

	inline BlockType* create() const
	{
		kvoid* chunk = _allocator.allocate(sizeof(BlockType));
		K_ASSERT( chunk );
		while(!_instancesCount.ref()) {} // Ref
		BlockType* block = new (chunk) BlockType;
		block->addFlag(Block::Allocated);
		return block;
	}

	inline void destroy(BlockType* block) const
	{
		while(!_instancesCount.deref()) {} // Unref
		block->~BlockType();
		_allocator.deallocate(block);
	}

	inline const QMetaObject* blockMetaObject() const { return &BlockType::staticMetaObject; }
	inline QString blockClassName() const { return BlockType::staticMetaObject.className(); }

	inline khash blockClassID() const
	{
		K_ASSERT( _blockClassID != K_NULL )
		return _blockClassID;
	}

	inline khash propertyID(kint property) const
	{
		K_ASSERT( !_propertiesHashes.isEmpty() )
		return _propertiesHashes.at(property);
	}
	inline kint propertyIndex(khash propertyHash) const
	{
		K_ASSERT( !_propertiesHashes.isEmpty() )
		return _propertiesHashes.indexOf(propertyHash);
	}

	//virtual void optimize();

	virtual QString blockIconPath() const = K_NULL;
	virtual QVariant blockProperty(kint property) const = K_NULL;
	QVariant blockSetting(const QString& setting, const QVariant& defaultValue) const;

protected:
	kbool registerBlockExtension(BlockExtension* extension) {}
	void unregisterBlockExtension(BlockExtension* extension) {}

private:
	void createClassID();
	void createPropertiesCache();
	void clearExtensions();
	inline void ref() {  }
	inline void deref() {  }

private:
	mutable AllocatorType _allocator;
	mutable QAtomicInt _instancesCount;

	khash _blockClassID;
	QVector<khash> _propertiesHashes;
};

}}
