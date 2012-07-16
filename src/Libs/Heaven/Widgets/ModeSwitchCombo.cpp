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

#include "ModeSwitchCombo.h"

namespace Heaven
{

	ModeSwitchCombo::ModeSwitchCombo( QWidget* parent )
		: QComboBox( parent )
	{
	}

	void ModeSwitchCombo::setCurrentMode( const QString& current )
	{
		bool blocked = blockSignals( true );

		for( int i = 0; i < count(); i++ )
		{
			if( itemText( i ) == current )
			{
				setCurrentIndex( i );
				break;
			}
		}

		blockSignals( blocked );
	}

	void ModeSwitchCombo::setModes( const QStringList& modes, const QString& current )
	{
		bool blocked = blockSignals( true );
		clear();

		for( int i = 0; i < modes.count(); i++ )
		{
			addItem( modes[ i ] );
			if( modes[ i ] == current )
			{
				setCurrentIndex( i );
			}
		}

		blockSignals( blocked );

		setVisible( count() );
	}

	QString ModeSwitchCombo::currentMode() const
	{
		return itemText( currentIndex() );
	}

}
