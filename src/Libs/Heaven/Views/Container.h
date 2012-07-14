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

typedef QTabWidget HeavenTabWidget;
typedef QSplitter HeavenSplitter;

namespace Heaven
{
	class View;
}

class HEAVEN_API HeavenViewContainer : public QObject
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
		bool					isView;
		union
		{
			Heaven::View*			view;
			HeavenViewContainer*	container;
		};
	};

public:
	HeavenViewContainer( Type t, Type s, HeavenViewContainer* parent = NULL );
	~HeavenViewContainer();


public:
	Type type() const;

	QList< Heaven::View* > views() const;
	int numViews() const;
	int addView( Heaven::View* view );
	Heaven::View* takeView( int index );
	QWidget* containerWidget();

	QList< HeavenViewContainer* > containers() const;
	int numContainers() const;
	int addContainer( HeavenViewContainer* container );
	void insertContainer( int pos, HeavenViewContainer* container );

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

#endif
