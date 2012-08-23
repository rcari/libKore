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

#include <data/Block.hpp>

#include <parallel/TaskletRunner.hpp>

#include <QtCore/QEvent>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

namespace Kore {

class KoreEngine;

namespace parallel {

class ProgressEvent;
class MetaTasklet;

/*!
 * @class Tasklet
 *
 * @brief A tasklet represents a set of data as well as a set of operations to be applied to that data.
 *
 * Although the tasklet MUST contain all the required data for the computation, it does not necessarily have
 * an actual implementation of the operations to be performed on its data (it implements the TaskletRunner interface though).
 * Using tasklets to implement complex operations allows such an operation to be performed in
 * various ways:
 * - It can be executed on the current thread as a standard synchronous function call
 * - It can be executed asynchronously (and take advantage of multithreading)
 * - It can be executed on a different processing unit such as a GPU (using OpenCL/CUDA...)
 *
 * @sa Kore::parallel::TaskletRunner
 * @sa K_TASKLET
 * @sa K_TASKLET_I
 */

class KoreExport Tasklet
:	public Kore::data::Block,
 	protected TaskletRunner
{
	Q_OBJECT

	friend class MetaTasklet;
	friend class ProgressEvent;
	friend class TaskletRunner;
	friend class Kore::KoreEngine;

	/*!
	 * Custom events for a Tasklet.
	 * XXX: Should be moved to the private section...
	 */
	enum Events
	{
		StartedEvent = QEvent::User,//!< Execution started event
		ProgressEvent,				//!< Progress event
		ProgressMessageEvent,		//!< Progress message event
		CanceledEvent,              //!< Execution was canceled event
		FailedEvent,                //!< Execution failed event
		CompletedEvent,             //!< Execution successfully completed event.
	};

protected:
	enum Flags
	{
		Cancellable =	Block::MAX_FLAG,
		MAX_FLAG =		(Block::MAX_FLAG << 1),
	};

public:
	/*!
	 * State of execution of the Tasklet.
	 */
	enum State
	{
		NotStarted = 0x0,//!< Execution has not started
		Running,         //!< Execution is in progress
		Aborted,		 //!< Execution is user-aborted
		Canceled,        //!< Execution was canceled
		Failed,          //!< Execution failed
		Completed,       //!< Execution successfully completed
	};

protected:
	/*!
	 * Constructor.
	 * @param autoDelete if true, the Tasklet is automatically destroyed when completed.
	 * @return a Tasklet instance.
	 */
	Tasklet(kbool autoDelete = false);

public:
	virtual ~Tasklet();

protected:
	// Tasklet implementation !
	virtual QString runnerName() const;
	virtual kint performanceScore() const;
	virtual void run(Tasklet* tasklet) const;

public:
	/*!
	 * Wait for the completion of the tasklet.
	 * @param timeout MAX number of ms to wait for completion before timeout. If set to ULONG_MAX, no timeout.
	 * @return true if the task completed, false if the wait timed out.
	 */
	kbool waitForFinished(kulong timeout = ULONG_MAX);
	kbool isRunning() const;

protected:
	// This is always executed in the thread the Tasklet belongs to (the main thread).
	/*!
	 * Overloading Qt's custom event processing routine.
	 *
	 * This is required for proper multi-threading operations. Tasklets always belong in the main application's thread (in a Qt sense).
	 * Thus, when a TaskletRunner performs operations asynchronously on a Tasklet, it must send/post messages to it after acquiring its
	 * mutex.
	 *
	 * @param e custom event to be processed
	 */
	virtual void customEvent(QEvent* e);

	/*!
	 * Convenience method for TaskletRunner-s. This handles message dispatching for started signal.
	 * @param runner The runner that started executing the tasklet.
	 */
	void runnerStarted();
	/*!
	 * Convenience method for TaskletRunner-s. This handles message dispatching for ended signal.
	 */
	void runnerCanceled();
	/*!
	 * Convenience method for TaskletRunner-s. This handles message dispatching for ended signal.
	 */
	void runnerFailed();
	/*!
	 * Convenience method for TaskletRunner-s. This handles message dispatching for ended signal.
	 */
	void runnerCompleted();

	/*!
	 * Convenience method for TaskletRunner-s. This allows runners to notify of progress.
	 * @param message
	 */
	void runnerProgress(const QString& message);

	/*!
	 * Convenience method for TaskletRunner-s. This allows runners to notify of progress.
	 * @param progress
	 * @param total
	 */
	void runnerProgress(kuint64 progress, kuint64 total);

	/*!
	 * Convenience method for TaskletRunner-s. Check if the computation should continue.
	 *
	 * TaskletRunner-s which implement the cancel operation should check regularly for the result of
	 * this method.
	 *
	 * @return true if the computation should continue, false otherwise.
	 */
	inline kbool keepRunning() { return _state == Running; }

signals:
	/*!
	 * Signal emitted when a TaskletRunner starts executing the Tasklet.
	 */
	void started();
	/*!
	 * Signal emitted when a TaskletRunner finished executing the Tasklet.
	 * @param state state at the end, @see State
	 */
	void ended(kint state);
	/*!
	 * Signal emitted when a TaskletRunner notifies its progress.
	 * @param message a string message
	 */
	void progress(const QString& message);
	/*!
	 * Signal emitted when a TaskletRunner notifies its progress.
	 * @param progress current progress
	 * @param total total progress
	 */
	void progress(kuint64 progress, kuint64 total);

public slots:
	/*!
	 * Cancel the execution of the Tasklet.
	 *
	 * This truly only makes sense for asynchronous execution of tasks.
	 * This merely suggests that the execution of the tasklet should be canceled. There is
	 * no guarantee that the running implementation is able to do so.
	 */
	void cancel();
	/*!
	 * Check whether the tasklet can be cancelled.
	 *
	 * @return true if it is cancellable, false otherwise.
	 */
	kbool isCancellable() const;

public:
	virtual const Kore::parallel::MetaTasklet* metaTasklet() const;

private:
	kbool _autoDelete;
	volatile State _state;
	QMutex _waitMutex;
	QWaitCondition _waitForFinished;
};

}}

#include "MetaTasklet.hpp"

/*!
 * K_TASKLET macro.
 */
#define K_TASKLET friend class Kore::data::Block;\
	private:\
		static const QMetaObject* QtMetaObject() { return &staticMetaObject; }\
	class PrivateMetaTasklet : public Kore::parallel::MetaTasklet {\
	private:\
		PrivateMetaTasklet();\
	public:\
		virtual Kore::data::Block* createBlock() const;\
		virtual Kore::data::Block* createBlock(kvoid*) const;\
		virtual ksize blockSize() const;\
		virtual QVariant blockProperty(kint property) const;\
		static PrivateMetaTasklet* Instance() { return (_Instance != K_NULL) ? _Instance : _Instance = new PrivateMetaTasklet(); }\
	private:\
		static PrivateMetaTasklet* _Instance;\
		static bool _Registered;\
	};\
	public:\
		inline static Kore::data::MetaBlock* StaticMetaBlock() { return PrivateMetaTasklet::Instance(); }\
		virtual Kore::data::MetaBlock* metaBlock() const { return PrivateMetaTasklet::Instance(); }\
		inline static const Kore::parallel::MetaTasklet* StaticMetaTasklet() { return PrivateMetaTasklet::Instance(); }\
		virtual const Kore::parallel::MetaTasklet* metaTasklet() const { return PrivateMetaTasklet::Instance(); }\
	private:
