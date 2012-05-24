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

#include "Heaven/UiObject.h"
#include "Heaven/UiContainer.h"
#include "Heaven/UiManager.h"

namespace Heaven
{

	UiObject::UiObject( QObject* owner )
	{
		UiManager::self()->addUiObject( this );
	}

	UiObject::~UiObject()
	{
		foreach( UiContainer* container, mContainers )
		{
			removeFromContainer( container );
		}

		UiManager::self()->delUiObject( this );
	}

	void UiObject::addedToContainer( UiContainer* container )
	{
		mContainers.insert( container );
	}

	void UiObject::removeFromContainer( UiContainer* container )
	{
		container->remove( this );
	}

	void UiObject::removedFromContainer( UiContainer* container )
	{
		mContainers.remove( container );
	}
}
