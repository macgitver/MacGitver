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

#include <QDebug>

#include "GitWrapPrivate.h"

#include "Error.h"

namespace Git
{

	Error::Error( const QString& errorText )
		: mErrorText( errorText )
	{
	}

	Error::Error( const Error& other )
		: mErrorText( other.mErrorText )
	{
	}

	Error::~Error()
	{
	}

	QString Error::text() const
	{
		return mErrorText;
	}

	/**
	 * @internal
	 */
	Result::Result( int resultCode )
	{
		setError( resultCode );
	}

	/**
	 * @internal
	 */
	void Result::setInvalidObject()
	{
		mCode = -255;
		mClass = -1;
		mText = QLatin1String( "An invalid GitWrap object was used." );
	}

	/**
	 * @internal
	 * @brief		_Import_ an error from `libgit2`
	 *
	 * @param[in]	resultCode	The result code as returned from `libgit2`
	 *
	 */
	void Result::setError( int resultCode )
	{
		mCode = resultCode;
		if( mCode < 0 )
		{
			const git_error* err = giterr_last();
			if( err )
			{
				mClass = err->klass;
				mText = QString::fromUtf8( err->message );
			}
			else
			{
				qDebug() << "Notice: LibGit2 returned" << mCode << "but did not supply a "
							"suitable error message.";
			}
		}
	}

	/**
	 * @internal
	 * @brief		Assignment operator
	 *
	 * @param[in]	resultCode	The result code as returned from `libgit2`
	 *
	 * @return		A reference to this result object
	 *
	 * Just a short-cut for setError()
	 */
	Result& Result::operator =( int resultCode )
	{
		setError( resultCode );
		return *this;
	}

}
