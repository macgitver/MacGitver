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

#include "Heaven/Actions/ActionContainerPrivate.h"
#include "Heaven/Actions/MenuPrivate.h"
#include "Heaven/Actions/ActionPrivate.h"
#include "Heaven/Actions/MergePlacePrivate.h"
#include "Heaven/Actions/Separator.h"

#include "Heaven/Actions/MergesManager.h"

namespace Heaven
{

	ActionContainerPrivate::ActionContainerPrivate( ActionContainer* owner )
		: UiContainer( owner )
		, mPriority( 50 )
	{
	}

	ActionContainerPrivate::~ActionContainerPrivate()
	{
	}

	UiObjectTypes ActionContainerPrivate::type() const
	{
		return ContainerType;
	}

	int ActionContainerPrivate::priority() const
	{
		return mPriority;
	}

	ActionContainer::ActionContainer( QObject* parent )
		: QObject( parent )
	{
		d = new ActionContainerPrivate( this );
	}

	ActionContainer::~ActionContainer()
	{
		delete d;
	}

	UiObject* ActionContainer::uiObject()
	{
		return d;
	}

	void ActionContainer::setMergePriority( int priority )
	{
		d->mPriority = priority;
	}

	int ActionContainer::mergePriority() const
	{
		return d->mPriority;
	}

	void ActionContainer::mergeInto( const QByteArray& mergePlace )
	{
		MergesManager::self()->mergeContainer( d, mergePlace );
	}

	void ActionContainer::add( Action* uio )
	{
		d->add( uio->uiObject() );
	}

	void ActionContainer::add( ActionContainer* uio )
	{
		d->add( uio->uiObject() );
	}

	void ActionContainer::add( Menu* uio )
	{
		d->add( uio->uiObject() );
	}

	void ActionContainer::add( MergePlace* uio )
	{
		d->add( uio->uiObject() );
	}

	void ActionContainer::addSeparator()
	{
		d->add( new Separator( this ) );
	}

}
