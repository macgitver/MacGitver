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

#include <QToolBar>
#include <QMenu>

#include "Heaven/ToolBarPrivate.h"
#include "Heaven/MenuPrivate.h"
#include "Heaven/ActionPrivate.h"
#include "Heaven/ActionContainerPrivate.h"
#include "Heaven/MergePlacePrivate.h"
#include "Heaven/Separator.h"

namespace Heaven
{

	ToolBarPrivate::ToolBarPrivate( ToolBar* owner )
		: UiContainer( owner )
		, mRebuildQueued( false )
	{
	}

	ToolBarPrivate::~ToolBarPrivate()
	{
	}

	QToolBar* ToolBarPrivate::createQToolBar( QWidget* forParent )
	{
		QToolBar* bar = new QToolBar( forParent );

		connect( bar, SIGNAL(destroyed()), this, SLOT(qtoolbarDestroyed()) );

		setContainerDirty();

		mToolBars.insert( bar );
		return bar;
	}

	QToolBar* ToolBarPrivate::getOrCreateQToolBar( QWidget* forParent )
	{
		foreach( QToolBar* bar, mToolBars )
		{
			if( bar->parent() == forParent )
			{
				return bar;
			}
		}

		return createQToolBar( forParent );
	}

	void ToolBarPrivate::qtoolbarDestroyed()
	{
		mToolBars.remove( qobject_cast< QToolBar* >( sender() ) );
	}

	void ToolBarPrivate::setContainerDirty( bool value )
	{
		UiContainer::setContainerDirty( value );
		if( value && !mRebuildQueued )
		{
			mRebuildQueued = true;
			QMetaObject::invokeMethod( this, "reemergeGuiElement", Qt::QueuedConnection );
		}
	}

	UiObjectTypes ToolBarPrivate::type() const
	{
		return ToolBarType;
	}

	void ToolBarPrivate::reemergeGuiElement()
	{
		MenuPrivate* menuPriv;
		ActionPrivate* actionPriv;
		ActionContainerPrivate* containerPriv;
		MergePlacePrivate* mergePlacePriv;
		mRebuildQueued = false;

		foreach( QToolBar* myBar, mToolBars )
		{
			myBar->clear();

			foreach( UiObject* uio, allObjects() )
			{
				QList< QAction* > actions;

				// TODO: Do this the other way round. ContainerType will be much easier that way.
				switch( uio->type() )
				{
				case MenuType:
					menuPriv = qobject_cast< MenuPrivate* >( uio );
					Q_ASSERT( menuPriv );
					actions << menuPriv->getOrCreateQMenu( myBar )->menuAction();
					myBar->addActions( actions );
					break;

				case ActionType:
					actionPriv = qobject_cast< ActionPrivate* >( uio );
					Q_ASSERT( actionPriv );
					actions << actionPriv->getOrCreateQAction( myBar );
					myBar->addActions( actions );
					break;

				case SeparatorType:
					myBar->addSeparator();
					break;

				case ContainerType:
					containerPriv = qobject_cast< ActionContainerPrivate* >( uio );
					Q_ASSERT( containerPriv );
					if( containerPriv->isContainerDirty() )
					{
						// TODO: Force reemerge
					}
					break;

				case MergePlaceType:
					mergePlacePriv = qobject_cast< MergePlacePrivate* >( uio );
					Q_ASSERT( mergePlacePriv );
					break;

				default:
					Q_ASSERT( false );
				}
			}
		}
	}

	ToolBar::ToolBar( QObject* parent )
		: QObject( parent )
	{
		d = new ToolBarPrivate( this );
	}

	ToolBar::~ToolBar()
	{
		delete d;
	}

	QToolBar* ToolBar::toolBarFor( QWidget* forParent )
	{
		return d->getOrCreateQToolBar( forParent );
	}

	void ToolBar::add( Action* uio )
	{
		d->add( uio->uiObject() );
	}

	void ToolBar::add( ActionContainer* uio )
	{
		d->add( uio->uiObject() );
	}

	void ToolBar::add( Menu* uio )
	{
		d->add( uio->uiObject() );
	}

	void ToolBar::add( MergePlace* uio )
	{
		d->add( uio->uiObject() );
	}

	void ToolBar::addSeparator()
	{
		d->add( new Separator );
	}

}
