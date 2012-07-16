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

#ifndef MGV_MAIN_SWITCH_MODE_COMBO_BOX_H
#define MGV_MAIN_SWITCH_MODE_COMBO_BOX_H

#include <QComboBox>

namespace Heaven
{

	class ModeSwitchCombo : public QComboBox
	{
		Q_OBJECT
	public:
		ModeSwitchCombo( QWidget* parent = 0 );

	public:
		void setCurrentMode( const QString& current );
		void setModes( const QStringList& modes, const QString& current = QString() );

		QString currentMode() const;
	};

}

#endif
