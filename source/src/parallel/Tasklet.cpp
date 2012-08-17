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

#include <parallel/Tasklet.hpp>
#include <parallel/TaskletRunner.hpp>
using namespace Kore::parallel;

#include <QtCore/QCoreApplication>
#include <QtCore/QMutexLocker>
#include <QtCore/QThread>

/* TRANSLATOR Kore::parallel::Tasklet */

namespace Kore { namespace parallel {

class ProgressEvent : public QEvent
{
public:
	ProgressEvent(const QString& message)
	:	QEvent((QEvent::Type)Tasklet::ProgressMessageEvent),
	 	_progress(0),
		_total(0),
		_message(message)
	{
	}

	ProgressEvent(kuint64 progress, kuint64 total)
	:	QEvent((QEvent::Type)Tasklet::ProgressEvent),
	 	_progress(progress),
	 	_total(total)
	{
	}

public:
	const QString& message() const { return _message; }
	kuint64 progress() const { return _progress; }
	kuint64 total() const { return _total; }

private:
	kuint64 _progress;
	kuint64 _total;
	QString _message;
};

}}

Tasklet::Tasklet(kbool autoDelete)
:	_autoDelete(autoDelete),
 	_state(NotStarted)
{
}

Tasklet::~Tasklet()
{
	if(!checkFlag(IsBeingDeleted))
	{
		// The Tasklet was created on the stack ! // ???? What is that for :/
		addFlag(IsBeingDeleted);
	}
}

QString Tasklet::runnerName() const
{
	return tr("Undefined Tasklet runner...");
}

kint Tasklet::performanceScore() const
{
	return -1; // Lowest performance, lowest priority.
}

void Tasklet::run(Tasklet*) const
{
	qWarning("The Tasklet %s has no default implementation !", qPrintable(objectClassName()));
}

kbool Tasklet::waitForFinished(kulong timeout)
{
	QMutexLocker locker(&_waitMutex);
	if(_state < Canceled)
	{
		// Wait on the wait condition.
		return _waitForFinished.wait(&_waitMutex, timeout);
	}
	return true;
}

kbool Tasklet::isRunning() const
{
	return _state == Running;
}

void Tasklet::cancel()
{
	_state = Aborted; // XXX: We might have to use something stronger for that, such as QAtomicInt or a specific mutex.
}

kbool Tasklet::isCancellable() const
{
	return checkFlag(Cancellable);
}

const MetaTasklet* Tasklet::metaTasklet() const
{
	return K_NULL;
}

void Tasklet::customEvent(QEvent* e)
{
	switch((kint)e->type())
	{
	case StartedEvent:
		e->accept();
		_state = Running;
		emit started();
		return; // We return !
	case ProgressMessageEvent:
		emit progress( static_cast<Kore::parallel::ProgressEvent*>(e)->message() );
		return;
	case ProgressEvent:
		{
			Kore::parallel::ProgressEvent* event = static_cast<Kore::parallel::ProgressEvent*>(e);
			emit progress(event->progress(), event->total());
		}
		return;
	case CanceledEvent:
		e->accept();
		_state = Canceled;
		break;
	case FailedEvent:
		e->accept();
		_state = Failed;
		break;
	case CompletedEvent:
		e->accept();
		_state = Completed;
		break;
	default:
		Block::customEvent(e);
		return;
	}

	// Just to factor code, for Canceled/Failed/Completed events.
	emit ended(_state);
	if(_autoDelete)
	{
		destroy();
	}
}

void Tasklet::runnerStarted()
{
	_state = Running;

	_waitMutex.lock();

	if(this->thread() == QThread::currentThread())
	{
		QEvent event((QEvent::Type)StartedEvent);
		QCoreApplication::sendEvent(this, &event);
	}
	else
	{
		QCoreApplication::postEvent(this, new QEvent((QEvent::Type)StartedEvent), Qt::HighEventPriority);
	}
}

void Tasklet::runnerCanceled()
{
	if(this->thread() == QThread::currentThread())
	{
		QEvent event((QEvent::Type)CanceledEvent);
		QCoreApplication::sendEvent(this, &event);
	}
	else
	{
		_state = Canceled; // Update right away (because of the wait condition)
		QCoreApplication::postEvent(this, new QEvent((QEvent::Type)CanceledEvent), Qt::HighEventPriority);
		_waitForFinished.wakeOne();
	}
	// Release the lock on the Tasklet.
	_waitMutex.unlock();
}

void Tasklet::runnerFailed()
{
	if(this->thread() == QThread::currentThread())
	{
		QEvent event((QEvent::Type)FailedEvent);
		QCoreApplication::sendEvent(this, &event);
	}
	else
	{
		_state = Failed; // Update right away (because of the wait condition)
		QCoreApplication::postEvent(this, new QEvent((QEvent::Type)FailedEvent), Qt::HighEventPriority);
		_waitForFinished.wakeOne();
	}
	// Release the lock on the Tasklet.
	_waitMutex.unlock();
}

void Tasklet::runnerCompleted()
{
	if(this->thread() == QThread::currentThread())
	{
		QEvent event((QEvent::Type)CompletedEvent);
		QCoreApplication::sendEvent(this, &event);
	}
	else
	{
		_state = Completed; // Update right away (because of the wait condition)
		QCoreApplication::postEvent(this, new QEvent((QEvent::Type)CompletedEvent), Qt::HighEventPriority);
		_waitForFinished.wakeOne();
	}
	// Release the lock on the Tasklet.
	_waitMutex.unlock();
}

void Tasklet::runnerProgress(const QString& message)
{
	if(this->thread() == QThread::currentThread())
	{
		Kore::parallel::ProgressEvent event(message);
		QCoreApplication::sendEvent(this, &event);
	}
	else
	{
		QCoreApplication::postEvent(this, new Kore::parallel::ProgressEvent(message));
	}
}

void Tasklet::runnerProgress(kuint64 progress, kuint64 total)
{
	if(this->thread() == QThread::currentThread())
	{
		Kore::parallel::ProgressEvent event(progress, total);
		QCoreApplication::sendEvent(this, &event);
	}
	else
	{
		QCoreApplication::postEvent(this, new Kore::parallel::ProgressEvent(progress, total));
	}
}
