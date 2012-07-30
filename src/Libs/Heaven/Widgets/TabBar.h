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

#ifndef TABBAR_H
#define TABBAR_H

#include <QWidget>
#include <QString>

namespace Heaven
{

	class TabBar : public QWidget
	{
		Q_OBJECT
	private:
		class Private;
		friend class Private;

	public:
		enum TabPos
		{
			North,
			South,
			West,
			East
		};

	public:
		explicit TabBar( QWidget* parent = NULL );
		~TabBar();

	signals:
		void currentChanged( int index );

	public slots:
		void setText( int index, const QString tabName );
		int addTab( const QString& tabName );
		int insertTab( int before, const QString& tabName );
		void removeTab( int index );
		void setCurrentIndex( int index );

	public:
		void setTabPos( TabPos pos );
		TabPos tabPos() const;

	public:
		int currentIndex();
		int count();

	public:
		QSize sizeHint() const;
		QSize minimumSizeHint() const;

	protected:
		bool event( QEvent* ev );

	private:
		Private* d;
	};

}

#endif
