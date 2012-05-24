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

#ifndef MGV_HEAVEN_MERGES_MANAGER_H
#define MGV_HEAVEN_MERGES_MANAGER_H

#include <QObject>
#include <QHash>

namespace Heaven
{

	class IContainer;
	class MergePlace;

	class MergesManager : public QObject
	{
	private:
		MergesManager();
		~MergesManager();

	public:
		static MergesManager* self();

	public:
		void setContainerDirty( IContainer* container );

		void createMergePlace( MergePlace* place );
		void removeMergePlace( MergePlace* place );

		void mergeContainer( IContainer* container, int priority, const QByteArray& mergePlace );
		void mergeContainer( IContainer* container, int priority, MergePlace* place );

		void unmergeContainer( IContainer* container, const QByteArray& mergePlace );
		void unmergeContainer( IContainer* container, MergePlace* place );
		void unmergeContainer( IContainer* container );

	private:
		static MergesManager* sSelf;

		struct ContainerMerge
		{
			IContainer*		mContainer;
			int				mPriority;
		};

		struct MergePlaces
		{
			QByteArray				mName;
			MergePlace*				mPlace;
			QList< ContainerMerge >	mContainers;
		};

		QHash< QByteArray, MergePlaces* > mKnownPlaces;
	};

}

#endif
