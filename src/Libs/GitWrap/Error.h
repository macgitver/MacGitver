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

#include "Git.h"

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

	class GITWRAP_API ResultCode
	{
	public:
		ResultCode();
		ResultCode( const ResultCode& other );
		explicit ResultCode( int resultCode );
		~ResultCode();
		ResultCode& operator=( const ResultCode& other );
		ResultCode& operator=( int );

	public:
		operator bool() const;

		int errorCode() const;
		int errorClass() const;
		const QString& errorText() const;

	public:
		void setError( int resultCode );

	public:
		static ResultCode invalidObject();
		static ResultCode success();

	private:
		int			mCode;
		int			mClass;
		QString		mText;
	};

	inline ResultCode::ResultCode()
		: mCode( 0 )
	{
	}

	inline ResultCode::ResultCode( const ResultCode& other )
	{
		if( ( mCode = other.mCode ) )
		{
			mClass = other.mClass;
			mText = other.mText;
		}
	}

	inline ResultCode::~ResultCode()
	{
	}

	inline ResultCode& ResultCode::operator=( const ResultCode& other )
	{
		if( ( mCode = other.mCode ) )
		{
			mClass = other.mClass;
			mText = other.mText;
		}

		return *this;
	}

	inline ResultCode::operator bool() const
	{
		return mCode == 0;
	}

	inline int ResultCode::errorCode() const
	{
		return mCode;
	}

	inline int ResultCode::errorClass() const
	{
		return mClass;
	}

	inline const QString& ResultCode::errorText() const
	{
		return mText;
	}

}

#endif

