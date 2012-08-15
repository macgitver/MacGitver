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
#include <QPushButton>
#include <QHBoxLayout>

#include "StringSelectorWidget.h"
#include "StringSelectorWidgetPrivate.h"

StringSelectorWidgetTree::StringSelectorWidgetTree( QWidget* parent )
	: QTreeWidget( parent )
{
	setHeaderHidden( true );
	setRootIsDecorated( false );
	setSelectionMode( QAbstractItemView::MultiSelection );
}

QSize StringSelectorWidgetTree::sizeHint() const
{
	return QSize( 120, 70 );
}

StringSelectorWidgetPrivate::StringSelectorWidgetPrivate( StringSelectorWidget* owner )
{
	mOwner = owner;
	mButtons = StringSelectorWidget::btnSelectOne |
			   StringSelectorWidget::btnUnselectOne;
	setupUi();
}

StringSelectorWidgetPrivate::~StringSelectorWidgetPrivate()
{
	clear();
}

void StringSelectorWidgetPrivate::setupUi()
{
	mSelected = new StringSelectorWidgetTree();
	QObject::connect( mSelected,
					  SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
					  mOwner,
					  SLOT(selItemDblClcked(QTreeWidgetItem*)) );

	QObject::connect( mSelected->selectionModel(),
					  SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
					  mOwner,
					  SLOT(selectionChanged()) );

	mUnselected = new StringSelectorWidgetTree();

	QObject::connect( mUnselected,
					  SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
					  mOwner,
					  SLOT(unselItemDblClcked(QTreeWidgetItem*)) );

	QObject::connect( mUnselected->selectionModel(),
					  SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
					  mOwner,
					  SLOT(selectionChanged()) );

	mButtonLayout = new QVBoxLayout;

	updateButtons();

	QHBoxLayout* l = new QHBoxLayout;
	l->addWidget( mSelected );
	l->addLayout( mButtonLayout );
	l->addWidget( mUnselected );
	l->setMargin( 0 );
	mOwner->setLayout( l );
}

void StringSelectorWidgetPrivate::updateButtons()
{
	foreach( QPushButton* btn, mButtonWidgets )
	{
		btn->deleteLater();
	}
	mButtonWidgets.clear();

	if( mButtons & StringSelectorWidget::btnSelectAll )
	{
		QPushButton* btn = new QPushButton( StringSelectorWidget::tr( "<<" ) );
		QObject::connect( btn, SIGNAL(clicked()),
						  mOwner, SLOT(selectAll()) );
		mButtonLayout->addWidget( btn );
		mButtonWidgets.insert( StringSelectorWidget::btnSelectAll, btn );
	}

	if( mButtons & StringSelectorWidget::btnSelectOne )
	{
		QPushButton* btn = new QPushButton( StringSelectorWidget::tr( "<" ) );
		mButtonLayout->addWidget( btn );
		QObject::connect( btn, SIGNAL(clicked()),
						  mOwner, SLOT(clickedSelect()) );
		mButtonWidgets.insert( StringSelectorWidget::btnSelectOne, btn );
	}

	if( mButtons & StringSelectorWidget::btnUnselectOne )
	{
		QPushButton* btn = new QPushButton( StringSelectorWidget::tr( ">" ) );
		mButtonLayout->addWidget( btn );
		QObject::connect( btn, SIGNAL(clicked()),
						  mOwner, SLOT(clickedUnselect()) );
		mButtonWidgets.insert( StringSelectorWidget::btnUnselectOne, btn );
	}

	if( mButtons & StringSelectorWidget::btnSelectNone )
	{
		QPushButton* btn = new QPushButton( StringSelectorWidget::tr( ">>" ) );
		QObject::connect( btn, SIGNAL(clicked()),
						  mOwner, SLOT(selectNone()) );
		mButtonLayout->addWidget( btn );
		mButtonWidgets.insert( StringSelectorWidget::btnSelectNone, btn );
	}

	updateButtonStates();
}

void StringSelectorWidgetPrivate::updateButtonStates()
{
	if( mButtonWidgets.contains( StringSelectorWidget::btnSelectAll ) )
	{
		const bool enabled = mUnselected->topLevelItemCount() > 0;
		mButtonWidgets.value( StringSelectorWidget::btnSelectAll )->setEnabled( enabled );
	}

	if( mButtonWidgets.contains( StringSelectorWidget::btnSelectNone ) )
	{
		const bool enabled = mSelected->topLevelItemCount() > 0;
		mButtonWidgets.value( StringSelectorWidget::btnSelectNone )->setEnabled( enabled );
	}

	if( mButtonWidgets.contains( StringSelectorWidget::btnSelectOne ) )
	{
		const bool enabled = mUnselected->selectionModel()->selectedRows().count();
		mButtonWidgets.value( StringSelectorWidget::btnSelectOne )->setEnabled( enabled );
	}

	if( mButtonWidgets.contains( StringSelectorWidget::btnUnselectOne ) )
	{
		const bool enabled = mSelected->selectionModel()->selectedRows().count();
		mButtonWidgets.value( StringSelectorWidget::btnUnselectOne )->setEnabled( enabled );
	}
}

void StringSelectorWidgetPrivate::clickedButton( QTreeWidget* widgetFrom, bool newSelection )
{
	Entries e;
	foreach( QTreeWidgetItem* it, widgetFrom->selectedItems() )
	{
		QVariant v = it->data( 0, Qt::UserRole );
		StringSelectorWidgetEntry* entry = entryFromValue( v );
		e.append( entry );
	}

	foreach( StringSelectorWidgetEntry* entry, e )
	{
		setEntrySelected( entry, newSelection );
	}
}

void StringSelectorWidgetPrivate::setEntrySelected( StringSelectorWidgetEntry* entry,
													bool selected )
{
	if( !entry || selected == entry->mIsSelected )
	{
		return;
	}

	if( selected )
	{
		int idx = mUnselected->indexOfTopLevelItem( entry->mItem );
		if( idx != -1 )
		{
			mUnselected->takeTopLevelItem( idx );
		}
		mSelected->addTopLevelItem( entry->mItem );
	}
	else
	{
		int idx = mSelected->indexOfTopLevelItem( entry->mItem );
		if( idx != -1 )
		{
			mSelected->takeTopLevelItem( idx );
		}
		mUnselected->addTopLevelItem( entry->mItem );
	}

	entry->mIsSelected = selected;

	updateButtonStates();
}

StringSelectorWidgetPrivate::Entries StringSelectorWidgetPrivate::all( bool selected ) const
{
	Entries e;

	for( int i = 0; i < mAllEntries.count(); i++ )
	{
		StringSelectorWidgetEntry* entry = mAllEntries.at( i );
		if( entry && entry->mIsSelected == selected )
		{
			e.append( entry );
		}
	}

	return e;
}

StringSelectorWidgetEntry* StringSelectorWidgetPrivate::entryFromValue(
		const QVariant& value ) const
{
	for( int i = 0; i < mAllEntries.count(); i++ )
	{
		if( mAllEntries.at( i )->mValue == value )
		{
			return mAllEntries.at( i );
		}
	}

	return NULL;
}

StringSelectorWidgetEntry* StringSelectorWidgetPrivate::entryFromDisplay(
		const QString& display ) const
{
	for( int i = 0; i < mAllEntries.count(); i++ )
	{
		if( mAllEntries.at( i )->mDisplay == display )
		{
			return mAllEntries.at( i );
		}
	}

	return NULL;
}

void StringSelectorWidgetPrivate::moveAll( bool select )
{
	Entries todo = all( !select );
	for( int i = 0; i < todo.count(); i++ )
	{
		setEntrySelected( todo[ i ], select );
	}
}

void StringSelectorWidgetPrivate::clear()
{
	while( mAllEntries.count() )
	{
		StringSelectorWidgetEntry* e = mAllEntries.takeFirst();
		if( e )
		{
			delete e->mItem;
			delete e;
		}
	}

	updateButtonStates();
}

StringSelectorWidget::StringSelectorWidget( QWidget* parent )
	: QWidget( parent )
{
	d = new StringSelectorWidgetPrivate( this );
}

StringSelectorWidget::~StringSelectorWidget()
{
	delete d;
}

void StringSelectorWidget::clear()
{
	d->clear();
}

void StringSelectorWidget::addEntry( const QString& display, bool isSelected,
									 const QVariant& value )
{
	StringSelectorWidgetEntry* e = new StringSelectorWidgetEntry;
	e->mDisplay		= display;
	e->mIsSelected	= isSelected;
	e->mValue		= value;

	QTreeWidget* wgt = isSelected ? d->mSelected : d->mUnselected;
	e->mPosition = wgt->topLevelItemCount();
	e->mItem = new QTreeWidgetItem( wgt, QStringList() << display );
	e->mItem->setData( 0, Qt::UserRole, value );

	d->mAllEntries.append( e );
	d->updateButtonStates();
}

int StringSelectorWidget::countEntries() const
{
	return d->mAllEntries.count();
}

void StringSelectorWidget::setValueSelected( const QVariant& value, bool selected )
{
	d->setEntrySelected( d->entryFromValue( value ), selected );
}

void StringSelectorWidget::setStringSelected( const QString& display, bool selected )
{
	d->setEntrySelected( d->entryFromDisplay( display ), selected );
}

void StringSelectorWidget::selectAll()
{
	d->moveAll( true );
}

void StringSelectorWidget::selectNone()
{
	d->moveAll( false );
}

void StringSelectorWidget::selItemDblClcked( QTreeWidgetItem* it )
{
	d->setEntrySelected( d->entryFromValue( it->data( 0, Qt::UserRole ) ), false );
}

void StringSelectorWidget::unselItemDblClcked( QTreeWidgetItem* it )
{
	d->setEntrySelected( d->entryFromValue( it->data( 0, Qt::UserRole ) ), true );
}

void StringSelectorWidget::setButtons( Buttons buttons )
{
	if( d->mButtons != buttons )
	{
		d->mButtons = buttons;
		d->updateButtons();
	}
}

StringSelectorWidget::Buttons StringSelectorWidget::buttons() const
{
	return d->mButtons;
}

void StringSelectorWidget::selectionChanged()
{
	d->updateButtonStates();
}

void StringSelectorWidget::clickedSelect()
{
	d->clickedButton( d->mUnselected, true );
}

void StringSelectorWidget::clickedUnselect()
{
	d->clickedButton( d->mSelected, false );
}
