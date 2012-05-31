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

#include <intrin.h>
#include <cstring>
using namespace std;

CPU::CPU() {
	init();
}

CPU::~CPU() {
}

void CPU::init() {
	// Get the parameters.
	int registers[4];
	__cpuid(registers, 0x00000001);

	_MMX = registers[3] & MMX_SUPPORTED;
	_SSE = registers[3] & SSE_SUPPORTED;
	_SSE2 =	registers[3] & SSE2_SUPPORTED;
	_SSE3 = registers[2] & SSE3_SUPPORTED;
	_SSE4 = registers[2] & SSE4_SUPPORTED;
	_3DNow = registers[3] & AMD_3DNOW_SUPPORTED;

	// Vendor string.
	for(int i = 0; i < 13; i++) {
		_vendorString[i] = 0x00;
	}

	__cpuid(registers, 0x80000001);

	_3DNow2 = registers[3] & AMD_3DNOW_EX_SUPPORTED;
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
