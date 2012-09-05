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

#include <cstdlib>

#include <QtCore/QtGlobal>

/*!
 * @def K_NULL
 * @brief Null pointer.
 */
#define K_NULL 0

#define K_NULL_REF(type) (reinterpret_cast<type*>(K_NULL))

/*!
 * @typedef ksize
 * @brief Size type.
 */
typedef size_t ksize;
/*!
 * @typedef kindex
 * @brief Index type.
 */
typedef size_t kindex;

/*!
 * @typedef kvoid
 * @brief Void type.
 */
typedef void kvoid;
/*!
 * @typedef kid
 * @brief ID type.
 *
 * 64 bits unique ID used in serialization (pointer).
 */
typedef quint64 kid;

/*!
 * @typedef khash
 * @brief Hash type (32 bits).
 */
typedef unsigned int khash;

/*!
 * @typedef kbyte
 * @brief Byte type.
 */
typedef unsigned char kbyte;

/*!
 * @typedef kbool
 * @brief Boolean type,
 */
typedef bool kbool;

/*!
 * @typedef kuchar
 * @brief Unsigned char type.
 */
typedef unsigned char kuchar;
/*!
 * @typedef kchar
 * @brief Signed char type.
 */
typedef char kchar;

/*!
 * @typedef kulong
 * @brief Unsigned long type.
 */
typedef unsigned long kulong;
/*!
 * @typedef klong
 * @brief Signed long type.
 */
typedef long klong;

/*!
 * @typedef kuint
 * @brief Unsigned int type.
 */
typedef unsigned int kuint;
/*!
 * @typedef kint
 * @brief Signed int type.
 */
typedef int kint;

/*!
 * @typedef kushort
 * @brief Unsigned short type.
 */
typedef unsigned short kushort;
/*!
 * @typedef kshort
 * @brief Signed short type.
 */
typedef short kshort;

/*!
 * @typedef kint64
 * @brief Signed 64-bit integer type.
 */
typedef long long kint64;

/*!
 * @typedef kuint64
 * @brief Unsigned 64-bit integer type.
 */
typedef unsigned long long kuint64;

/*!
 * @typedef kfloat
 * @brief 32bit floating point type.
 */
typedef float kfloat;

/*!
 * @typedef kdouble
 * @brief 64bit floating point type.
 */
typedef double kdouble;

#include <Macros.hpp>
