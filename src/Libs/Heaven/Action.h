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

#ifndef MGV_HEAVEN_ACTION_H
#define MGV_HEAVEN_ACTION_H

#include <QString>
#include <QObject>

class QAction;

#include "Heaven/HeavenApi.h"

namespace Heaven
{
	class UiObject;
	class ActionPrivate;

	class HEAVEN_API Action : public QObject
	{
		Q_OBJECT
	public:
		Action( QObject* parent );
		~Action();

	public:
		QString text() const;
		QString statusTip() const;
		QString toolTip() const;
		bool isEnabled() const;
		bool isCheckable() const;
		bool isChecked() const;

	public slots:
		void setText( const QString& text );
		void setToolTip( const QString& text );
		void setStatusTip( const QString& text );
		void setEnabled( bool v );
		void setDisabled( bool v );
		void setCheckable( bool v );
		void setChecked( bool v );

	signals:
		void triggered();
		void toggled( bool checked );

	public:
		QAction* actionFor( QObject* parent );

	public:
		UiObject* uiObject();

	private:
		ActionPrivate* d;
	};

}

#endif
