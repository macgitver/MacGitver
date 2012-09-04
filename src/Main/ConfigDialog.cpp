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

#include <QPushButton>

#include "Interfaces/IConfigPage.h"

#include "ConfigDialog.h"

ConfigDialog::ConfigDialog()
{
	setupUi( this );

	connect( widgetTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
			 this, SLOT(onWidgetChange(QTreeWidgetItem*)) );
}

ConfigDialog::~ConfigDialog()
{
}

void ConfigDialog::addPage( IConfigPage* page )
{
	widgets->addWidget( page->widget() );

	QTreeWidgetItem* groupItem = mGroupsById.value( page->groupId(), NULL );
	if( !groupItem )
	{
		groupItem = new QTreeWidgetItem( widgetTree, QStringList( page->groupName() ) );
		mGroupsById.insert( page->groupId(), groupItem );
		QFont f = font();
		f.setBold( true );
		groupItem->setFont( 0, f );
		groupItem->setExpanded( true );
		groupItem->setFlags( Qt::ItemIsEnabled );
	}

	Q_ASSERT( groupItem );
	QTreeWidgetItem* pageItem = new QTreeWidgetItem( groupItem, QStringList( page->pageName() ) );
	QByteArray fullPageId = page->groupId() + "/" + page->pageId();
	mPagesById.insert( fullPageId, page );
	mPageIdsByTree.insert( pageItem, fullPageId );
}

void ConfigDialog::setModified( IConfigPage* page, bool value )
{
	Q_ASSERT( page );
	buttonBox->button( QDialogButtonBox::Apply )->setEnabled( value );
	buttonBox->button( QDialogButtonBox::Ok )->setEnabled( value );
}

int ConfigDialog::exec()
{
	buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
	buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );

	return QDialog::exec();
}

void ConfigDialog::onWidgetChange( QTreeWidgetItem* newCurrent )
{
	if( !newCurrent )
	{
		return;
	}

	QByteArray pageId = mPageIdsByTree.value( newCurrent, QByteArray() );
	if( pageId.isEmpty() )
	{
		return;
	}

	IConfigPage* page = mPagesById.value( pageId, NULL );
	if( !page )
	{
		return;
	}

	widgets->setCurrentWidget( page->widget() );
}
