/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#include "Heaven/UiContainer.h"
#include "Heaven/UiManager.h"

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

}
