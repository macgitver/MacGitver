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

#include "Heaven/ToolBarPrivate.h"
#include "Heaven/Menu.h"
#include "Heaven/Action.h"
#include "Heaven/ActionContainer.h"
#include "Heaven/MergePlace.h"

namespace Heaven
{

	ToolBarPrivate::ToolBarPrivate( ToolBar* owner )
		: UiContainer( owner )
	{
	}

	ToolBarPrivate::~ToolBarPrivate()
	{
	}

	UiObjectTypes ToolBarPrivate::type() const
	{
		return ToolBarType;
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

}
