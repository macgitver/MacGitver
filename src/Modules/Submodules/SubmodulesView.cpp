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

#include "SubmodulesView.h"

SubmodulesView::SubmodulesView()
	: GlobalView( "Submodules" )
{
	setViewName( trUtf8( "Submodules" ) );

	setupActions( this );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );

	l->addWidget( tbSMViewToolbar->toolBarFor( this ) );

	mTree = new QTreeView;
	l->addWidget( mTree );

	setLayout( l );
}

