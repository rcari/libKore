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

#include <plugin/Loadable.hpp>

#include <QtCore/QAtomicInt>
#include <QtCore/QMetaClassInfo>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>

#include <QtCore/QMultiHash>
#include <QtCore/QVector>

namespace Kore { namespace data {

class BlockExtension;

class KoreExport MetaBlock : public Kore::plugin::Loadable
{
    Q_OBJECT

    friend class Block;
    friend class BlockExtension;

protected:
    MetaBlock( const QMetaObject* mo );

    virtual void library( Kore::data::Library* lib );

public:
    virtual bool canUnload() const; // Loadable !!

    virtual QString iconPath() const;

    virtual Block* createBlock() const = K_NULL;
    template< typename T >
    T* createBlockT() { return static_cast< T* >( createBlock() ); }

    inline const QMetaObject* blockMetaObject() const
        { return _blockMetaObject; }
    inline QString blockClassName() const
        { return QLatin1String( _blockMetaObject->className() ); }

    inline khash blockClassID() const
    {
        K_ASSERT( _blockClassID != K_NULL )
        return _blockClassID;
    }

    inline khash propertyID( kint property ) const
    {
        K_ASSERT( !_propertiesHashes.isEmpty() )
        return _propertiesHashes.at( property );
    }

    inline kint propertyIndex( khash propertyHash ) const
    {
        K_ASSERT( ! _propertiesHashes.isEmpty() )
        return _propertiesHashes.indexOf( propertyHash );
    }

    virtual QString blockIconPath() const = K_NULL;
    virtual QVariant blockProperty( kint property ) const = K_NULL;
    QVariant blockSetting( const QString& setting,
                           const QVariant& defaultValue ) const;

    BlockExtension* blockExtension( const QString& name ) const;
    QList< BlockExtension* > blockExtensions( const QString& name ) const;
    const QMultiHash< QString, BlockExtension* >& extensions() const;

    MetaBlock* superMetaBlock();
    const MetaBlock* superMetaBlock() const;

protected:
    inline void setBlockAllocated( Block* b ) const
        { b->addFlag( Block::Allocated ); }
    kbool registerBlockExtension( BlockExtension* extension );
    void unregisterBlockExtension( BlockExtension* extension );

    virtual void destroyBlock( Block* b ) const;

    inline void ref() const { while( ! _instancesCount.ref() ) {} }
    inline void deref() const { while( ! _instancesCount.deref() ) {} }

private:
    void createPropertiesCache() const;
    void clearExtensions();

private:
    const QMetaObject* _blockMetaObject;
    mutable MetaBlock* _superMetaBlock;

    mutable khash _blockClassID;
    mutable QVector< khash > _propertiesHashes;

    mutable QAtomicInt _instancesCount;
    QMultiHash< QString, BlockExtension* > _extensions;
};

}}
