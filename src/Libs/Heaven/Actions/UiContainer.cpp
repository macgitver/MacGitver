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

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include "Heaven/Actions/UiContainer.h"
#include "Heaven/Actions/UiManager.h"
#include "Heaven/Actions/ActionContainerPrivate.h"
#include "Heaven/Actions/MenuPrivate.h"
#include "Heaven/Actions/ActionPrivate.h"
#include "Heaven/Actions/WidgetActionPrivate.h"
#include "Heaven/Actions/MergePlacePrivate.h"
#include "Heaven/Actions/Separator.h"
#include "Heaven/Actions/MergesManager.h"
#include "Heaven/Actions/WidgetActionWrapper.h"

namespace Heaven
{

	UiContainer::UiContainer( QObject* owner )
		: UiObject( owner )
		, mDirty( false )
	{
	}

	UiContainer::~UiContainer()
	{
	}

	void UiContainer::add( UiObject* uio )
	{
		mContent.append( uio );
		setContainerDirty();
	}

	void UiContainer::remove( UiObject* uio )
	{
		for( int i = 0; i < mContent.count(); i++ )
		{
			if( mContent[ i ] == uio )
			{
				mContent.removeAt( i );
				i--;
				setContainerDirty();
			}
		}
	}

	int UiContainer::numObjects() const
	{
		return mContent.count();
	}

	UiObject* UiContainer::objectAt( int index )
	{
		return mContent[ index ];
	}

	QList< UiObject* > UiContainer::allObjects() const
	{
		return mContent;
	}

	bool UiContainer::isContainerDirty() const
	{
		return mDirty;
	}

	void UiContainer::setContainerDirty( bool value )
	{
		mDirty = value;
	}

	int UiContainer::priority() const
	{
		return -1;
	}

	bool UiContainer::mergeInto( QMenu* menu )
	{
		MenuPrivate* menuPriv;
		ActionPrivate* actionPriv;
		WidgetActionPrivate* widgetActPriv;
//		ActionContainerPrivate* containerPriv;
//		MergePlacePrivate* mergePlacePriv;

		foreach( UiObject* uio, allObjects() )
		{
			QAction* action;

			switch( uio->type() )
			{
			case MenuType:
				menuPriv = qobject_cast< MenuPrivate* >( uio );
				Q_ASSERT( menuPriv );
				action = menuPriv->getOrCreateQMenu( menu )->menuAction();
				menu->addAction( action );
				break;

			case ActionType:
				actionPriv = qobject_cast< ActionPrivate* >( uio );
				Q_ASSERT( actionPriv );
				action = actionPriv->getOrCreateQAction( menu );
				menu->addAction( action );
				break;

			case SeparatorType:
				menu->addSeparator();
				break;

			case WidgetActionType:
				widgetActPriv = qobject_cast< WidgetActionPrivate* >( uio );
				Q_ASSERT( widgetActPriv );
				// We don't have to create several widget actions
				menu->addAction( widgetActPriv->wrapper() );
				break;

			case ContainerType:
				((UiContainer*)uio)->mergeInto( menu );
				break;

			case MenuBarType:
			case ToolBarType:
				// cannot merge Bars into menus
				Q_ASSERT( false );
				break;
			}
		}
		return true;
	}


	bool UiContainer::mergeInto( QMenuBar* menuBar )
	{
		MenuPrivate* menuPriv;
		ActionPrivate* actionPriv;
		WidgetActionPrivate* widgetActPriv;
//		ActionContainerPrivate* containerPriv;
//		MergePlacePrivate* mergePlacePriv;

		foreach( UiObject* uio, allObjects() )
		{
			QAction* action;

			switch( uio->type() )
			{
			case MenuType:
				menuPriv = qobject_cast< MenuPrivate* >( uio );
				Q_ASSERT( menuPriv );
				action = menuPriv->getOrCreateQMenu( menuBar )->menuAction();
				menuBar->addAction( action );
				break;

			case ActionType:
				actionPriv = qobject_cast< ActionPrivate* >( uio );
				Q_ASSERT( actionPriv );
				action = actionPriv->getOrCreateQAction( menuBar );
				menuBar->addAction( action );
				break;

			case SeparatorType:
				menuBar->addSeparator();
				break;

			case WidgetActionType:
				widgetActPriv = qobject_cast< WidgetActionPrivate* >( uio );
				Q_ASSERT( widgetActPriv );
				// We don't have to create several widget actions
				menuBar->addAction( widgetActPriv->wrapper() );
				break;

			case ContainerType:
				((UiContainer*)uio)->mergeInto( menuBar );
				break;

			case MenuBarType:
			case ToolBarType:
				// cannot merge Bars into menus
				Q_ASSERT( false );
				break;
			}

		}
		return true;
	}

	bool UiContainer::mergeInto( QToolBar* toolBar )
	{
		MenuPrivate* menuPriv;
		ActionPrivate* actionPriv;
		WidgetActionPrivate* widgetActPriv;
//		ActionContainerPrivate* containerPriv;
//		MergePlacePrivate* mergePlacePriv;

		foreach( UiObject* uio, allObjects() )
		{
			QAction* action;

			switch( uio->type() )
			{
			case MenuType:
				menuPriv = qobject_cast< MenuPrivate* >( uio );
				Q_ASSERT( menuPriv );
				action = menuPriv->getOrCreateQMenu( toolBar )->menuAction();
				toolBar->addAction( action );
				break;

			case ActionType:
				actionPriv = qobject_cast< ActionPrivate* >( uio );
				Q_ASSERT( actionPriv );
				action = actionPriv->getOrCreateQAction( toolBar );
				toolBar->addAction( action );
				break;

			case WidgetActionType:
				widgetActPriv = qobject_cast< WidgetActionPrivate* >( uio );
				Q_ASSERT( widgetActPriv );
				// We don't have to create several widget actions
				toolBar->addAction( widgetActPriv->wrapper() );
				break;

			case SeparatorType:
				toolBar->addSeparator();
				break;

			case ContainerType:
				((UiContainer*)uio)->mergeInto( toolBar );
				break;

			case MenuBarType:
			case ToolBarType:
				// cannot merge bars into bars
				Q_ASSERT( false );
				break;
			}

		}
		return true;
	}

}
