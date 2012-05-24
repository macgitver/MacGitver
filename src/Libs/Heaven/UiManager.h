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

#ifndef MGV_HEAVEN_ACTION_MANAGER_H
#define MGV_HEAVEN_ACTION_MANAGER_H

#include <QObject>
#include <QHash>
#include <QSet>

namespace Heaven
{

	class UiObject;

	class UiManager : public QObject
	{
		Q_OBJECT
	private:
		UiManager();
		~UiManager();

	public:
		static UiManager* self();

	public:
		void addUiObject( UiObject* uio );
		void delUiObject( UiObject* uio );

	private:
		static UiManager* sSelf;

		QHash< UiObject*, QSet< UiObject* > >	mUioUsage;
	};

}

#endif
