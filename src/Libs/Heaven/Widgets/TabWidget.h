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

#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "TabBar.h"

class QStackedWidget;

namespace Heaven
{

	class TabBar;

	class TabWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit TabWidget( QWidget* parent = NULL );

	public:
		int addTab( QWidget* widget, const QString& tabName );
		int insertTab( int before, QWidget* widget, const QString& tabName );
		void removeTab( int index );
		void removeTab( QWidget* widget );
		QWidget* takeAt( int index );
		int indexOf( QWidget* widget );
		int count();

		void setTabPos( TabBar::TabPos pos );

	signals:
		void currentChanged( int newCurrent );

	private slots:
		void tabChanged( int newCurrent );

	private:
		TabBar*			mBar;
		QStackedWidget*	mStack;
	};

}

#endif // TABWIDGET_H
