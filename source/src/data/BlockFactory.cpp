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

#include <data/BlockFactory.hpp>
#include <data/MetaBlock.hpp>
using namespace Kore::data;

#include <KoreApplication.hpp>
#include <KoreEngine.hpp>
using namespace Kore;

#include <QtCore/QCoreApplication>

BlockFactory::BlockFactory(const MetaBlock* mb)
:	_mb(mb)
{
}

BlockFactory::~BlockFactory()
{
}

void BlockFactory::setBlockFactory(Block* b) const
{
	b->_factory = this;
}

void BlockFactory::destroyBlock(Block* b) const
{
	// The following line is useless (and therefore commented out) here as we use deleteLater().
	// It might however come handy if we manage ourselves the creation / deletion of Blocks.
	//QCoreApplication::removePostedEvents(b);
	if( kApp->isClosing() )
	{
		// No delete later, delete the block right away !
		// Cleanup!!
		QCoreApplication::removePostedEvents(b); // XXX: This might be useless...
		delete b;
	}
	else
	{
		// Delete when controls gets back to the main event loop.
		b->deleteLater();
	}
}
