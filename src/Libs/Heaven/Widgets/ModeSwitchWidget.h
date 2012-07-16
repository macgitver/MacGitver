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

#ifndef MGV_MAIN_SWITCH_MODE_WIDGET_H
#define MGV_MAIN_SWITCH_MODE_WIDGET_H

#include "Heaven/HeavenApi.h"

#include <QWidget>

class QLabel;
class QStackedLayout;

namespace Heaven
{

	class ModeSwitchCombo;

	class HEAVEN_API ModeSwitchWidget : public QWidget
	{
		Q_OBJECT
	public:
		ModeSwitchWidget();

	public:

	private:
		ModeSwitchCombo*		mSelect;
		QLabel*					mMode;
		QStackedLayout*			mStack;
	};

}

#endif
