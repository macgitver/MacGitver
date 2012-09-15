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
#include "TreeBuilder.h"
#include "TreeBuilderPrivate.h"
#include "ObjectId.h"

#include "RepositoryPrivate.h"

namespace Git
{

	BEGIN_INTERNAL_IMPL()

	TreeBuilderPrivate::TreeBuilderPrivate( RepositoryPrivate* repo, git_treebuilder* builder )
		: RepoObject( repo )
		, mBuilder( builder )
	{
		Q_ASSERT( mBuilder );
	}

	TreeBuilderPrivate::~TreeBuilderPrivate()
	{
		git_treebuilder_free( mBuilder );
	}

	END_INTERNAL_IMPL()

	TreeBuilder::TreeBuilder()
	{
	}

	TreeBuilder::TreeBuilder( const TreeBuilder& other )
		: d( other.d )
	{
	}

	TreeBuilder::TreeBuilder( Internal::TreeBuilderPrivate* _d )
		: d( _d )
	{
	}

	TreeBuilder::~TreeBuilder()
	{
	}

	TreeBuilder& TreeBuilder::operator=( const TreeBuilder& other )
	{
		d = other.d;
		return * this;
	}

	bool TreeBuilder::isValid() const
	{
		return d;
	}

	void TreeBuilder::clear( Result& result )
	{
		if( !result )
		{
			return;
		}

		if( !d )
		{
			result.setInvalidObject();
			return;
		}

		git_treebuilder_clear( d->mBuilder );
	}

	bool TreeBuilder::remove( const QString& fileName, Result& result )
	{
		if( !result )
		{
			return false;
		}

		if( !d )
		{
			result.setInvalidObject();
			return false;
		}


		result = git_treebuilder_remove( d->mBuilder, fileName.toUtf8().constData() );
		return result;
	}

	bool TreeBuilder::insert( const QString& fileName, TreeEntryAttributes type,
							  const ObjectId& oid, Result& result )
	{
		if( !result )
		{
			return false;
		}

		if( !d )
		{
			result.setInvalidObject();
			return false;
		}

		const git_tree_entry* te = NULL;
		git_filemode_t fm = Internal::teattr2filemode( type );

		result = git_treebuilder_insert( &te, d->mBuilder, fileName.toUtf8().constData(),
										 (const git_oid*) oid.raw(), fm );

		/* ignoring the returned tree-entry pointer; can be fetched by 'get' */
		return result;
	}

	ObjectId TreeBuilder::write( Result& result )
	{
		if( !result )
		{
			return ObjectId();
		}

		if( !d )
		{
			result.setInvalidObject();
			return ObjectId();
		}

		git_oid oid;
		result = git_treebuilder_write( &oid, d->repo()->mRepo, d->mBuilder );

		if( !result )
		{
			return ObjectId();
		}

		return ObjectId::fromRaw( oid.id );
	}


	TreeEntry TreeBuilder::get( const QString& name, Result& result )
	{
		if( !result )
		{
			return TreeEntry();
		}

		if( !d )
		{
			result.setInvalidObject();
			return TreeEntry();
		}

		const git_tree_entry* entry = git_treebuilder_get( d->mBuilder, name.toUtf8().constData() );
		if( !entry )
		{
			return TreeEntry();
		}

		return new Internal::TreeEntryPrivate( entry, true );
	}

}
