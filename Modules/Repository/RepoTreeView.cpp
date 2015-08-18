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
#include <QSortFilterProxyModel>

#include "libMacGitverCore/Widgets/TreeViewCtxMenu.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libRepoMan/RepoMan.hpp"

#include "RepoTreeView.hpp"
#include "RepoInfoModel.hpp"
#include "RepositoryContext.hpp"

RepoTreeView::RepoTreeView()
    : ContextView("RepoTreeView")
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

    QSortFilterProxyModel* sfpm = new QSortFilterProxyModel(this);
    sfpm->setSourceModel(mModel);
    sfpm->setSortRole(Qt::DisplayRole);
    sfpm->sort(0);
    mRepos->setModel(sfpm);

    mRepos->setIndentation( 12 );
    mRepos->expandAll();
    mRepos->setHeaderHidden( true );

    setWidget( mRepos );

    connect( mRepos,    SIGNAL(contextMenu(QModelIndex,QPoint)),
             this,      SLOT(contextMenu(QModelIndex,QPoint)) );

    connect(&RM::RepoMan::instance(),   &RM::RepoMan::repositoryActivated,
            this,                       &RepoTreeView::onRepoActivated);

    connect(&RM::RepoMan::instance(),   &RM::RepoMan::repositoryDeactivated,
            this,                       &RepoTreeView::onRepoDeactivated);
}

QModelIndex RepoTreeView::deeplyMapToSource( QModelIndex current ) const
{
    while( current.isValid() )
    {
        const QAbstractProxyModel* apm = qobject_cast< const QAbstractProxyModel* >( current.model() );
        if( !apm )
            return current;

        current = apm->mapToSource( current );
    }

    return QModelIndex();
}

void RepoTreeView::contextMenu( const QModelIndex& index, const QPoint& globalPos )
{
#if 0
    RM::Repo* info = mModel->index2Repo(deeplyMapToSource(index));

    if (info) {
        Heaven::Menu* menu = info->isSubModule() ? menuCtxMenuSMRepo : menuCtxMenuRepo;
        menu->setActivationContext( info );
        actActivate->setEnabled( !info->isActive() );

        menu->showPopup( globalPos );
    }
#endif
}

void RepoTreeView::onCtxActivate()
{
#if 0
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if (action) {
        RM::Repo* info = qobject_cast< RM::Repo* >( action->activatedBy() );
        if( info )
        {
            MacGitver::repoMan().activate( info );
        }
    }
#endif
}

void RepoTreeView::onCtxClose()
{
#if 0
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if( action )
    {
        RM::Repo* info = qobject_cast< RM::Repo* >( action->activatedBy() );
        if( info )
        {
            info->close();
        }
    }
#endif
}

void RepoTreeView::onRepoActivated(const RM::Frontend::Repo& repo)
{
#if 0
    BlueSky::ContextKeys keys = mkKeys();
    keys.set("RepoName", repo->path());

    bool isNewContext = false;
    BlueSky::ViewContext* ctx = contextFor(keys, &isNewContext);

    if (isNewContext) {
        RepositoryContext* ctx2 = qobject_cast< RepositoryContext* >(ctx);
        Q_ASSERT(ctx2);

        ctx2->setRepository(repo);
    }

    setCurrentContext(ctx);
#endif
}

void RepoTreeView::onRepoDeactivated(const RM::Frontend::Repo& repo)
{
#if 0
    RepositoryContext* ctx = qobject_cast< RepositoryContext* >(currentContext());

    if (ctx && ctx->repository() == repo) {
        setCurrentContext( NULL );
    }
#endif
}

BlueSky::ViewContext* RepoTreeView::createContextObject() const
{
    return new RepositoryContext;
}
