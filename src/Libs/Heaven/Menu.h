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

#ifndef MGV_HEAVEN_MENU_H
#define MGV_HEAVEN_MENU_H

#include <QString>
#include <QObject>

#include "Heaven/HeavenApi.h"

namespace Heaven
{
	class Action;
	class MergePlace;
	class ActionContainer;

	class UiObject;
	class MenuPrivate;

	class HEAVEN_API Menu : public QObject
	{
		Q_OBJECT
	public:
		Menu( QObject* parent );
		~Menu();

	public:
		QString text() const;
		QString statusTip() const;
		QString toolTip() const;
		bool isEnabled() const;

	public slots:
		void setText( const QString& text );
		void setToolTip( const QString& text );
		void setStatusTip( const QString& text );
		void setEnabled( bool v );
		void setDisabled( bool v );

	public:
		void add( Menu* );
		void add( Action* );
		void add( MergePlace* );
		void add( ActionContainer* );

	public:
		UiObject* uiObject();

	private:
		MenuPrivate*	d;
	};

}

#endif
