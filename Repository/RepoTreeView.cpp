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

#include <QDebug>
#include <QVBoxLayout>

#include "libMacGitverCore/Widgets/TreeViewCtxMenu.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/MacGitver/RepoManager.hpp"
#include "libMacGitverCore/MacGitver/RepositoryInfo.hpp"

#include "RepoTreeView.hpp"
#include "RepoInfoModel.hpp"

RepoTreeView::RepoTreeView()
    : GlobalView( QLatin1String( "RepoTree" ) )
{
    setViewName( trUtf8( "Repositories" ) );

    mRepos = new TreeViewCtxMenu;
    mRepos->setFrameShape( QFrame::NoFrame );
    #ifdef Q_OS_MACX
    mRepos->setAttribute( Qt::WA_MacShowFocusRect, false );
    #endif
    mModel = new RepoInfoModel();
    mRepos->setModel( mModel );
    mRepos->setIndentation( 12 );
    mRepos->expandAll();
    mRepos->setHeaderHidden( true );

    connect( mRepos, SIGNAL(contextMenu(QModelIndex,QPoint)),
             this, SLOT(contextMenu(QModelIndex,QPoint)) );

    QVBoxLayout* l = new QVBoxLayout;
    l->setMargin( 0 );
    l->addWidget( mRepos );
    setLayout( l );

    setupActions( this );
}

void RepoTreeView::contextMenu( const QModelIndex& index, const QPoint& globalPos )
{
    RepositoryInfo* info = mModel->index2Info( index );

    if( info && !info->isDisabled() )
    {
        Heaven::Menu* menu = info->isSubModule() ? menuCtxMenuSMRepo : menuCtxMenuRepo;
        menu->setActivationContext( info );
        actActivate->setEnabled( !info->isActive() );

        menu->showPopup( globalPos );
    }
}

void RepoTreeView::onCtxActivate()
{
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if( action )
    {
        RepositoryInfo* info = qobject_cast< RepositoryInfo* >( action->activatedBy() );
        if( info )
        {
            MacGitver::repoMan().activate( info );
        }
    }
}

void RepoTreeView::onCtxClose()
{

}
