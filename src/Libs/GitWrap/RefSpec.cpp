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

	RefSpec::RefSpec( const QByteArray& source, const QByteArray& destination )
		: mSrc( source )
		, mDst( destination )
	{
	}

	QByteArray RefSpec::source() const
	{
		return mSrc;
	}

	QByteArray RefSpec::destination() const
	{
		return mDst;
	}

	RefSpec mkRefSpec( const git_refspec* refspec )
	{
		if( !refspec )
		{
			return RefSpec();
		}

		QByteArray src = git_refspec_src( refspec );
		QByteArray dst = git_refspec_dst( refspec );
		return RefSpec( src, dst );
	}

}
