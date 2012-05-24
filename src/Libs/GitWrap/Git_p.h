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

#include "Git.h"

namespace Git
{

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
	{ return d == o.d; }

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

	// we have our own ref count and go out of scope when it's 0
	#define OWN_REF() \
		void ref(){ mRefCounter.ref(); } \
		void deref(){ if( !mRefCounter.deref() ) delete this; } \
		QAtomicInt mRefCounter;

	// we use another one's ref count. he'll kill us if he's about to bite the dust
	#define PROXY_REF(proxyType, proxy) \
		void ref(){ proxy->ref(); } \
		void deref(){ proxy->deref(); }

	class RepositoryPrivate;
	class IndexPrivate;
	class ObjectPrivate;

	class Signature;

	Signature git2Signature( const git_signature* gitsig );
	git_signature* signature2git( const Signature& sig );

	class RepositoryPrivate
	{
	public:
		OWN_REF();
	public:
		RepositoryPrivate( git_repository* repo );
		~RepositoryPrivate();

		void setError( int rc );

		friend int status_callback( const char* name, unsigned int status, void* );

	public:
		git_repository*		mRepo;
		IndexPrivate*		mIndex;
	};

	class IndexPrivate
	{
	public:
		OWN_REF()

	public:
		IndexPrivate( RepositoryPrivate* repo, git_index* index );
		~IndexPrivate();

	public:
		RepositoryPrivate*	mRepo;
		git_index*			mIndex;
	};

	class ObjectPrivate
	{
	public:
		OWN_REF()

	public:
		ObjectPrivate( RepositoryPrivate* repo, git_object* o );
		~ObjectPrivate();

	public:
		RepositoryPrivate*	mRepo;
		git_object*			mObj;
	};

	class ReferencePrivate
	{
	public:
		OWN_REF()

	public:
		ReferencePrivate( RepositoryPrivate* repo, git_reference* ref );
		~ReferencePrivate();

	public:
		RepositoryPrivate*	mRepo;
		git_reference*		mRef;
	};

	class RevisionWalkerPrivate
	{
	public:
		OWN_REF()

	public:
		RevisionWalkerPrivate( RepositoryPrivate* repo, git_revwalk* walker );
		~RevisionWalkerPrivate();

	public:
		RepositoryPrivate*	mRepo;
		git_revwalk*		mWalker;
	};

}

#endif
