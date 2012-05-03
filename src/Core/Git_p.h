
#ifndef GIT_P_H
#define GIT_P_H

#include "git2.h"

#include "Core/Git.h"

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
			if( x ) d->deref();
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

	#define OWN_REF() \
		void ref(){ mRef.ref(); } \
		void deref(){ if( !mRef.deref() ) delete this; } \
		QAtomicInt mRef;

	#define PROXY_REF(proxyType, proxy) \
		void ref(){ proxy->ref(); } \
		void deref(){ proxy->deref(); }

	class RepositoryPrivate
	{
	public:
		OWN_REF();
	public:
		RepositoryPrivate( git_repository* repo );
		~RepositoryPrivate();

		void setError( int rc );

	public:
		git_repository*		mRepo;
		IndexPrivate*		mIndex;
	};

	class IndexPrivate
	{
	public:
		PROXY_REF(RepositoryPrivate, mRepo );
	public:
		IndexPrivate( RepositoryPrivate* repo, git_index* index );
		~IndexPrivate();

	public:
		RepositoryPrivate*	mRepo;
		git_index*			mIndex;
	};

}

#endif
