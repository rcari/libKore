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
 *	DISCLAIMED. IN NO EVENT SHALL Romuald CARI BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <plugin/Module.hpp>
using namespace Kore::data;
using namespace Kore::plugin;

#include <KoreModule.hpp>

#define K_BLOCK_TYPE Kore::plugin::Module
#include <data/BlockMacros.hpp>
K_BLOCK_BEGIN
    K_BLOCK_ICON( "kore/images/icons/module.png" )
    K_BLOCK_VIRTUAL
    K_BLOCK_PROPERTY_DEFAULT
K_BLOCK_END

Module::Module()
{
}

void Module::registerLoadable( Loadable::Instantiator instantiator )
{
    _instantiators.append(instantiator);
}

bool Module::load()
{
    blockName( name() );
    while( ! _instantiators.empty() )
    {
        Loadable* loadable = ( _instantiators.takeFirst() )();
        if( ! loadable )
        {
            clear();
            return false;
        }
        addBlock( loadable );
    }
    return true;
}

bool Module::unload()
{
    // Check that all meta blocks can be unloaded...
    for( kint i = 0; i < size(); ++i )
    {
        if( ! at( i )->canUnload() )
            return false;
    }
    clear();
    return true;
}
