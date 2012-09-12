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

#include "Git_p.h"

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

	Result::Result( int resultCode )
	{
		setError( resultCode );
	}

	void Result::setError( int resultCode )
	{
		mCode = resultCode;
		if( mCode < 0 )
		{
			const git_error* err = giterr_last();
			mClass = err->klass;	/// @todo wrap this
			mText = QString::fromUtf8( err->message );
		}
	}

	Result& Result::operator =( int resultCode )
	{
		setError( resultCode );
		return *this;
	}

}
