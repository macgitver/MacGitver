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

#include "Heaven/MergesManager.h"
#include "Heaven/MergePlace.h"

#include "Heaven/UiContainer.h"

namespace Heaven
{

	MergesManager::MergesManager()
	{
	}

	MergesManager::~MergesManager()
	{
	}

	MergesManager* MergesManager::sSelf = NULL;

	MergesManager* MergesManager::self()
	{
		if( !sSelf )
		{
			sSelf = new MergesManager;
		}

		return sSelf;
	}

	void MergesManager::setContainerDirty( UiContainer* container )
	{
	}

	void MergesManager::createMergePlace( MergePlace* place )
	{
		Q_ASSERT( place );

		MergePlaces* mps = mKnownPlaces.value( place->name(), NULL );

		if( !mps )
		{
			mps = new MergePlaces;
			mps->mName = place->name();

			mKnownPlaces.insert( place->name(), mps );
		}

		mps->mPlace = place;
	}

	void MergesManager::removeMergePlace( MergePlace* place )
	{
		if( mKnownPlaces.count() == 0 )
		{
			sSelf = NULL;
			delete this;
		}
	}

	bool MergesManager::mergeContainer( UiContainer* container, const QByteArray& mergePlace )
	{
		MergePlaces* place = mKnownPlaces.value( mergePlace, NULL );
		if( !place )
		{
			place = new MergePlaces;
			place->mName = mergePlace;
			place->mPlace = NULL;

			mKnownPlaces.insert( mergePlace, place );
		}

		int i = 0, prio = container->priority();
		while( i < place->mContainers.count() && prio > place->mContainers[ i ].mPriority )
			i++;

		/*
		if( prio > place->mContainers[ i ].mPriority )
			i++;
		*/

		Q_ASSERT( i <= place->mContainers.count() );

		place->mContainers.insert( i, ContainerMerge( container, prio ) );

		return true;
	}

	void MergesManager::mergeContainer( UiContainer* container, MergePlace* place )
	{
	}

	void MergesManager::unmergeContainer( UiContainer* container, const QByteArray& mergePlace )
	{
	}

	void MergesManager::unmergeContainer( UiContainer* container, MergePlace* place )
	{
	}

	void MergesManager::unmergeContainer( UiContainer* container )
	{
	}

	bool MergesManager::emerge( const QByteArray& mergeName, QMenu* menu )
	{
		MergePlaces* place = mKnownPlaces.value( mergeName, NULL );
		if( !place )
		{
			return false;
		}

		for( int i = 0; i < place->mContainers.count(); i++ )
		{
			UiContainer* container = place->mContainers.at( i ).mContainer;
			container->mergeInto( menu );
		}

		return true;
	}

	bool MergesManager::emerge( const QByteArray& mergeName, QMenuBar* menuBar )
	{
		MergePlaces* place = mKnownPlaces.value( mergeName, NULL );
		if( !place )
		{
			return false;
		}

		for( int i = 0; i < place->mContainers.count(); i++ )
		{
			UiContainer* container = place->mContainers.at( i ).mContainer;
			container->mergeInto( menuBar );
		}

		return true;
	}

	bool MergesManager::emerge( const QByteArray& mergeName, QToolBar* toolBar )
	{
		MergePlaces* place = mKnownPlaces.value( mergeName, NULL );
		if( !place )
		{
			return false;
		}

		for( int i = 0; i < place->mContainers.count(); i++ )
		{
			UiContainer* container = place->mContainers.at( i ).mContainer;
			container->mergeInto( toolBar );
		}

		return true;
	}
}
