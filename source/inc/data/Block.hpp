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
#include <data/PointerTypes.hpp>

#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace Kore { namespace data {

class Library;
class MetaBlock;

/*!
 * @brief A Block is the root object of the the Kore library class hierarchy.
 *
 * Blocks are organized in memory as a tree. This allows operations such as
 * serialization to be performed simply and quickly. Moreover, this also
 * simplifies memory management aspects as the deletion of a node of the tree
 * triggers the proper cleanup of all its child nodes recursively.
 *
 * \sa	Kore::data::Library
 */
class KoreExport Block : public QObject
{

    Q_OBJECT
    Q_PROPERTY( QString blockName
                    READ blockName WRITE blockName NOTIFY blockNameChanged
                    STORED false DESIGNABLE false USER true )

    friend class Library;
    friend class MetaBlock;

public:
    /*!
     * @enum	Flags.
     *
     * The different flags to mark a Block with.
     */
    enum Flags
    {
        /// The block was allocated by its allocator
        Allocated =         0x1 << 0,
        /// The block is currently being deleted
        IsBeingDeleted =    0x1 << 1,
        /// The block is Serializable: should be considered for serialization.
        Serializable =      0x1 << 2,
        /// The block is browsable, understand GUI navigable.
        Browsable =         0x1 << 3,
        /// The block is editable, users can modify it.
        Editable =          0x1 << 4,
        /// The block is owned by the system (can not be deleted by the user).
        SystemOwned =       0x1 << 5,
        /// The block is a system library (block, factories...)
        System =            0x1 << 6,
        MAX_FLAG =          0x1 << 7
    };

    enum Properties
    {
        BlockTypeName = 0x1,
        BlockPropertiesName,
        BlockSettings,
        MAX_PROPERTY
    };

protected:
    /*!
     * @brief Default constructor.
     *
     * This default constructor is protected as a Block should not be
     * instantiated as-is but rather sub-classed and specialized.
     */
    Block();
    /*!
     * @brief Initialization routine.
     *
     * Because some generic initialization is not possible in the base
     * constructor (the object is not fully built), this virtual method call
     * allows to do some generic post instantiation initialization.
     * All overrides must call the parent implementation.
     */
    virtual void initialize();

public:
    virtual ~Block();

public:
    /*!
     * @property Block::index
     * @brief The index is the position of this Block in its parent Library.
     */
    inline kint index() const { return _index; }
protected:
    virtual void index( kint idx );

    /*!
     * @property Block::library
     * @brief The library is the parent node of this Block.
     * Classes that inherit Block can customize the behavior upon
     * insertion/removal in a tree hierarchy by customizing this method.
     */
    virtual void library( Library* lib );
public:
    inline Library* library() { return _library; }
    inline const Library* library() const { return _library; }
    template< typename T >
    inline T* parent() { return static_cast< T* >( _library ); }
    template< typename T >
    inline const T* parent() const
        { return static_cast< const T* >( _library ); }
    inline kbool hasParent() const { return _library != K_NULL; }

    /*!
     * @brief Optimization.
     *
     * Block trees sometimes need to be optimized for performance or memory
     * footprint. This virtual method is where such optimizations should be
     * implemented.
     */
    virtual void optimize();

    QString objectClassName() const;

    virtual void blockName( const QString& name );
    virtual QString blockName() const;

    virtual QString infoString() const;
    virtual QString iconPath() const;

    /*!
     * @brief Check the Block for a flag.
     *
     * Convenience method.
     *
     * @param[ in ] flag Flag to be checked for.
     * @return      true on success, false otherwise
     */
    inline kbool checkFlag( kuint flag ) const
        { return ( _flags & flag ) != 0; }

    /*!
     * @brief Block unique ID.
     *
     * This is derived from the memory address of the Block at runtime.
     *
     * @return	The Block unique ID.
     */
    kid ID() const;

    virtual kbool isLibrary() const { return false; }
    inline kbool isBeingDeleted() const { return checkFlag( IsBeingDeleted ); }

    /*!
     * @brief The Block's instantiator.
     *
     * This method returns a pointer to the BlockFactory that instantiated this
     * object.
     *
     * @return BlockFactory* the object's factory.
     */
    //const BlockFactory* factory() const;

    /*!
     * @brief	The object's meta block.
     *
     * This method returns a const pointer to the MetaBlock of the object.
     *
     * @return MetaBlock* the object's meta block.
     */
    virtual const MetaBlock* metaBlock() const;

    template< typename T >
    kbool fastInherits() const;

    kbool fastInherits( const MetaBlock* mb ) const;

protected:
    /*!
     * @brief	Adds a flag to the Block.
     *
     * Convenience method.
     *
     * @param[in] flag	Flag to be added.
     */
    void addFlag( kuint flag );
    /*!
     * @brief	Removes a flag from the Block.
     *
     * Convenience method.
     *
     * @param[in] flag	Flag to be removed.
     */
    void removeFlag( kuint flag );

signals:
    void blockNameChanged( const QString& name );
    void blockInserted();
    void blockRemoved();
    void blockDeleted();
    void indexChanged( kint oldIndex, kint newIndex );

public slots:
    /*!
     * @brief	Destroy the block.
     */
    virtual bool destroy();

public:
    static QVariant DefaultBlockProperty( kint property );

private:
    // Better alignment: pointers first (32/64...)
    Library*    _library;	//! The parent library
    // Members afterwards
    kuint       _flags;		//!	The block flags
    kint        _index;		//! The block Index of this Block in its Library.
};

} /* namespace data */ } /* namespace Kore */

#include <src/data/Block.cxx>

#include "MetaBlock.hpp"

#define K_BLOCK friend class Kore::data::Block;\
    public:\
        virtual QString iconPath() const;\
    private:\
        static const QMetaObject* QtMetaObject() { return &staticMetaObject; }\
    class PrivateMetaBlock : public Kore::data::MetaBlock {\
    private:\
        PrivateMetaBlock();\
    public:\
        Kore::data::Block* instantiate() const;\
        template< typename T >\
        inline T* instantiateT() const\
            { return static_cast< T* >( instantiate() ); }\
        virtual Kore::data::Block* createBlock() const\
            { return instantiate(); }\
        virtual QString blockIconPath() const;\
        virtual QVariant blockProperty( kint property ) const;\
        static Kore::plugin::Loadable* Instance()\
            { return ( _Instance != K_NULL )\
                ? _Instance\
                : _Instance = new PrivateMetaBlock(); }\
    private:\
        static PrivateMetaBlock* _Instance;\
        static bool _Registered;\
    };\
    public:\
        static PrivateMetaBlock* StaticMetaBlock()\
            { return static_cast< PrivateMetaBlock* >(\
                PrivateMetaBlock::Instance() ); }\
        virtual Kore::data::MetaBlock* metaBlock() const\
            { return static_cast< Kore::data::MetaBlock* >(\
                PrivateMetaBlock::Instance() ); }\
    private:

#define K_BLOCK_CREATE_INSTANCE( block )\
    ( block::StaticMetaBlock()->instantiateT<block>() )
