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

#ifndef GIT_P_H
#define GIT_P_H

#include "git2.h"

#include "GitWrap.h"

#define BEGIN_INTERNAL_DECL()	namespace Internal {
#define END_INTERNAL_DECL()		}

#define BEGIN_INTERNAL_IMPL()	namespace Internal {
#define END_INTERNAL_IMPL()		}

namespace Git
{

	class Signature;
	class RefSpec;

	BEGIN_INTERNAL_DECL()

	template< class T >
	GitPtr< T >::GitPtr()
		: d(0)
	{}

	template< class T >
	GitPtr< T >::GitPtr( const GitPtr< T >& o )
		: d( o.d )
	{ if( d ) d->ref(); }

	template< class T >
	GitPtr< T >::GitPtr( T* o )
		: d( o )
	{ if( d ) d->ref(); }

	template< class T >
	GitPtr< T >::~GitPtr()
	{ if( d ) d->deref(); }

	template< class T >
	GitPtr< T >& GitPtr< T >::operator=( const GitPtr< T >& o )
	{
		T* x = d; d = o.d;
		if( x != d )
		{
			if( d ) d->ref();
			if( x ) x->deref();
		}
		return *this;
	}

	template< class T >
	bool GitPtr< T >::operator==( const GitPtr< T >& o ) const
	{ return d == o.d; }

	template< class T >
	bool GitPtr< T >::operator==( T* o ) const
	{ return d == d; }

	template< class T >
	T* GitPtr< T >::operator->()
	{ return d; }

	template< class T >
	const T* GitPtr< T >::operator->() const
	{ return d; }

	template< class T >
	T* GitPtr< T >::operator*()
	{ return d; }

	template< class T >
	const T* GitPtr< T >::operator*() const
	{ return d; }

	template< class T >
	GitPtr< T >::operator bool() const
	{ return d; }

	template< class T >
	GitPtr< T >::operator T*()
	{ return d; }

	template< class T >
	GitPtr< T >::operator const T*() const
	{ return d; }

	class RepositoryPrivate;
	class IndexPrivate;
	class ObjectPrivate;

	Signature git2Signature( const git_signature* gitsig );
	git_signature* signature2git( const Signature& sig );
	RefSpec mkRefSpec( const git_refspec* refspec );

	/**
	 * @internal
	 * @ingroup		GitWrap
	 * @brief		convert GitWrap's TreeEntryAttributes convert to LibGit2 file mode
	 * @param[in]	attr	GitWrap's TreeEntryAttributes
	 * @return		LibGit2's git_filemode_t
	 */
	static inline git_filemode_t teattr2filemode( TreeEntryAttributes attr )
	{
		switch( attr )
		{
		case UnkownAttr:			return GIT_FILEMODE_NEW;
		case TreeAttr:				return GIT_FILEMODE_TREE;
		case FileAttr:				return GIT_FILEMODE_BLOB;
		case FileExecutableAttr:	return GIT_FILEMODE_BLOB_EXECUTABLE;
		case GitLinkAttr:			return GIT_FILEMODE_LINK;
		case SubmoduleAttr:			return GIT_FILEMODE_COMMIT;
		}
		Q_ASSERT( false );
		// Why is there no "Q_ASSUME( false );"???
		return GIT_FILEMODE_NEW;
	}

	/**
	 * @internal
	 * @ingroup		GitWrap
	 * @brief		Convert LibGit2 object type to GitWrap object type
	 * @param[in]	otype	LibGit2's object type
	 * @return		LibGitWrap's object type
	 */
	static inline ObjectType gitotype2ObjectType( git_otype otype )
	{
		switch( otype )
		{
		case GIT_OBJ_BLOB:		return otBlob;
		case GIT_OBJ_COMMIT:	return otCommit;
		case GIT_OBJ_TREE:		return otTree;
		case GIT_OBJ_TAG:		return otTag;
		default:				Q_ASSERT( false );
								return otAny;
		}
	}

	END_INTERNAL_DECL()
}

#endif
