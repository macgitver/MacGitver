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

#ifndef TABBARPRIVATE_H
#define TABBARPRIVATE_H

#include "TabBar.h"

namespace Heaven
{

	class TabBar::Private
	{
	public:
		Private();

		struct TabInfo
		{
			QString				mName;
			mutable QRect		mRect;
		};

		enum MouseStates
		{
			msNone,
			msDown
		};

	public:
		bool event( QEvent* ev );
		void calculate();

	private:
		void paint();
		bool mousePress( QMouseEvent* ev );
		bool mouseRelease( QMouseEvent* ev );
		bool mouseMove( QMouseEvent* ev );
		bool mouseLeave();

	public:
		TabBar*					mWidget;
		QVector< TabInfo >		mInfos;
		int						mCurrent;
		TabBar::TabPos			mTabPos;
		mutable QSize			mSizeHint;
		mutable QSize			mMinSize;
		int						mHovered;
		MouseStates				mMouseState;
	};

}

#endif
