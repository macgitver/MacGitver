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

#include "Views/Diff/DiffRawHighlighter.h"


DiffRawHighlighter::DiffRawHighlighter( QTextEdit* editor )
	: QSyntaxHighlighter( editor )
{
}

void DiffRawHighlighter::highlightBlock( const QString& text )
{
	QColor c = Qt::black;

	if( text.startsWith( "diff" ) || text.startsWith( "---" || text.startsWith( "+++" ) ) )
	{
		c = Qt::darkGreen;
	}
	else if( text.startsWith( '-' ) )
	{
		c = Qt::darkRed;
	}
	else if( text.startsWith( '+' ) )
	{
		c = Qt::darkCyan;
	}
	else if( text.startsWith( ' ' ) )
	{
		c = Qt::gray;
	}
	else if( text.startsWith( "@@" ) )
	{
		int i = text.indexOf( "@@", 2 );
		i += 2;
		setFormat( 0, i, Qt::darkYellow );
		setFormat( i, text.length() - i, Qt::blue );
		return;
	}

	setFormat( 0, text.length(), c );
}
