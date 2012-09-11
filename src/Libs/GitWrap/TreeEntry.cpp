/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ObjectId.h"
#include "TreeEntry.h"
#include "TreeEntryPrivate.h"

namespace Git
{

	TreeEntryPrivate::TreeEntryPrivate( const git_tree_entry* entry, bool unmanaged )
		: mEntry( entry )
		, mUnmanaged( unmanaged )
	{
		Q_ASSERT( mEntry );
	}

	TreeEntryPrivate::~TreeEntryPrivate()
	{
		if( !mUnmanaged )
		{
			git_tree_entry_free( const_cast< git_tree_entry* >( mEntry ) );
		}
	}

	TreeEntry::TreeEntry()
	{
	}

	TreeEntry::TreeEntry( const TreeEntry& other )
		: d( other.d )
	{
	}

	TreeEntry::TreeEntry( TreeEntryPrivate* _d )
		: d( _d )
	{
	}

	TreeEntry::~TreeEntry()
	{
	}

	TreeEntry& TreeEntry::operator=( const TreeEntry& other )
	{
		d = other.d;
		return * this;
	}

	bool TreeEntry::isValid() const
	{
		return d;
	}


	TreeEntry TreeEntry::clone() const
	{
		if( !d )
		{
			return TreeEntry();
		}

		git_tree_entry* entry = git_tree_entry_dup( d->mEntry );
		Q_ASSERT( entry );

		return new TreeEntryPrivate( entry );
	}

	ObjectId TreeEntry::sha1() const
	{
		if( !d )
		{
			return ObjectId();
		}

		const git_oid* oid = git_tree_entry_id( d->mEntry );
		if( !oid )
		{
			return ObjectId();
		}

		return ObjectId::fromRaw( oid->id );
	}

	QString TreeEntry::name() const
	{
		if( !d )
		{
			return QString();
		}

		const char* szName = git_tree_entry_name( d->mEntry );

		if( !szName )
		{
			return QString();
		}

		return QString::fromUtf8( szName );
	}

	ObjectType TreeEntry::type() const
	{
		if( !d )
		{
			return otAny;
		}

		return gitotype2ObjectType( git_tree_entry_type( d->mEntry ) );
	}

}
