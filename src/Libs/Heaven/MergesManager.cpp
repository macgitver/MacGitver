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

#include "Heaven/MergesManager.h"
#include "Heaven/MergePlace.h"

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

	void MergesManager::setContainerDirty( IContainer* container )
	{
	}

	void MergesManager::createMergePlace( MergePlace* place )
	{
		Q_ASSERT( place );

		MergePlaces* mps = new MergePlaces;
		mps->mName = place->name();
		mps->mPlace = place;

		mKnownPlaces.insert( place->name(), mps );
	}

	void MergesManager::removeMergePlace( MergePlace* place )
	{
		if( mKnownPlaces.count() == 0 )
		{
			sSelf = NULL;
			delete this;
		}
	}

	void mergeContainer( IContainer* container, int priority, const QByteArray& mergePlace )
	{
	}

	void mergeContainer( IContainer* container, int priority, MergePlace* place )
	{
	}

	void unmergeContainer( IContainer* container, const QByteArray& mergePlace )
	{
	}

	void unmergeContainer( IContainer* container, MergePlace* place )
	{
	}

	void unmergeContainer( IContainer* container )
	{
	}

}
