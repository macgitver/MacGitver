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
#include "RefSpec.h"

namespace Git
{

	RefSpec::RefSpec()
	{
	}

	RefSpec::RefSpec( const RefSpec& other )
		: mSrc( other.mSrc )
		, mDst( other.mDst )
	{
	}

	RefSpec::RefSpec( const QString& source, const QString& destination )
		: mSrc( source )
		, mDst( destination )
	{
	}

	QString RefSpec::source() const
	{
		return mSrc;
	}

	QString RefSpec::destination() const
	{
		return mDst;
	}

	RefSpec mkRefSpec( const git_refspec* refspec )
	{
		if( !refspec )
		{
			return RefSpec();
		}

		QString src = QString::fromUtf8( git_refspec_src( refspec ) );
		QString dst = QString::fromUtf8( git_refspec_dst( refspec ) );
		return RefSpec( src, dst );
	}

}
