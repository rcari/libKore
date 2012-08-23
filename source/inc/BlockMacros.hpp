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

#ifndef K_MODULE_TYPE
#error You must define a module type !
#endif

#ifndef K_BLOCK_TYPE
#error You must define a block type before include <BlockMacros.hpp> !
#endif

#include "KoreEngine.hpp"

// Begin / End blocks !
#ifdef K_BLOCK_SUPER_TYPE
//#warning Defining the block super type is useless now ! // Not entirely true...
#endif

#define K_BLOCK_BEGIN K_BLOCK_TYPE::PrivateMetaBlock::PrivateMetaBlock() : MetaBlock( K_BLOCK_XSTR(K_BLOCK_TYPE) , &(K_BLOCK_TYPE::staticMetaObject)) {}\
	ksize K_BLOCK_TYPE::PrivateMetaBlock::blockSize() const { return sizeof(K_BLOCK_TYPE); }\
	K_BLOCK_TYPE::PrivateMetaBlock* K_BLOCK_TYPE::PrivateMetaBlock::_Instance = NULL;\
	bool K_BLOCK_TYPE::PrivateMetaBlock::_Registered = K_MODULE_TYPE::RegisterMetaBlockInstantiator( &(K_BLOCK_TYPE::StaticMetaBlock) );

#define K_BLOCK_END

// Icons
#define K_BLOCK_ICON( icon ) QString K_BLOCK_TYPE::iconPath() const { return QString( icon ); }\
	QString K_BLOCK_TYPE::PrivateMetaBlock::blockIconPath() const { return QString( icon ); }

#ifdef K_BLOCK_SUPER_TYPE

# define K_BLOCK_ICON_DEFAULT QString K_BLOCK_TYPE::iconPath() const { return K_BLOCK_SUPER_TYPE::iconPath(); }\
	QString K_BLOCK_TYPE::PrivateMetaBlock::blockIconPath() const { return K_BLOCK_SUPER_TYPE::StaticMetaBlock()->blockIconPath(); }

#else

# define K_BLOCK_ICON_DEFAULT QString K_BLOCK_TYPE::iconPath() const { return QString("kore/images/icons/block.png"); }\
	QString K_BLOCK_TYPE::PrivateMetaBlock::blockIconPath() const { return QString("kore/images/icons/block.png"); }

#endif

// Instantiation
#define K_BLOCK_VIRTUAL Kore::data::Block* K_BLOCK_TYPE::PrivateMetaBlock::createBlock() const { qWarning("Can not instantiate virtual block " K_BLOCK_XSTR(K_BLOCK_TYPE)); return K_NULL; }\
	Kore::data::Block* K_BLOCK_TYPE::PrivateMetaBlock::createBlock(kvoid*) const { qWarning("Can not instantiate virtual block " K_BLOCK_XSTR(K_BLOCK_TYPE)); return K_NULL; }

#define K_BLOCK_ALLOCABLE Kore::data::Block* K_BLOCK_TYPE::PrivateMetaBlock::createBlock() const { Kore::data::Block* b = new K_BLOCK_TYPE; setBlockFactory(b); return b; }\
	Kore::data::Block* K_BLOCK_TYPE::PrivateMetaBlock::createBlock(kvoid* ptr) const { Kore::data::Block* b = new (ptr) K_BLOCK_TYPE; setBlockFactory(b); return b; }

// Properties
#define K_BLOCK_PROPERTY_METHOD( propertyMethod ) QVariant K_BLOCK_TYPE::PrivateMetaBlock::blockProperty(int property) const { return propertyMethod(property); }

#define K_BLOCK_PROPERTY_DEFAULT QVariant K_BLOCK_TYPE::PrivateMetaBlock::blockProperty(int property) const { return Kore::data::Block::DefaultBlockProperty(property); }
