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

#include <data/BlockExtension.hpp>
#include <data/MetaBlock.hpp>
using namespace Kore::data;

BlockExtension::BlockExtension(const QString& extensionName)
:	_extensionName(extensionName)
{
}

BlockExtension::~BlockExtension()
{
	// Cleanup registrations !
	for(kint i = 0; i < _registrations.size(); i++)
	{
		_registrations.at(i)->unregisterBlockExtension(this);
	}
	_registrations.clear();
}

const QString& BlockExtension::extensionName() const
{
	return _extensionName;
}

void BlockExtension::registerWithMetaBlock(MetaBlock* mb)
{
	K_ASSERT( !_registrations.contains(mb) ) // You can't register with a block more than once !
	_registrations.append(mb);
	mb->registerBlockExtension(this);
}

void BlockExtension::unregisterWithMetaBlock(MetaBlock* mb)
{
	_registrations.removeOne(mb);
	mb->unregisterBlockExtension(this);
}
