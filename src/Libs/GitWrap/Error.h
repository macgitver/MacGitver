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

#ifndef GIT_ERROR_H
#define GIT_ERROR_H

#include <QString>

#include "GitWrap.h"

namespace Git
{

    /**
	 * @ingroup		GitWrap
	 * @brief		Wraps the git_error
	 *
     */
	class GITWRAP_API Error
	{
	public:
		Error( const QString& errorText );
		Error( const Error& other );
		~Error();

	public:
		QString text() const;

	private:
		QString		mErrorText;
	};

	/**
	 * @ingroup		GitWrap
	 * @brief		Encapsulates a result
	 */
	class GITWRAP_API Result
	{
	public:
		Result();
		Result( const Result& other );
		explicit Result( int resultCode );
		~Result();
		Result& operator=( const Result& other );
		Result& operator=( int );

	public:
		operator bool() const;

		int errorCode() const;
		int errorClass() const;
		const QString& errorText() const;

	public:
		void setError( int resultCode );

	public:
		static Result invalidObject();
		static Result success();

	private:
		int			mCode;
		int			mClass;
		QString		mText;
	};

	inline Result::Result()
		: mCode( 0 )
	{
	}

	inline Result::Result( const Result& other )
	{
		if( ( mCode = other.mCode ) )
		{
			mClass = other.mClass;
			mText = other.mText;
		}
	}

	inline Result::~Result()
	{
	}

	inline Result& Result::operator=( const Result& other )
	{
		if( ( mCode = other.mCode ) )
		{
			mClass = other.mClass;
			mText = other.mText;
		}

		return *this;
	}

	inline Result::operator bool() const
	{
		return mCode == 0;
	}

	inline int Result::errorCode() const
	{
		return mCode;
	}

	inline int Result::errorClass() const
	{
		return mClass;
	}

	inline const QString& Result::errorText() const
	{
		return mText;
	}

}

#endif

