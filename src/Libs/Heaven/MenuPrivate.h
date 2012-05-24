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

#ifndef MGV_HEAVEN_MENU_PRIVATE_H
#define MGV_HEAVEN_MENU_PRIVATE_H

#include <QSet>

class QMenu;

#include "Heaven/Menu.h"
#include "Heaven/UiContainer.h"

namespace Heaven
{

	class MenuPrivate : public UiContainer
	{
		Q_OBJECT
	public:
		MenuPrivate( Menu* owner );
		~MenuPrivate();

	public:
		QMenu* createQMenu( QWidget* forParent );
		QMenu* getOrCreateQMenu( QWidget* forParent );

	public:
		void setText( const QString& text );
		void setToolTip( const QString& text );
		void setStatusTip( const QString& text );
		void setEnabled( bool v );

	private slots:
		void menuAboutToShow();
		void menuDestroyed();
		void reemergeGuiElement();

	public:
		void setContainerDirty( bool value = true );
		UiObjectTypes type() const;

	public:
		bool			mEnabled	: 1;
		bool			mRebuildQueued;
		QString			mText;
		QString			mToolTip;
		QString			mStatusTip;
		QSet< QMenu* >	mQMenus;
	};

}

#endif
