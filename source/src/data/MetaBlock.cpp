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
#include <data/BlockSettings.hpp>
#include <data/MetaBlock.hpp>
#include <data/BlockExtension.hpp>
using namespace Kore::data;

#include <KoreApplication.hpp>
#include <KoreEngine.hpp>
using namespace Kore;

#include <QtCore/QSettings>

MetaBlock::MetaBlock(const char* className, const QMetaObject* mo)
:	_blockMetaObject(mo),
	_superMetaBlock(K_NULL),
	_blockClassID(K_NULL)
{
	blockName( className );
	addFlag(System);
	_blockClassID = qHash( QByteArray::fromRawData( className, strlen(className)) );
}

void MetaBlock::library(Library* lib)
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

void MetaBlock::createClassID() const
{
	if(_blockClassID != K_NULL)
	{
		return;
	}

	// Hash the class name
	qDebug("Generating hash for string: %s", _blockMetaObject->className());

	_blockClassID = qHash(
			QByteArray::fromRawData(
					_blockMetaObject->className(),
					strlen(_blockMetaObject->className())
				)
		);
}

void MetaBlock::createPropertiesCache() const
{
	if(!_propertiesHashes.isEmpty())
	{
		return;
	}

	// Hash the properties names.
	_propertiesHashes.resize(_blockMetaObject->propertyCount());
	_propertiesHashes.fill(0);
	for(kint i = Block::staticMetaObject.propertyOffset(); i < _blockMetaObject->propertyCount(); i++)
	{
		khash propertyHash = qHash(
				QByteArray::fromRawData(
						_blockMetaObject->property(i).name(),
						strlen(_blockMetaObject->property(i).name())
					)
			);

		// To check for very low probability hash collisions...
		K_ASSERT( !_propertiesHashes.contains(propertyHash) )

		_propertiesHashes[i] = propertyHash;
	}
}

void MetaBlock::clearExtensions()
{
	QList<BlockExtension*> extensions = _extensions.values();
	for(int i = 0; i < extensions.size(); i++)
	{
		extensions.at(i)->unregisterWithMetaBlock(this);
	}
}

bool MetaBlock::canDestroy()
{
	return _instancesCount == 0;
}

/*bool MetaBlock::destroy()
{
	// A Meta-Block can not be destroyed !
	// It will be cleared when the process will terminate.
	setParent(K_NULL); // Remove from the Qt tree for deletion !
	return false;
}*/

QString MetaBlock::iconPath() const
{
	return QString("kore/images/icons/metablock.png");
}

void MetaBlock::optimize()
{
	createClassID();
	createPropertiesCache();
}

QVariant MetaBlock::blockSetting(const QString& setting, const QVariant& defaultValue) const
{
	QSettings settings;
	settings.beginGroup(K_BLOCK_SETTINGS_GROUP);
	return settings.value(QString("%1.%2").arg(blockClassName()).arg(setting), defaultValue);
}

BlockExtension* MetaBlock::blockExtension(const QString& name) const
{
	return _extensions.value(name, K_NULL);
}

QList<BlockExtension*> MetaBlock::blockExtensions(const QString& name) const
{
	return _extensions.values(name);
}

const QMultiHash<QString,BlockExtension*>& MetaBlock::extensions() const
{
	return _extensions;
}

kbool MetaBlock::registerBlockExtension(BlockExtension* extension)
{
	_extensions.insertMulti(extension->extensionName(), extension);
	return true;
}

void MetaBlock::unregisterBlockExtension(BlockExtension* extension)
{
	_extensions.remove(extension->extensionName(), extension);
}

MetaBlock* MetaBlock::superMetaBlock()
{
	if(!_superMetaBlock)
	{
		_superMetaBlock	= KoreEngine::GetMetaBlock(
				QString::fromAscii(_blockMetaObject->superClass()->className())
			);
	}
	return _superMetaBlock;
}

const MetaBlock* MetaBlock::superMetaBlock() const
{
	if(!_superMetaBlock)
	{
		_superMetaBlock	= KoreEngine::GetMetaBlock(
				QString::fromAscii(_blockMetaObject->superClass()->className())
			);
	}
	return _superMetaBlock;
}

void MetaBlock::destroyBlock(Block* b) const
{
	// The following line is useless (and therefore commented out) here as we use deleteLater().
	// It might however come handy if we manage ourselves the creation / deletion of Blocks.
	//QCoreApplication::removePostedEvents(b);
	if( kApp->isClosing() )
	{
		// No delete later, delete the block right away !
		delete b;
	}
	else
	{
		b->deleteLater();
	}
}
