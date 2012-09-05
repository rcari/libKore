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

#pragma once

#include <KoreExport.hpp>
#include <Types.hpp>

namespace Kore { namespace system {

/*!
 * @class CPU
 * @brief Gets all information about the CPU.
 */
class KoreExport CPU {

public:
	/*!
	 * @brief Constructor of the CPU class.
	 */
	CPU();
	/*!
	 * @brief Destructor of the CPU class.
	 */
	~CPU();

	/*!
	 * @brief Gets the number of CPUs of the computer.
	 */
	ksize	getCPUsCount() const;
	/*!
	 * @brief Returns true if CPUs are MMX (MultiMedia eXtensions) compliant, false otherwise.
	 */
	kbool	isMMXEnabled() const;
	/*!
	 * @brief Returns true if CPUs are SSE (Streaming SIMD Extensions) compliant, false otherwise.
	 */
	kbool	isSSEEnabled() const;
	/*!
	 * @brief Returns true if CPUs are SSE2 compliant, false otherwise.
	 */
	kbool	isSSE2Enabled() const;
	/*!
	 * @brief Returns true if CPUs are SSE3 compliant, false otherwise.
	 */
	kbool	isSSE3Enabled() const;
	/*!
	 * @brief Returns true if CPUs are SSE4 compliant, false otherwise.
	 */
	kbool	isSSE4Enabled() const;
	/*!
	 * @brief Returns true if CPUs are 3DNow compliant, false otherwise.
	 */
	kbool	is3DNowEnabled() const;
	/*!
	 * @brief Returns true if CPUs are 3DNowEx compliant, false otherwise.
	 */
	kbool	is3DNow2Enabled() const;
	/*!
	 * @brief Gets the vendor's name of CPUs.
	 */
	const kchar* getVendorString() const;

private:
	void init();

	ksize	_cpusCount;
	kbool	_MMX;
	kbool	_SSE;
	kbool	_SSE2;
	kbool	_SSE3;
	kbool	_SSE4;
	kbool	_3DNow;
	kbool	_3DNow2;
	kchar	_vendorString[12+1];
};

}}

