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

namespace Kore { namespace parallel {

class Tasklet;

/*!
 * @class TaskletRunner
 *
 * A TaskletRunner represents an implementation of a Tasklet's set of operations.
 */
class KoreExport TaskletRunner
{
public:
	enum RunMode
	{
		Synchronous,
		Asynchronous,
	};

public:
	virtual ~TaskletRunner();

	virtual QString runnerName() const = K_NULL;

	/*!
	 * Evaluate RAW performance of this implementation for the given Tasklet.
	 *
	 * The score is used by the Parallel engine to decide which implementation to use
	 * in order to run the Tasklet.
	 *
	 * @return raw score for this implemetnation
	 */
	virtual kint performanceScore() const = K_NULL;

	/*!
	 * This method implements the actual operations on the Tasklet.
	 *
	 * A single TaskletRunner instance may be used at the same time by different threads. This method
	 * is const to make sure that no modifications whatsoever are made to the instance.
	 *
	 * @param tasklet The tasklet to run.
	 */
	virtual void run(Tasklet* tasklet) const = K_NULL;

protected:
	void start(Tasklet* tasklet) const;
	void cancel(Tasklet* tasklet) const;
	void fail(Tasklet* tasklet) const;
	void complete(Tasklet* tasklet) const;
	void progress(Tasklet* tasklet, const QString& message) const;
	void progress(Tasklet* tasklet, kuint64 progress, kuint64 total) const;
	kbool keepRunning(Tasklet* tasklet) const;

};

}}
