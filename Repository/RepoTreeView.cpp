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

#include "libMacGitverCore/Widgets/TreeViewCtxMenu.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/MacGitver/RepoManager.hpp"
#include "libMacGitverCore/MacGitver/RepositoryInfo.hpp"

#include "RepoTreeView.hpp"
#include "RepoInfoModel.hpp"
#include "RepositoryContext.hpp"

RepoTreeView::RepoTreeView()
    : ContextView( "RepoTree" )
{
    setViewName( trUtf8( "Repositories" ) );
    setFlags( ProvidesContexts );
    setupActions( this );

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

    setWidget( mRepos );

    connect( mRepos, SIGNAL(contextMenu(QModelIndex,QPoint)),
             this, SLOT(contextMenu(QModelIndex,QPoint)) );

    connect( &MacGitver::repoMan(), SIGNAL(repositoryActivated(RepositoryInfo*)),
             this, SLOT(onRepoActivated(RepositoryInfo*)) );
    connect( &MacGitver::repoMan(), SIGNAL(repositoryDeactivated(RepositoryInfo*)),
             this, SLOT(onRepoDeactivated(RepositoryInfo*)) );
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

void RepoTreeView::onRepoActivated(RepositoryInfo* repo)
{
    Heaven::ContextKeys keys = mkKeys();
    keys.set("RepoName", repo->path());

    bool isNewContext = false;
    Heaven::ViewContext* ctx = contextFor(keys, &isNewContext);

    if (isNewContext) {
        RepositoryContext* ctx2 = qobject_cast< RepositoryContext* >(ctx);
        Q_ASSERT(ctx2);

        ctx2->setRepository(repo);
    }

    setCurrentContext(ctx);
}

void RepoTreeView::onRepoDeactivated(RepositoryInfo* repo)
{
    RepositoryContext* ctx = qobject_cast< RepositoryContext* >(currentContext());

    if (ctx && ctx->repository() == repo) {
        setCurrentContext( NULL );
    }
}

Heaven::ViewContext* RepoTreeView::createContextObject() const
{
    return new RepositoryContext;
}
