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

#ifndef MGV_HEAVEN_MENUBAR_H
#define MGV_HEAVEN_MENUBAR_H

class QMenuBar;

#include <QString>
#include <QObject>

#include "Heaven/HeavenApi.h"

namespace Heaven
{
	class Action;
	class Menu;
	class MergePlace;
	class ActionContainer;

	class MenuBarPrivate;

	class HEAVEN_API MenuBar : public QObject
	{
		Q_OBJECT
	public:
		MenuBar( QObject* parent );
		~MenuBar();

	public:
		QMenuBar* menuBarFor( QWidget* forParent );

	public:
		void add( Menu* );
		void add( Action* );
		void add( MergePlace* );
		void add( ActionContainer* );

	private:
		MenuBarPrivate* d;
	};

}

#endif
