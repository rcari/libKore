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

#include <data/Block.hpp>
#include <data/Library.hpp>
using namespace Kore::data;

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
//#include <QtGui/QIcon>

Block::Block()
:	_mutex(QMutex::Recursive),
 	_library(K_NULL),
	_flags(K_NULL),
 	_index(-1),
 	_factory(K_NULL),
 	_frozen(false)
{
}

Block::~Block()
{
	K_ASSERT( _library == K_NULL )
	K_ASSERT( checkFlag(IsBeingDeleted) )
	//qDebug("Destroying %s @ %p", qPrintable(blockName()), this);
}

bool Block::destroy()
{
	// This block is being deleted.
	addFlag(IsBeingDeleted);
	// Notify our listeners.
	notifyDeleted();

	// Cleanup the connection to the library
	if(_library)
	{
		// We have to remove ourselves from the library only if our library is not being deleted: we are the one being deleted.
		if(!_library->checkFlag(IsBeingDeleted))
		{
			// Immediate removal from the library tree.
			_library->removeBlock(this);
		}
		else
		{
			// Announce to our listeners that we have been removed (not really but as if !)
			notifyRemoved();
		}
		setParent(K_NULL); // We do not want Qt to take care of the deletion because of the factory and all.
		_library = K_NULL;
	}

	// Cleanup!!
	QCoreApplication::removePostedEvents(this);

	// Actually destroy this thing.
	if(_factory)
	{
		// If the block has a factory, it is in charge of releasing it.
		_factory->destroyBlock(this);
	}
	else if(checkFlag(System)) // The block does not have a factory but is a system block.
	{
		// The block does not have a factory but should be deleted anyway as it is a SYSTEM block.
		delete this; // Delete this straight up !
	}

	return true;
}

void Block::index(kint idx)
{
	QMutexLocker locker(&_mutex);

	if(_index != idx)
	{
		emit indexChanged(_index, idx);
		_index = idx;
	}
}

void Block::library(Library* lib)
{
	QMutexLocker locker(&_mutex);
	if(_library)
	{
		// We test whether the Block already belongs to a Library
		// we moreover test whether that Library is currently removing this Block
		// in that case, there is no need to perform further removal. (tricky one :P)
		if(_index != -1)
		{
			// Here the current block library is not aware that the block is about to change of owner.
			library()->removeBlock(this);
		}
		// We set a NULL library.
		if(lib == K_NULL)
		{
			notifyRemoved();
		}
	}
	else if(lib) // The block had no library and is getting one: it is being inserted !
	{
		notifyInserted();
	}

	setParent(lib); // Qt threading and everything...
	_library = lib;
}

void Block::optimize()
{
	// Default does... well, nothing :D
	// This is definitely an optimal Block.
}

QString Block::objectClassName() const
{
	return QString::fromAscii(metaObject()->className());
}

void Block::blockName(const QString& name)
{
	QMutexLocker locker(&_mutex);
	if(objectName() == name)
	{
		return;
	}

	setObjectName(name);
	emit blockNameChanged(name);
	emit blockChanged();
}

QString Block::blockName() const
{
	return objectName();
}

QString Block::infoString() const
{
	return objectClassName();
}

QString Block::iconPath() const
{
	return QString("kore/images/icons/block.png");
}

kid Block::ID() const {
	return reinterpret_cast<kid>(this); // Memory location (32/64 bits compatible).
}

void Block::addFlag(kuint flag)
{
	QMutexLocker locker(&_mutex);
	_flags |= flag;
}

void Block::removeFlag(kuint flag)
{
	QMutexLocker locker(&_mutex);
	(_flags & flag) ? _flags ^= flag : _flags;
}

void Block::notifyInserted()
{
	emit blockInserted();
}

void Block::notifyRemoved()
{
	emit blockRemoved();
}

void Block::notifyDeleted()
{
	emit blockDeleted();
}

const BlockFactory* Block::factory() const
{
	return _factory;
}

const MetaBlock* Block::metaBlock() const
{
	return K_NULL;
}

kbool Block::fastInherits(const MetaBlock* mb) const
{
	const QMetaObject* classMO = mb->blockMetaObject();

	// Speedup proposal... Far more efficient based on pointer comparison / arithmetic.
	// What about cache locality ?
	// Avoid using qMetaCast based on strcmp stuff...
	for(const QMetaObject* mo = this->metaObject(); mo; mo = mo->superClass())
	{
		if(mo == classMO)
		{
			return true;
		}
	}

	return false;
}

void Block::freeze(kbool frozen)
{
	QMutexLocker locker(&_mutex);
	if(frozen != _frozen)
	{
		_frozen = frozen;
		emit freezeChanged(_frozen);
	}
}

kbool Block::isFrozen() const
{
	return _frozen;
}

QVariant Block::DefaultBlockProperty(kint property)
{
	switch(property)
	{
	case Block::BlockTypeName:
		return tr("Basic block");
	case Block::BlockPropertiesName:
		{
			QHash<QString,QVariant> result;
			result.insert("blockName", tr("Name:"));
			return result;
		}
	default:
		return QVariant();
	}
}