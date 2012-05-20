/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#include "Libs/Core/MacGitver.h"
#include "Libs/Core/MainWindow.h"

#include "Modules/RefsViews/RefsViewsModule.h"
#include "Modules/RefsViews/TagsView.h"
#include "Modules/RefsViews/BranchesView.h"
#include "Modules/RefsViews/RefsView.h"

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

void RefsViewsModule::setupConfigPages( ConfigDlg* dialog )
{
}

Module::Types RefsViewsModule::providesModuleTypes() const
{
	return View;
}

void RefsViewsModule::initialize()
{
	mBranchesView = new BranchesView();
	MacGitver::self().mainWindow()->integrateView( mBranchesView, Heaven::Left );

	mTagsView = new TagsView();
	MacGitver::self().mainWindow()->integrateView( mTagsView, Heaven::Left );

	mRefsView = new RefsView();
	MacGitver::self().mainWindow()->integrateView( mRefsView, Heaven::Left );
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


IMPLEMENT_INTERNAL_MODULE( RefsViews )
