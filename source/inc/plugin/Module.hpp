/*
 * 	Copyright (c) 2010-2014, Romuald CARI
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *		* Redistributions of source code must retain the above copyright
 *		  notice, this list of conditions and the following disclaimer.
 *		* Redistributions in binary form must reproduce the above copyright
 *		  notice, this list of conditions and the following disclaimer in the
 *		  documentation and/or other materials provided with the distribution.
 *		* Neither the name of the Moving Pixels Labs nor the
 *		  names of its contributors may be used to endorse or promote products
 *		  derived from this software without specific prior written permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL Romuald CARI BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once

#include <data/LibraryT.hpp>
#include <data/MetaBlock.hpp>
#include <QtCore/QLinkedList>

namespace Kore { namespace plugin {

class KoreExport Module : public Kore::data::LibraryT<Kore::data::MetaBlock>
{
	Q_OBJECT

	Q_PROPERTY( QString name READ name DESIGNABLE true )
	Q_PROPERTY( QString author READ author DESIGNABLE true )
	Q_PROPERTY( QString url READ url DESIGNABLE true )
	Q_PROPERTY( QString version READ version DESIGNABLE true )

public:
	typedef Kore::data::MetaBlock* (*MetaBlockInstantiator)();

protected:
	Module();

public:
	kbool load();
	kbool unload();

public:
	virtual QString name() const = 0;
	virtual QString author() const = 0;
	virtual QString url() const = 0;
	virtual QString version() const = 0;

	void registerMetaBlockInstantiator(MetaBlockInstantiator instantiator);

private:
	QLinkedList<MetaBlockInstantiator> _instantiators;
};

}}

#define K_MODULE friend class Kore::data::MetaBlock;\
	public:\
		static const Kore::plugin::Module* Instance();\
		static bool RegisterMetaBlockInstantiator(MetaBlockInstantiator instantiator);\
	private:\
		static Kore::plugin::Module* PrivateInstance();\
		static Kore::plugin::Module* _Instance;
