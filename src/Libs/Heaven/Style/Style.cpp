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

#include <QMenuBar>

#include "Style/Style.h"

namespace Heaven
{

	static bool isStyled( const QWidget* w )
	{
		if( !w ||
			( w->window()->windowFlags() & Qt::WindowType_Mask ) == Qt::Dialog )
		{
			return false;
		}

		for( const QWidget* p = w; p; p = p->parentWidget() )
		{
			if( qobject_cast< const QMenuBar* >( p ) ||
				p->property( "heavenStyle" ).toBool() )
			{
				return true;
			}
		}

		return false;
	}

	Style::Style( QStyle* baseStyle )
		: QProxyStyle( baseStyle )
	{
	}

	int Style::pixelMetric( PixelMetric metric, const QStyleOption* option,
							const QWidget* widget ) const
	{
		int retval = QProxyStyle::pixelMetric( metric, option, widget );

		switch (metric)
		{
		case PM_MenuPanelWidth:
		case PM_MenuBarHMargin:
		case PM_MenuBarVMargin:
		case PM_ToolBarFrameWidth:
			if( isStyled( widget ) )
				retval = 1;
			break;

		case PM_MenuBarPanelWidth:
			if( isStyled( widget ) )
				retval = 0;
			break;

		default:
			break;
		}

		return retval;
	}

}
