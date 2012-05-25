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
#include <QAction>

#include "Heaven/MenuPrivate.h"
#include "Heaven/ActionPrivate.h"
#include "Heaven/ActionContainerPrivate.h"
#include "Heaven/MergePlacePrivate.h"
#include "Heaven/Separator.h"

#include "Heaven/MergesManager.h"

namespace Heaven
{

	MenuPrivate::MenuPrivate( Menu* owner )
		: UiContainer( owner )
		, mEnabled( true )
		, mRebuildQueued( false )
	{
	}

	MenuPrivate::~MenuPrivate()
	{
	}

	QMenu* MenuPrivate::createQMenu( QWidget* forParent )
	{
		QMenu* menu = new QMenu( forParent );

		menu->setTitle( mText );
		menu->setEnabled( mEnabled );

		QAction* action = menu->menuAction();
		action->setToolTip( mToolTip );
		action->setStatusTip( mStatusTip );

		setContainerDirty();

		mQMenus.insert( menu );
		return menu;
	}

	QMenu* MenuPrivate::getOrCreateQMenu( QWidget* forParent )
	{
		foreach( QMenu* menu, mQMenus )
		{
			if( menu->parent() == forParent )
			{
				return menu;
			}
		}

		return createQMenu( forParent );
	}

	void MenuPrivate::setText( const QString& text )
	{
		mText = text;

		foreach( QMenu* menu, mQMenus )
		{
			menu->setTitle( text );
		}
	}

	void MenuPrivate::setStatusTip( const QString& text )
	{
		mStatusTip = text;

		foreach( QMenu* menu, mQMenus )
		{
			menu->menuAction()->setStatusTip( text );
		}
	}

	void MenuPrivate::setToolTip( const QString& text )
	{
		mToolTip = text;

		foreach( QMenu* menu, mQMenus )
		{
			menu->menuAction()->setToolTip( text );
		}
	}

	void MenuPrivate::setEnabled( bool v )
	{
		mEnabled = v;

		foreach( QMenu* menu, mQMenus )
		{
			menu->setEnabled( v );
		}
	}

	void MenuPrivate::menuAboutToShow()
	{
	}

	void MenuPrivate::menuDestroyed()
	{
		mQMenus.remove( qobject_cast< QMenu* >( sender() ) );
	}

	void MenuPrivate::reemergeGuiElement()
	{
		MenuPrivate* menuPriv;
		ActionPrivate* actionPriv;
		ActionContainerPrivate* containerPriv;
		MergePlacePrivate* mergePlacePriv;

		mRebuildQueued = false;

		foreach( QMenu* myMenu, mQMenus )
		{
			myMenu->clear();

			foreach( UiObject* uio, allObjects() )
			{
				QList< QAction* > actions;

				// TODO: Do this the other way round. ContainerType will be much easier that way.
				switch( uio->type() )
				{
				case MenuType:
					menuPriv = qobject_cast< MenuPrivate* >( uio );
					Q_ASSERT( menuPriv );
					actions << menuPriv->getOrCreateQMenu( myMenu )->menuAction();
					myMenu->addActions( actions );
					break;

				case ActionType:
					actionPriv = qobject_cast< ActionPrivate* >( uio );
					Q_ASSERT( actionPriv );
					actions << actionPriv->getOrCreateQAction( myMenu );
					myMenu->addActions( actions );
					break;

				case SeparatorType:
					myMenu->addSeparator();
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
					MergesManager::self()->emerge( mergePlacePriv->mName, myMenu );
					break;

				default:
					Q_ASSERT( false );
				}
			}
		}
	}

	void MenuPrivate::setContainerDirty( bool value )
	{
		UiContainer::setContainerDirty( value );
		if( value && !mRebuildQueued )
		{
			mRebuildQueued = true;
			QMetaObject::invokeMethod( this, "reemergeGuiElement", Qt::QueuedConnection );
		}
	}

	UiObjectTypes MenuPrivate::type() const
	{
		return MenuType;
	}

	Menu::Menu( QObject* parent )
		: QObject( parent )
	{
		d = new MenuPrivate( this );
	}

	Menu::~Menu()
	{
		delete d;
	}

	UiObject* Menu::uiObject()
	{
		return d;
	}

	QString Menu::text() const
	{
		return d->mText;
	}

	QString Menu::statusTip() const
	{
		return d->mStatusTip;
	}

	QString Menu::toolTip() const
	{
		return d->mToolTip;
	}

	bool Menu::isEnabled() const
	{
		return d->mEnabled;
	}

	void Menu::setText( const QString& text )
	{
		if( text != d->mText )
		{
			d->setText( text );
		}
	}

	void Menu::setStatusTip( const QString& text )
	{
		if( text != d->mStatusTip )
		{
			d->setStatusTip( text );
		}
	}

	void Menu::setToolTip( const QString& text )
	{
		if( text != d->mToolTip )
		{
			d->setToolTip( text );
		}
	}

	void Menu::setStatusToolTip( const QString& text )
	{
		setStatusTip( text );
		setToolTip( text );
	}

	void Menu::setEnabled( bool v )
	{
		d->setEnabled( v );
	}

	void Menu::setDisabled( bool v )
	{
		setEnabled( !v );
	}

	void Menu::add( Action* uio )
	{
		d->add( uio->uiObject() );
	}

	void Menu::add( ActionContainer* uio )
	{
		d->add( uio->uiObject() );
	}

	void Menu::add( Menu* uio )
	{
		d->add( uio->uiObject() );
	}

	void Menu::add( MergePlace* uio )
	{
		d->add( uio->uiObject() );
	}

	void Menu::addSeparator()
	{
		d->add( new Separator( this ) );
	}

}
