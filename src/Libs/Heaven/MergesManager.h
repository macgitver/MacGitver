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

class QMenu;
class QMenuBar;
class QToolBar;

namespace Heaven
{

	class UiContainer;
	class MergePlace;
	class MergePlacePrivate;

	class MergesManager : public QObject
	{
	private:
		MergesManager();
		~MergesManager();

	public:
		static MergesManager* self();

	public:
		void setContainerDirty( UiContainer* container );

		void createMergePlace( MergePlace* place );
		void removeMergePlace( MergePlace* place );

		bool mergeContainer( UiContainer* container, const QByteArray& mergePlace );
		void mergeContainer( UiContainer* container, MergePlace* place );

		void unmergeContainer( UiContainer* container, const QByteArray& mergePlace );
		void unmergeContainer( UiContainer* container, MergePlace* place );
		void unmergeContainer( UiContainer* container );

		bool emerge( const QByteArray& mergeName, QMenu* menu );
		bool emerge( const QByteArray& mergeName, QMenuBar* menuBar );
		bool emerge( const QByteArray& mergeName, QToolBar* toolBar );

	private:
		static MergesManager* sSelf;

		struct ContainerMerge
		{
			ContainerMerge()
				: mContainer( NULL )
				, mPriority( 0 )
			{
			}

			ContainerMerge( UiContainer* container, int prio )
				: mContainer( container )
				, mPriority( prio )
			{
			}

			UiContainer*	mContainer;
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
