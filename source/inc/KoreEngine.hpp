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

#include <data/LibraryT.hpp>
#include <parallel/TaskletRunner.hpp>

#include <QtCore/QHash>

namespace Kore {

namespace plugin { class Module; }

class KoreApplication;

class KoreExport KoreEngine : public Kore::data::Library
{
	friend class KoreApplication;

	Q_OBJECT

private:
	KoreEngine();

protected:
	virtual void customEvent(QEvent* event);

signals:
	void error(QString error, QString details);

public:
	static const QList<Kore::data::MetaBlock*> MetaBlocks();
	static void RegisterModule(Kore::plugin::Module* module);
	static void RegisterMetaBlock(Kore::data::MetaBlock* mb);
	static void UnregisterMetaBlock(Kore::data::MetaBlock* mb);
	static Kore::data::Block* CreateBlock(QString name);

	template<typename T>
	static T* CreateBlockT(QString name) { return static_cast<T*>( CreateBlock(name) ); }

	template<typename T>
	static void RegisterTaskletRunner(Kore::parallel::TaskletRunner* runner) { T::StaticMetaTasklet()->registerTaskletRunner(runner); }
	static void RunTasklet(Kore::parallel::Tasklet* tasklet, Kore::parallel::TaskletRunner::RunMode mode);

	static Kore::data::MetaBlock* GetMetaBlock(QString name);
	static Kore::data::MetaBlock* GetMetaBlock(khash blockTypeHash);

	static void Error(QString error, QString details = QString());

	static KoreEngine* Instance();

private:
	Kore::data::LibraryT<Kore::plugin::Module> _modules;
	QHash<QString,Kore::data::MetaBlock*> _metaBlocksStringHash;
	QHash<khash,Kore::data::MetaBlock*> _metaBlocksHashHash;

private:
	static KoreEngine* _Instance;

};

}

/*#define K_BLOCK_I_VIRTUAL( blockType ) blockType::PrivateMetaBlock::PrivateMetaBlock() : MetaBlock(&(blockType::staticMetaObject)) { setObjectName( # blockType ); Kore::KoreEngine::RegisterMetaBlock( this ); }\
	Kore::data::Block* blockType::PrivateMetaBlock::createBlock() const { qWarning("Can not instantiate virtual block " # blockType); return K_NULL; }\
	Kore::data::Block* blockType::PrivateMetaBlock::createBlock(kvoid*) const { qWarning("Can not instantiate virtual block " # blockType); return K_NULL; }\
	ksize blockType::PrivateMetaBlock::blockSize() const { return sizeof(blockType); }\
	QVariant blockType::PrivateMetaBlock::blockProperty(int property) const { return Kore::data::Block::BlockProperty(property); }\
	blockType::PrivateMetaBlock* blockType::PrivateMetaBlock::_Instance = blockType::PrivateMetaBlock::Instance();

#define K_BLOCK_I( blockType ) blockType::PrivateMetaBlock::PrivateMetaBlock() : MetaBlock(&(blockType::staticMetaObject)) { setObjectName( # blockType ); Kore::KoreEngine::RegisterMetaBlock( this ); }\
	Kore::data::Block* blockType::PrivateMetaBlock::createBlock() const { Kore::data::Block* b = new blockType; setBlockFactory(b); return b; }\
	Kore::data::Block* blockType::PrivateMetaBlock::createBlock(kvoid* ptr) const { Kore::data::Block* b = new (ptr) blockType; setBlockFactory(b); return b; }\
	ksize blockType::PrivateMetaBlock::blockSize() const { return sizeof(blockType); }\
	QVariant blockType::PrivateMetaBlock::blockProperty(int property) const { return Kore::data::Block::BlockProperty(property); }\
	blockType::PrivateMetaBlock* blockType::PrivateMetaBlock::_Instance = blockType::PrivateMetaBlock::Instance();

#define K_BLOCK_PROPERTY_I( blockType, propertyFunction ) blockType::PrivateMetaBlock::PrivateMetaBlock() : MetaBlock(&(blockType::staticMetaObject)) { setObjectName( # blockType ); Kore::KoreEngine::RegisterMetaBlock( this ); }\
	Kore::data::Block* blockType::PrivateMetaBlock::createBlock() const { Kore::data::Block* b = new blockType; setBlockFactory(b); return b; }\
	Kore::data::Block* blockType::PrivateMetaBlock::createBlock(kvoid* ptr) const { Kore::data::Block* b = new (ptr) blockType; setBlockFactory(b); return b; }\
	ksize blockType::PrivateMetaBlock::blockSize() const { return sizeof(blockType); }\
	QVariant blockType::PrivateMetaBlock::blockProperty(int property) const { return propertyFunction(property); }\
	blockType::PrivateMetaBlock* blockType::PrivateMetaBlock::_Instance = blockType::PrivateMetaBlock::Instance();

#define K_TASKLET_I( taskletType ) taskletType::PrivateMetaTasklet::PrivateMetaTasklet() : MetaTasklet(&(taskletType::staticMetaObject)) { setObjectName( # taskletType ); Kore::KoreEngine::RegisterMetaBlock( this ); }\
	Kore::data::Block* taskletType::PrivateMetaTasklet::createBlock() const { Kore::data::Block* b = new taskletType; setBlockFactory(b); return b; }\
	Kore::data::Block* taskletType::PrivateMetaTasklet::createBlock(kvoid*) const { Kore::data::Block* b = new (ptr) taskletType; setBlockFactory(b); return b; }\
	ksize taskletType::PrivateMetaTasklet::blockSize() const { return sizeof(taskletType); }\
	QVariant taskletType::PrivateMetaTasklet::blockProperty(int) const { return QVariant(); }\
	taskletType::PrivateMetaTasklet* taskletType::PrivateMetaTasklet::_Instance = taskletType::PrivateMetaTasklet::Instance();*/

#define K_TASKLET_RUNNER_I( runnerType, taskletType ) kbool runnerType::_K_Instance = runnerType::CreateInstance();\
	runnerType::CreateInstance() { _K_Instance = new runnerType; Kore::KoreEngine::RegisterTaskletRunner<taskletType>(_K_Instance); taskletType::TaskletFactory(); }
