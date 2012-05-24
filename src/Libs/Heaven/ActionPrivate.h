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

#ifndef MGV_HEAVEN_ACTION_PRIVATE_H
#define MGV_HEAVEN_ACTION_PRIVATE_H

#include <QSet>

class QAction;

#include "Heaven/Action.h"
#include "Heaven/UiObject.h"

namespace Heaven
{

	class ActionPrivate : public UiObject
	{
		Q_OBJECT
	public:
		ActionPrivate( Action* owner );
		~ActionPrivate();

	public:
		QAction* createQAction( QObject* forParent );
		QAction* getOrCreateQAction( QObject* forParent );

		void setText( const QString& text );
		void setToolTip( const QString& text );
		void setStatusTip( const QString& text );
		void setEnabled( bool v );
		void setCheckable( bool v );
		void setChecked( bool v );

	private slots:
		void qactionDestroyed();
		void qactionTriggered();
		void qactionToggled( bool checked );

	signals:
		void triggered();
		void toggled( bool checked );

	public:
		UiObjectTypes type() const;

	public:
		bool				mEnabled	: 1;
		bool				mCheckable	: 1;
		bool				mChecked	: 1;
		QString				mText;
		QString				mToolTip;
		QString				mStatusTip;
		QSet< QAction* >	mQActions;
	};

}

#endif
