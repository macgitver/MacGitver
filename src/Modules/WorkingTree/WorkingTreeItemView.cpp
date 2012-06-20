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

#include "WorkingTreeItemView.h"
#include "WorkingTreeModel.h"

WorkingTreeItemView::WorkingTreeItemView()
{
	mModel = new WorkingTreeModel( Git::Repository(), this );
	setModel( mModel );
}

void WorkingTreeItemView::setRepository( const Git::Repository& repo )
{
	mRepo = repo;
	if( mRepo.isValid() )
	{
		mModel->setRepository( mRepo );
	}
	else
	{
		setModel( NULL );
		delete mModel;
		mModel = NULL;
	}
}

WorkingTreeFilters WorkingTreeItemView::filters() const
{
	return mFilters;
}

void WorkingTreeItemView::setFilter( WorkingTreeFilters filters )
{
	mFilters = filters;
	mModel->setFilters( filters );
}
