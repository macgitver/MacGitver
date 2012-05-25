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

#include "Heaven/ActionContainerPrivate.h"
#include "Heaven/MenuPrivate.h"
#include "Heaven/ActionPrivate.h"
#include "Heaven/MergePlacePrivate.h"
#include "Heaven/Separator.h"

namespace Heaven
{

	ActionContainerPrivate::ActionContainerPrivate( ActionContainer* owner )
		: UiContainer( owner )
	{
	}

	ActionContainerPrivate::~ActionContainerPrivate()
	{
	}

	UiObjectTypes ActionContainerPrivate::type() const
	{
		return ContainerType;
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
