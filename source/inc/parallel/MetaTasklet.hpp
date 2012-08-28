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

#include <data/MetaBlock.hpp>

namespace Kore {

class KoreEngine;

namespace parallel {

class TaskletRunner;

/*!
 * @class MetaTasklet
 *
 * This class is a meta object for a tasklet. Its main role is to gather
 * the available TaskletRunner-s for the Tasklet it describes.
 *
 * This class should not be used on its own !
 *
 * @sa Kore::parallel::Tasklet
 *
 * @sa K_TASKLET
 * @sa K_TASKLET_I
 * @sa K_TASKLET_RUNNER
 * @sa K_TASKLET_RUNNER_I
 */
class KoreExport MetaTasklet : public Kore::data::MetaBlock
{
	Q_OBJECT

	friend class Kore::KoreEngine;

protected:
	/*!
	 * Constructor.
	 * @param mo Qt meta object for the tasklet.
	 * @return a MetaTasklet instance.
	 */
	MetaTasklet(const QMetaObject* mo);

	/*!
	 * Register a tasklet runner for the described tasklet.
	 * @param runner a runner to register.
	 */
	void registerTaskletRunner(TaskletRunner* runner);

	/*!
	 * Unregister a tasklet runner for the described tasklet.
	 * @param runner a runner to unregister.
	 */
	void unregisterTaskletRunner(TaskletRunner* runner);

public:
	/*!
	 * Get a list of available runners.
	 * @return the list of available runners.
	 */
	inline const QList<const TaskletRunner*>& runners() const { return _runners; }
	inline const TaskletRunner* bestRunner() const
	{
		return _runners.isEmpty() ? K_NULL : _runners.first();
	}

	virtual QString blockIconPath() const { return QString(); }

private:
	QList<const TaskletRunner*> _runners;
};

}}
