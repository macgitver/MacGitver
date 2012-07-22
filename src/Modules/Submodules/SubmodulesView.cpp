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
#include <QStandardItemModel>
#include <QToolBar>
#include <QTreeView>

#include "GitWrap/ObjectId.h"

#include "MacGitver/MacGitver.h"

#include "SubmodulesView.h"
#include "SubmodulesCreateEditDlg.h"

SubmodulesView::SubmodulesView()
	: GlobalView( "Submodules" )
{
	setupActions( this );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );

	l->addWidget( tbSMViewToolbar->toolBarFor( this ) );

	mTree = new QTreeView;
	mTree->setRootIsDecorated( false );
	l->addWidget( mTree );

	mModel = new QStandardItemModel( this );
	mTree->setModel( mModel );

	setLayout( l );

	setViewName( trUtf8( "Submodules" ) );

	connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
			 this, SLOT(repositoryChanged(Git::Repository)) );

	Git::Repository repo = MacGitver::self().repository();
	if( repo.isValid() )
	{
		repositoryChanged( repo );
	}
}

void SubmodulesView::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;
	readSubmodules();
}

void SubmodulesView::readSubmodules()
{
	QList< Git::Submodule > submodules;

	if( mRepo.isValid() )
	{
		submodules = mRepo.submodules();
	}

	QStringList toVisit = mNameToItem.keys();

	foreach( Git::Submodule module, submodules )
	{
		QString name = module.name();
		QStandardItem* it = mNameToItem.value( name, NULL );

		if( it )
		{
			int i = toVisit.indexOf( name );
			if( i != -1 )	// It should really be in, shouldn't it? :)
			{
				toVisit.removeAt( i );
			}
		}
		else
		{
			it = new QStandardItem( name );
			mModel->appendRow( it );
		}

		it->setData( module.path(), Qt::UserRole + 1 );
		it->setData( module.url(), Qt::UserRole + 2 );
		it->setData( module.fetchRecursive(), Qt::UserRole + 3 );
		it->setData( module.updateStrategy(), Qt::UserRole + 4 );
		it->setData( module.ignoreStrategy(), Qt::UserRole + 5 );
		it->setData( module.currentSHA1().toString(), Qt::UserRole + 6 );
	}

	foreach( QString module, toVisit )
	{
		QStandardItem* it = mNameToItem.take( module );
		delete it;
	}
}

void SubmodulesView::addSubmodule()
{
	SubmodulesCreateEditDlg d;
	d.exec();
}
