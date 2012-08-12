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

#include <QVBoxLayout>
#include <QToolBar>
#include <QTreeView>

#include "RemotesView.h"
#include "RemotesViewContext.h"

RemotesView::RemotesView()
	: GlobalView( QLatin1String( "Remotes" ) )
	, mCtx( NULL )
{
	setupActions( this );

	QVBoxLayout* l = new QVBoxLayout();
	l->setMargin( 0 );
	l->setSpacing( 0 );
	setLayout( l );

	mTreeView = new QTreeView();
	mTreeView->setFrameShape( QFrame::NoFrame );

	l->addWidget( tbRemotesTB->toolBarFor( this ) );

	l->addWidget( mTreeView );

	setViewName( trUtf8( "Remotes" ) );
}

Heaven::ViewContext* RemotesView::createContext()
{
	return new RemotesViewContext;
}

void RemotesView::setContext( Heaven::ViewContext* context )
{
	GlobalView::setContext( context );
	mCtx = qobject_cast< RemotesViewContext* >( context );
	Q_ASSERT( mCtx );

	mTreeView->setModel( mCtx->model() );
}


