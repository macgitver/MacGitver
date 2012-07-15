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

#ifndef HEAVEN_CONTAINER_H
#define HEAVEN_CONTAINER_H

#include <QObject>
#include <QTableWidget>
#include <QSplitter>

#include "Heaven/HeavenApi.h"

namespace Heaven
{
	class View;

	typedef QTabWidget	HeavenTabWidget;
	typedef QSplitter	HeavenSplitter;

	class HEAVEN_API ViewContainer : public QObject
	{
		Q_OBJECT
	public:
		enum Type
		{
			Tab			= 1 << 8,
			Splitter	= 1 << 9,
			BaseMask	= Tab|Splitter,

			SubTabTop		= 0,
			SubTabBottom	= 1,
			SubTabRight		= 2,
			SubTabLeft		= 3,

			SubSplitVert	= 0,
			SubSplitHorz	= 1,

			SubMask			= 255
		};

		struct Content
		{
			bool				isView;
			union
			{
				View*			view;
				ViewContainer*	container;
			};
		};

	public:
		ViewContainer( Type t, Type s, ViewContainer* parent = NULL );
		~ViewContainer();

	public:
		Type type() const;

		QList< View* > views() const;
		int numViews() const;
		int addView( View* view );
		View* takeView( int index );
		QWidget* containerWidget();

		QList< ViewContainer* > containers() const;
		int numContainers() const;
		int addContainer( ViewContainer* container );
		void insertContainer( int pos, ViewContainer* container );

	private:
		Type				mType;
		QList< Content >	mContent;

		union
		{
			QWidget*			mContainerWidget;
			HeavenSplitter*		mSpliterWidget;
			HeavenTabWidget*	mTabWidget;
		};
	};

}

#endif
