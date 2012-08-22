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

#include <memory/SimpleMemoryManager.hpp>
using namespace Kore::memory;

#include <QtCore/QObject>

#include <stdlib.h>

SimpleMemoryManager::SimpleMemoryManager()
{
	qDebug("Kore / Created Simple Memory Manager");
	blockName("Simple Memory Manager");
	removeFlag(Block::Serializable); // A memory manager is not serializable.
	addFlag(Block::System);
}

void* SimpleMemoryManager::mAlloc(ksize sz) const
{
	return malloc(sz);
}

void* SimpleMemoryManager::mAlloc_a(ksize sz, ksize align) const
{

	kbyte* data = static_cast<kbyte*>( malloc(sz + align + sizeof(void*)) );

	kbyte* alignedPtr = data + sizeof(void*);
	alignedPtr = (kbyte*)_K_NEXT_ALIGNED_VALUE( alignedPtr, align );

	void** ptr = reinterpret_cast<void**>(alignedPtr);
	ptr[-1] = data;

	K_ASSERT( K_IS_ALIGNED(alignedPtr, align) )

	return alignedPtr;
}

void SimpleMemoryManager::mFree(void* ptr) const
{
	free(ptr);
}

void SimpleMemoryManager::mFree_a(void* ptr) const
{
	void** p = static_cast<void**>(ptr);
	free(p[-1]);
}

void* SimpleMemoryManager::mReAlloc(void* ptr, ksize sz) const
{
	return realloc(ptr, sz);
}

void* SimpleMemoryManager::mReAlloc_a(void* ptr, ksize sz, ksize align) const
{
	void** p = static_cast<void**>(ptr);

	kbyte* data = static_cast<kbyte*>( realloc(p[-1], sz + align + sizeof(void*)) );

	kbyte* alignedPtr = data + sizeof(void*);
	alignedPtr = (kbyte*)_K_NEXT_ALIGNED_VALUE( alignedPtr, align );

	p = reinterpret_cast<void**>(alignedPtr);
	p[-1] = data;

	K_ASSERT( K_IS_ALIGNED(alignedPtr, align) )

	return alignedPtr;
}
