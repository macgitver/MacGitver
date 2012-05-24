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

#include <QtPlugin>

#include "Interfaces/IMainWindow.h"

#include "RefsViewsModule.h"
#include "TagsView.h"
#include "BranchesView.h"
#include "RefsView.h"

RefsViewsModule::RefsViewsModule()
	: mBranchesView( NULL )
	, mTagsView( NULL )
	, mRefsView( NULL )
{
}

void RefsViewsModule::repositoryChanged( Git::Repository newRepository )
{
	if( mBranchesView )
	{
		mBranchesView->repositoryChanged( newRepository );
	}
	if( mTagsView )
	{
		mTagsView->repositoryChanged( newRepository );
	}
	if( mRefsView )
	{
		mRefsView->repositoryChanged( newRepository );
	}
}

void RefsViewsModule::setupConfigPages( IConfigDialog* dialog )
{
}

Module::Types RefsViewsModule::providesModuleTypes() const
{
	return View;
}

void RefsViewsModule::initialize()
{
	mBranchesView = new BranchesView();
	mainWindow()->integrateView( mBranchesView, Heaven::Left );

	mTagsView = new TagsView();
	mainWindow()->integrateView( mTagsView, Heaven::Left );

	mRefsView = new RefsView();
	mainWindow()->integrateView( mRefsView, Heaven::Left );
}

void RefsViewsModule::deinitialize()
{
	delete mBranchesView;
	delete mTagsView;
	delete mRefsView;
	mBranchesView = NULL;
	mTagsView = NULL;
	mRefsView = NULL;
}

Q_EXPORT_PLUGIN2( RefsViews, RefsViewsModule )
