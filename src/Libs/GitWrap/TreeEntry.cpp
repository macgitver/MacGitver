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

#include "TreeEntry.h"
#include "TreeEntryPrivate.h"

namespace Git
{

	TreeEntryPrivate::TreeEntryPrivate( git_tree_entry* entry, bool unmanaged )
		: mEntry( entry )
		, mUnmanaged( unmanaged )
	{
		Q_ASSERT( mEntry );
	}

	TreeEntryPrivate::~TreeEntryPrivate()
	{
		if( !mUnmanaged )
		{
			git_tree_entry_free( mEntry );
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

}
