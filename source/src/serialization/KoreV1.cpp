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

#include <serialization/KoreV1.hpp>
#include <serialization/Structures.hpp>
#include <serialization/ProgressCallback.hpp>
using namespace Kore::serialization;

#include <data/Library.hpp>
using namespace Kore::data;

#include <KoreEngine.hpp>

#include <QtCore/QDataStream>
#include <QtCore/QtEndian>

BlockDeflater::DeflationResult KoreV1::deflate(QIODevice* device, const Block* block, ProgressCallback* callback) const
{
	if(callback)
	{
		if(!callback->continueOperation())
		{
			// Operation was canceled !
			return BlockDeflater::Canceled;
		}
	}

	if(!block->checkFlag(Block::Serializable))
	{
		return BlockDeflater::Skipped;
	}

	const Library* lib = K_NULL;

	BlockHeader header;
	header.blockType = block->metaBlock()->blockClassID();

	// Compute the number of serializable children...
	if(block->isLibrary())
	{
		kuint childrenNb = 0;
		lib = static_cast<const Library*>(block);
		for(kint i = 0; i < lib->size(); i++)
		{
			if(lib->at(i)->checkFlag(Block::Serializable))
			{
				childrenNb++;
			}
		}
		header.childrenNb = childrenNb;
	}
	else
	{
		header.childrenNb = 0;
	}

	// We create a scope to avoid keeping everything in memory during serialization (byte array + recursion !)
	{
		// Byte array to store data
		// TODO: Find a way to preallocate memory for the buffer.
		QByteArray data;
		data.reserve(1024); // Pre allocate 1KB...

		// Data stream to actually process the data
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream.setByteOrder(QDataStream::LittleEndian); // Little endian
		stream.setVersion(QDataStream::Qt_4_6);

		// Serialize the block
		header.propertiesNb = 0;
		for	(
				kint i = Block::staticMetaObject.propertyOffset();
				i < block->metaObject()->propertyCount();
				i++
			)
		{
			const MetaBlock* mb = block->metaBlock();

			QMetaProperty property = mb->blockMetaObject()->property(i);
			if(!property.isStored())
			{
				continue;
			}

			stream << mb->propertyID(i);
			stream << property.read(block);

			// Store the number of serialized properties.
			header.propertiesNb++;
		}

		// Store the size of the buffer.
		header.size = data.size();

		QDataStream dataStream(device);
		dataStream.setByteOrder(QDataStream::LittleEndian); // Little endian
		dataStream.setVersion(QDataStream::Qt_4_6);

		dataStream << header.size;
		dataStream << header.blockType;
		dataStream << header.propertiesNb;
		dataStream << header.childrenNb;

		// Write the data to the stream.
		dataStream.writeRawData(data.constData(), data.size());
	}

	if(lib)
	{
		// Serialize the block's children.
		for(kint i = 0; i < lib->size(); i++)
		{
			deflate(device, lib->at(i), callback);
		}
	}

	return BlockDeflater::Success;
}

BlockInflater::InflationResult KoreV1::inflate(QIODevice* device, Kore::data::Block** block, ProgressCallback* callback) const
{
	if(callback)
	{
		if(!callback->continueOperation())
		{
			// Operation was canceled !
			return BlockInflater::Canceled;
		}
		callback->progress(device->pos(), device->size());
	}

	BlockHeader header;

	// Scope to release the data stream resources.
	{
		QDataStream stream(device);
		stream.setByteOrder(QDataStream::LittleEndian); // Little endian
		stream.setVersion(QDataStream::Qt_4_6);

		stream >> header.size;
		stream >> header.blockType;
		stream >> header.propertiesNb;
		stream >> header.childrenNb;

		const MetaBlock* mb = KoreEngine::GetMetaBlock(header.blockType);
		if(!mb)
		{
			qWarning("Could not find the factory for block type %u", header.blockType);
			return BlockInflater::Failed;
		}

		*block = mb->createBlock();

		for(kuint i = 0; i < header.propertiesNb; i++)
		{
			quint32 propertyHash;
			QVariant variant;

			stream >> propertyHash;
			stream >> variant;

			kint propertyIndex = mb->propertyIndex(propertyHash);
			QMetaProperty property = mb->blockMetaObject()->property(propertyIndex);
			if(!property.write(*block, variant))
			{
				qWarning("Failed to set block property %s", property.name());
				return BlockInflater::Failed;
			}
		}
	}

//	qDebug(
//			"Successfully deserialized block %s of type %s\nID:%d\nTotalSize: %u\nPropertiesNb: %u\nChildrenNb: %u",
//			qPrintable((*block)->blockName()), qPrintable((*block)->objectClassName()),
//			(*block)->metaBlock()->blockClassID(),
//			header.size, header.propertiesNb, header.childrenNb
//		);

	for(kuint i = 0; i < header.childrenNb; i++)
	{
		Block* childBlock = K_NULL;
		if(inflate(device, &childBlock, callback) == BlockInflater::Failed) // TODO: Check for the Canceled case!
		{
			qWarning("Failed to add child %d to Block %s", i, qPrintable((*block)->blockName()));
			if(childBlock)
			{
				childBlock->destroy();
			}
			return BlockInflater::Failed;
		}
		static_cast<Library*>(*block)->addBlock(childBlock);
	}

	return BlockInflater::Success;
}
