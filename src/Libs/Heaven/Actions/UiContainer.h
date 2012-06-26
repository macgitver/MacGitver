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

#ifndef MGV_HEAVEN_UICONTAINER_H
#define MGV_HEAVEN_UICONTAINER_H

#include <QList>

class QMenu;
class QToolBar;
class QMenuBar;

#include "Heaven/Actions/UiObject.h"

namespace Heaven
{

	class UiContainer : public UiObject
	{
		Q_OBJECT
	protected:
		UiContainer( QObject* owner );
		~UiContainer();

	public:
		void add( UiObject* uio );
		void remove( UiObject* uio );

	public:
		bool isContainerDirty() const;
		virtual void setContainerDirty( bool value = true );
		virtual int priority() const;

		virtual bool mergeInto( QMenu* menu );
		virtual bool mergeInto( QMenuBar* menuBar );
		virtual bool mergeInto( QToolBar* toolBar );

	protected:
		int numObjects() const;
		UiObject* objectAt( int index );
		QList< UiObject* > allObjects() const;

	private:
		bool					mDirty;
		QList< UiObject* >		mContent;
	};

}

#endif
