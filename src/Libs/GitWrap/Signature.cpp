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
#include "Signature.h"

namespace Git
{

	Signature git2Signature( const git_signature* gitsig )
	{
		QDateTime dt = QDateTime::fromTime_t( gitsig->when.time );
		dt.setUtcOffset( gitsig->when.offset * 60 );

		return Signature(
			QString::fromUtf8( gitsig->name ),
			QString::fromUtf8( gitsig->email ),
			dt );
	}

	git_signature* signature2git( const Signature& sig )
	{
		git_signature* gitsig = 0;

		int rc = git_signature_new( &gitsig,
									sig.name().toUtf8().constData(),
									sig.email().toUtf8().constData(),
									sig.when().toTime_t(),
									sig.when().utcOffset() / 60 );

		if( rc < GIT_OK )
		{
			return NULL;
		}

		return gitsig;
	}

}

