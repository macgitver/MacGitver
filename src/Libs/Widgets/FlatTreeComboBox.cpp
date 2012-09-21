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

#include <QListView>

#include "FlatTreeModelPrivate.h"
#include "FlatTreeDelegate.h"
#include "FlatTreeComboBox.h"

FlatTreeComboBox::FlatTreeComboBox( QWidget* parent )
	: QComboBox( parent )
{
	setModel( new FlatTreeModel( this ) );
	QListView* lv = new QListView;
	lv->setModel( model() );
	lv->setItemDelegate( new FlatTreeDelegate( this ) );
	setView( lv );
}

void FlatTreeComboBox::setModel( FlatTreeModel* model )
{
	QComboBox::setModel( model );
}

void FlatTreeComboBox::clear()
{
	FlatTreeModel* m = qobject_cast< FlatTreeModel* >( model() );
	Q_ASSERT( m );
	m->clear();
}

void FlatTreeComboBox::add( const QString& entry )
{
	FlatTreeModel* m = qobject_cast< FlatTreeModel* >( model() );
	Q_ASSERT( m );
	m->remove( entry );
}

void FlatTreeComboBox::add( const QStringList& entries )
{
	FlatTreeModel* m = qobject_cast< FlatTreeModel* >( model() );
	Q_ASSERT( m );
	m->add( entries );
}

void FlatTreeComboBox::remove( const QString& entry )
{
	FlatTreeModel* m = qobject_cast< FlatTreeModel* >( model() );
	Q_ASSERT( m );
	m->remove( entry );
}

void FlatTreeComboBox::remove( const QStringList& entries )
{
	FlatTreeModel* m = qobject_cast< FlatTreeModel* >( model() );
	Q_ASSERT( m );
	m->remove( entries );
}
