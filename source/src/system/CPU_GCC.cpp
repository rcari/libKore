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

#include <system/CPU.hpp>
using namespace Kore::system;

// bit flags set by cpuid when called with register eax set to 1
#define MMX_SUPPORTED			0x00800000
#define SSE_SUPPORTED			0x02000000
#define SSE2_SUPPORTED			0x04000000
#define SSE3_SUPPORTED			0x00000001
#define SSE3_EX_SUPPORTED		0x00000200
#define SSE4_SUPPORTED			0x00080000
#define AMD_3DNOW_SUPPORTED		0x80000000

// AMD specific
#define AMD_3DNOW_EX_SUPPORTED	0x40000000
#define AMD_MMX_EX_SUPPORTED	0x00400000

#include <cstring>
using namespace std;

CPU::CPU() {
	init();
}

CPU::~CPU() {
}

void CPU::init() {
	// Get the parameters.
	unsigned int cpu_info = 0;
	unsigned int cpu_ssex = 0;

	asm volatile(
		"movl 	$0x00000001,	%%eax	\n\t"
		"cpuid							\n\t"
		: "=d" (cpu_info), "=c" (cpu_ssex)
		:
		: "%eax", "memory"
	);
	
	_MMX = 	cpu_info & MMX_SUPPORTED;
	_SSE = 	cpu_info & SSE_SUPPORTED;
	_SSE2 =	cpu_info & SSE2_SUPPORTED;
	_SSE3 = cpu_ssex & SSE3_SUPPORTED;
	_SSE4 = cpu_ssex & SSE4_SUPPORTED;
	_3DNow = cpu_info & AMD_3DNOW_SUPPORTED;

	// Vendor string.
	unsigned int temp[3];
	asm volatile(
		"xor	%%eax,	%%eax	\n\t"
		"cpuid					\n\t"
		"movl	%%ebx,	%0		\n\t"
		: "=q" (temp[0]), "=d" (temp[1]), "=c" (temp[2])
	);

	for(int i = 0; i < 13; i++) {
		_vendorString[i] = 0x00;
	}
	//memset(_vendorString, 0x00, 13);
	//memcpy(_vendorString, &temp[0], 12);

	asm volatile(
		"movl	$0x80000001,	%%eax	\n\t"
		"cpuid							\n\t"
		: "=d" (cpu_info)
		:
		: "%eax"
	);

	_3DNow2 = cpu_info & AMD_3DNOW_EX_SUPPORTED;
}

ksize	CPU::getCPUsCount() const {
	return _cpusCount;
}

kbool	CPU::isMMXEnabled() const {
	return _MMX;
}

kbool	CPU::isSSEEnabled() const {
	return _SSE;
}

kbool	CPU::isSSE2Enabled() const {
	return _SSE2;
}

kbool	CPU::isSSE3Enabled() const {
	return _SSE3;
}

kbool	CPU::isSSE4Enabled() const {
	return _SSE4;
}

kbool	CPU::is3DNowEnabled() const {
	return _3DNow;
}

kbool	CPU::is3DNow2Enabled() const {
	return _3DNow2;
}

const kchar* CPU::getVendorString() const {
	return _vendorString;
}
