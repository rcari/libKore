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

#include <KoreApplication.hpp>
#include <KoreEngine.hpp>
#include <KoreModule.hpp>
using namespace Kore;

#include <data/Library.hpp>
using namespace Kore::data;

#include <memory/SimpleMemoryManager.hpp>
using namespace Kore::memory;

#include <QtCore/QCoreApplication>
#include <QtCore/QMetaMethod>

KoreApplication::KoreApplication( kint argc, kchar** argv )
    : _argc     ( argc )
    , _argv     ( argv )
    , _closing  ( false )
{
    qDebug( "Kore / Loading KoreApplication" );

    // First, we check that Qt was properly initialized.
    K_ASSERT( QCoreApplication::instance() != K_NULL );
    // Only one Kore Application instance.
    K_ASSERT( _Instance == K_NULL )

    _Instance = this; // Store the instance !

    // Create the memory manager first!!!
    _memoryManager = new SimpleMemoryManager();

    // Create the root library.
    _rootLibrary = new Library( Block::System );
    _rootLibrary->blockName( "Root" );

    Library* appLib = new Library( Block::System );
    appLib->blockName( "Kore Internals" );

    _rootLibrary->addBlock( appLib );

    // Register the Kore engine
    appLib->addBlock( KoreEngine::Instance() );

    // Register the memory manager.
    appLib->addBlock( _memoryManager );

    // Load the module
    KoreModule::PrivateInstance()->load();

    // Create the library that will hold the application data.
    _dataLibrary = new Library( Block::System );
    _dataLibrary->blockName( "Data" );
    _rootLibrary->addBlock( _dataLibrary );

    // Optimize the root library (prepares the MetaBlock properties cache !!).
    _rootLibrary->optimizeTree();
}

KoreApplication::~KoreApplication()
{
    qDebug( "Kore / Unloading KoreApplication" );
    // Deletes all registered engines and managers and data structures.
    // This call effectively cleans up all heap allocated memory.
    _rootLibrary->destroy();
}

const Library* KoreApplication::rootLibrary() const
{
    return _rootLibrary;
}

const MemoryManager* KoreApplication::memoryManager() const
{
    return _memoryManager;
}

Library* KoreApplication::dataLibrary() const
{
    return _dataLibrary;
}

kint KoreApplication::argc() const
{
    return _argc;
}

kchar** KoreApplication::argv() const
{
    return _argv;
}

kbool KoreApplication::isClosing() const
{
    return _closing;
}

void KoreApplication::exit( kint exitCode )
{
    _closing = true;
    QCoreApplication::exit( exitCode );
}

void KoreApplication::quit()
{
    this->exit( 0 );
}

KoreApplication* KoreApplication::Instance()
{
    return _Instance;
}

QString KoreApplication::Version()
{
    return QString( _KORE_VERSION );
}

KoreApplication* KoreApplication::_Instance = K_NULL;
