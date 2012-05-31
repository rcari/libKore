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

template<typename T>
QList<T*> Kore::data::Library::findChildren(int maxDepth)
{
	QList<T*> result;
	if(this->fastInherits<T>())
	{
		result.append(static_cast<T*>(this));
	}
	for(kint i = 0; maxDepth != 0 && i < _blocks.size(); i++)
	{
		if(_blocks.at(i)->isLibrary())
		{
			result.append(static_cast<Library*>(_blocks.at(i))->findChildren<T>(maxDepth-1));
		}
		else if(_blocks.at(i)->fastInherits<T>())
		{
			result.append(static_cast<T*>(_blocks.at(i)));
		}
	}
	return result;
}

template<typename T>
QList<const T*> Kore::data::Library::findChildrenConst(int maxDepth) const
{
	QList<const T*> result;
	if(this->fastInherits<T>())
	{
		result.append(static_cast<const T*>(this));
	}
	for(kint i = 0; maxDepth != 0 && i < _blocks.size(); i++)
	{
		if(_blocks.at(i)->isLibrary())
		{
			result.append(static_cast<const Library*>(_blocks.at(i))->findChildrenConst<T>(maxDepth-1));
		}
		else if(_blocks.at(i)->fastInherits<T>())
		{
			result.append(static_cast<const T*>(_blocks.at(i)));
		}
	}
	return result;
}
