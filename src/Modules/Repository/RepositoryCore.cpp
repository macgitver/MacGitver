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

#include "GitWrap/Object.h"
#include "GitWrap/ObjectCommit.h"
#include "GitWrap/ObjectTag.h"

#include "RepositoryCore.h"

RepositoryCore::RepositoryCore()
{
}

RepositoryCore::~RepositoryCore()
{
}

void RepositoryCore::setRepository( Git::Repository repo )
{
	mRepo = repo;
}

bool RepositoryCore::createBranch( const Git::ObjectId& baseOID, const QString& branchName,
								   bool force )
{
	Git::Object obj = mRepo.lookup( baseOID );

	if( !obj.isValid() )
	{
		return false;
	}
	else if( obj.type() == Git::otCommit )
	{
		obj.asCommit().createBranch( branchName, force );
		return true;
	}
	else if( obj.type() == Git::otTag )
	{
		//obj.asTag().createBranch( branchName, force );
		//return true;
	}

	return false;
}
