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

#include <QTreeView>
#include <QTextBrowser>
#include <QVBoxLayout>

#include "RepoTreeView.hpp"
#include "RepoInfoModel.hpp"

RepoTreeView::RepoTreeView()
    : GlobalView( QLatin1String( "RepoTree" ) )
{
    setViewName( trUtf8( "Repositories" ) );

    mSplitter = new Heaven::MiniSplitter( Qt::Vertical );

    mRepos = new QTreeView;
    mRepos->setFrameShape( QFrame::NoFrame );
    mRepos->setModel( new RepositoryInfoModel() );
    mSplitter->addWidget( mRepos );

    mDetails = new QTextBrowser;
    mDetails->setFrameShape( QFrame::NoFrame );
    mSplitter->addWidget( mDetails );

    QVBoxLayout* l = new QVBoxLayout;
    l->setMargin( 0 );
    l->addWidget( mSplitter );
    setLayout( l );
}
