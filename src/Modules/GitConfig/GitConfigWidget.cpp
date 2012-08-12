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

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QFileSystemWatcher>

#include "GitConfigWidget.h"

GitConfigWidget::GitConfigWidget()
	: mEditable( false )
{
	setupUi( this );

	mTree->setColumnCount( 2 );

	mWatcher = new QFileSystemWatcher( this );
	connect( mWatcher, SIGNAL(fileChanged(QString)), this, SLOT(configChanged()) );
}

void GitConfigWidget::setConfig( const QString& fileName )
{
	QStringList l( fileName );
	setConfigs( l );
}

void GitConfigWidget::setConfigs( const QStringList& fileNames )
{
	mConfigFiles = fileNames;

	QString flat;

	Git::Config cfg = Git::Config::create();
	for( int i = 0; i < mConfigFiles.count(); i++ )
	{
		if( mConfigFiles[ i ].isEmpty() )
		{
			mConfigFiles.removeAt( i );
			--i;
			continue;
		}
		cfg.addFile( mConfigFiles[ i ], i + 1 );

		if( !flat.isEmpty() )
			flat += L'\n';

		flat += mConfigFiles[ i ];
	}

	if( flat.isEmpty() )
	{
		flat = trUtf8( "<b>This configuration was not found.</b>" );
	}
	txtReadFrom->setText( flat );

	mWatcher->addPaths( mConfigFiles );
	setConfig( cfg );
}

void GitConfigWidget::setConfig( const Git::Config& config )
{
	mConfig = config;

	syncConfig();
}

void GitConfigWidget::configChanged()
{
	mWatcher->removePaths( mConfigFiles );
	setConfigs( mConfigFiles );
}

void GitConfigWidget::allItems( QSet< QTreeWidgetItem* >& result, QTreeWidgetItem* parent )
{
	for( int i = 0; i < parent->childCount(); i++ )
	{
		QTreeWidgetItem* child = parent->child( i );
		result.insert( child );
		allItems( result, child );
	}
}

void GitConfigWidget::syncConfig()
{
	QFont fontBold( font() );
	fontBold.setBold( true );

	QSet< QTreeWidgetItem* > current;
	allItems( current, mTree->invisibleRootItem() );

	Git::ConfigValues values = mConfig.values();

	foreach( QString key, values.keys() )
	{
		QStringList path = key.split( L'.' );
		EntryData* parent = NULL;
		QString sectPart;
		for( int i = 0; i < path.count() - 1; i++ )
		{
			if( !sectPart.isEmpty() )
				sectPart += L'.';

			sectPart += path[ i ];
			if( mSectionEntries.contains( sectPart ) )
			{
				parent = mSectionEntries.value( sectPart );

				Q_ASSERT( parent->type == KeySection );

				if( current.contains( parent->treeItem ) )
				{
					current.remove( parent->treeItem );
				}
			}
			else
			{
				QTreeWidgetItem* parItem = parent ? parent->treeItem : mTree->invisibleRootItem();
				QTreeWidgetItem* myItem = new QTreeWidgetItem( parItem );
				parItem->setExpanded( true );
				myItem->setFont( 0, fontBold );
				myItem->setText( 0, path[ i ] );

				parent = new EntryData;
				parent->fullKey = sectPart;
				parent->treeItem = myItem;
				parent->type = KeySection;

				mSectionEntries.insert( parent->fullKey, parent );
				mEntries.insert( myItem, parent );
			}
		}

		if( mValueEntries.contains( key ) )
		{
			EntryData* data = mValueEntries.value( key, NULL );
			if( data->type == KeyValue )
			{
				if( current.contains( data->treeItem ) )
				{
					// It's the first time (in this run) that we visit this value entry. By now we
					// assume it _HAS CHANGED_ rather than assume that being propagated to a MultiVar

					data->value = values[ key ];
					data->treeItem->setText( 1, data->value );

					current.remove( data->treeItem );
				}
			}
		}
		else
		{
			QTreeWidgetItem* parItem = parent ? parent->treeItem : mTree->invisibleRootItem();
			QTreeWidgetItem* myItem = new QTreeWidgetItem( parItem );
			parItem->setExpanded( true );
			myItem->setText( 0, path.last() );
			myItem->setText( 1, values[ key ] );

			parent = new EntryData;
			parent->fullKey = key;
			parent->treeItem = myItem;
			parent->type = KeyValue;

			mValueEntries.insert( parent->fullKey, parent );
			mEntries.insert( myItem, parent );
		}
	}

	mTree->resizeColumnToContents( 0 );
	mTree->sortItems( 0, Qt::AscendingOrder );

	foreach( QTreeWidgetItem* it, current )
	{
		it->parent()->removeChild( it );
	}
	qDeleteAll( current );
}
