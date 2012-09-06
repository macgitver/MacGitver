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

#include "Config/Config.h"

#include "Widgets/HeaderView.h"

#include "WorkingTreeItemView.h"
#include "WorkingTreeModel.h"

WorkingTreeItemView::WorkingTreeItemView()
{
	mModel = NULL;

	QVariant v = Config::self().get( QLatin1String( "Worktree/Filters" ),
									 int( WTF_All ) );
	mFilters = WorkingTreeFilters( v.toInt() );
}

void WorkingTreeItemView::setRepository( const Git::Repository& repo )
{
	if( mModel )
	{
		setModel( NULL );
		delete mModel;
		mModel = NULL;
	}

	mRepo = repo;

	if( mRepo.isValid() )
	{
		mModel = new WorkingTreeModel( Git::Repository(), this );
		mModel->setRepository( mRepo );
		mModel->setFilters( mFilters );
		setModel( mModel );
	}

	HeaderView* hv = new HeaderView( Qt::Horizontal );
	setHeader( hv );
	hv->setConfigName( QLatin1String( "Worktree/Columns" ) );
}

WorkingTreeFilters WorkingTreeItemView::filters() const
{
	return mFilters;
}

void WorkingTreeItemView::setFilter( WorkingTreeFilters filters )
{
	mFilters = filters;
	if( mModel )
	{
		mModel->setFilters( filters );
	}

	Config::self().set( QLatin1String( "Worktree/Filters" ), int( mFilters ) );
}
