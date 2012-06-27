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

#include <KoreEngine.hpp>
using namespace Kore;
using namespace Kore::data;

#include <event/KoreEvent.hpp>
#include <event/ErrorEvent.hpp>
using namespace Kore::event;

#include <parallel/Tasklet.hpp>
using namespace Kore::parallel;

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QThread>

#include <QtCore/QtConcurrentRun>
#include <QtCore/QtDebug>

/* TRANSLATOR Kore::KoreEngine */

KoreEngine::KoreEngine()
:	_metaBlocks(Block::SystemOwned)
{
	blockName(tr("Kore Engine"));
	K_ASSERT( _Instance == K_NULL )
	addFlag(Library::System);
	_metaBlocks.blockName(tr("Meta Blocks"));
	addBlock(&_metaBlocks);

	qDebug() << "Kore / Starting up on" << QDateTime::currentDateTime().toString();
}

void KoreEngine::customEvent(QEvent* event)
{
	if(event->type() != KoreEvent::EventType())
	{
		Library::customEvent(event);
		return; // This is not an event for us !
	}

	KoreEvent* kEvent = static_cast<KoreEvent*>(event);
	switch(kEvent->koreEventType())
	{
	case KoreEvent::Error:
		{
			ErrorEvent* eEvent = static_cast<ErrorEvent*>(kEvent);
			emit error(eEvent->error(), eEvent->details());
		}
		break;
	default:
		break;
	}
}

const Library* KoreEngine::MetaBlocks()
{
	return &Instance()->_metaBlocks;
}

void KoreEngine::RegisterMetaBlock(MetaBlock* mb)
{
	Instance()->_metaBlocks.addBlock(mb);
	Instance()->_metaBlocksStringHash.insert(mb->blockClassName(), mb);
	K_ASSERT( !Instance()->_metaBlocksHashHash.contains(mb->blockClassID()) )
	Instance()->_metaBlocksHashHash.insert(mb->blockClassID(), mb);
	qDebug("Kore / Registered meta-block for %s", qPrintable(mb->blockClassName()));
}

Block* KoreEngine::CreateBlock(QString name)
{
	const MetaBlock* mb = GetMetaBlock(name);
	return mb ? mb->createBlock() : K_NULL;
}

void KoreEngine::RunTasklet(Tasklet* tasklet, TaskletRunner::RunMode mode)
{
	// Find the runner.
	const TaskletRunner* runner = tasklet->metaTasklet() ? tasklet->metaTasklet()->bestRunner() : K_NULL;
	runner = runner ? runner : tasklet; // Because the tasklet is its default runner as well !

	switch(mode)
	{
	case TaskletRunner::Synchronous:
		runner->run(tasklet); // Run right here on the current thread.
		break;
	case TaskletRunner::Asynchronous:
		// TODO: Manage our own thread pool ? Would that be useful ?
		QtConcurrent::run(runner, &TaskletRunner::run, tasklet); // Use a future, Qt ThreadPool.
		break;
	default:
		qWarning("Kore / Unknown running mode for tasklet %s", qPrintable(tasklet->objectClassName()));
		break;
	}
}

MetaBlock* KoreEngine::GetMetaBlock(QString name)
{
	return Instance()->_metaBlocksStringHash.value(name, K_NULL);
}

MetaBlock* KoreEngine::GetMetaBlock(khash blockTypeHash)
{
	return Instance()->_metaBlocksHashHash.value(blockTypeHash, K_NULL);
}

void KoreEngine::Error(QString error, QString details)
{
	// Set a default details string.
	details = details.isNull() ? tr("None") : details;

	if(QThread::currentThread() == Instance()->thread())
	{
		ErrorEvent event(error, details);
		QCoreApplication::sendEvent(Instance(), &event);
	}
	else
	{
		ErrorEvent* event = new ErrorEvent(error, details);
		QCoreApplication::postEvent(Instance(), event);
	}
}

KoreEngine* KoreEngine::Instance()
{
	return (_Instance != K_NULL) ? _Instance : _Instance = new KoreEngine();
}

KoreEngine* KoreEngine::_Instance = K_NULL;
