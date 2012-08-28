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

#include <parallel/MetaTasklet.hpp>
#include <parallel/TaskletRunner.hpp>
using namespace Kore::parallel;

#include <QtCore/QtDebug>

MetaTasklet::MetaTasklet(const QMetaObject* mo)
:	Kore::data::MetaBlock(mo)
{
	blockName(tr("MetaTasklet for %1").arg(mo->className()));
}

bool RunnerLessThan(const TaskletRunner* r1, const TaskletRunner* r2)
{
	return r1->performanceScore() > r2->performanceScore();
}

void MetaTasklet::registerTaskletRunner(TaskletRunner* runner)
{
	K_ASSERT( !_runners.contains(runner) )
	_runners.append(runner);
	qSort(_runners.begin(), _runners.end(), &RunnerLessThan);
	qDebug() << "Kore / Registered tasklet runner:" << runner->runnerName()
			<< "for Tasklet:" << MetaBlock::blockClassName()
			<< "with score:" << runner->performanceScore();
}

void MetaTasklet::unregisterTaskletRunner(TaskletRunner* runner)
{
	K_ASSERT( _runners.contains(runner) )
	_runners.removeOne(runner);
	qSort(_runners.begin(), _runners.end(), &RunnerLessThan);
	qDebug() << "Kore / Registered tasklet runner:" << runner->runnerName()
			<< "for Tasklet:" << MetaBlock::blockClassName();
}
