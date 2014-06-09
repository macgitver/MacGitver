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

#include <QTextEdit>

#include "Views/Raw/RawHighlighter.hpp"

namespace DiffViews
{

    RawHighlighter::RawHighlighter( QTextEdit* editor )
        : QSyntaxHighlighter( editor->document() )
    {
    }

    void RawHighlighter::highlightBlock( const QString& text )
    {
        QColor c = Qt::black;

        if( text.startsWith( QLatin1String( "diff" ) ) ||
            text.startsWith( QLatin1String( "---" ) ) ||
            text.startsWith( QLatin1String( "+++" ) ) )
        {
            c = Qt::darkGreen;
        }
        else if( text.startsWith( L'-' ) )
        {
            c = Qt::darkRed;
        }
        else if( text.startsWith( L'+' ) )
        {
            c = Qt::darkCyan;
        }
        else if( text.startsWith( L' ' ) )
        {
            c = Qt::gray;
        }
        else if( text.startsWith( QLatin1String( "@@" ) ) )
        {
            int i = text.indexOf( QLatin1String( "@@" ), 2 );
            i += 2;
            setFormat( 0, i, Qt::darkYellow );
            setFormat( i, text.length() - i, Qt::blue );
            return;
        }

        int i = text.length();
        while( i && text[ --i ].isSpace() )
            /* */ ;

        i++;
        if( i != text.length() )
        {
            setFormat( 0, i, c );
            QTextCharFormat tcf;
            tcf.setBackground( Qt::red );
            tcf.setForeground( Qt::white );
            setFormat( i, text.length() - i, tcf );
        }
        else
        {
            setFormat( 0, text.length(), c );
        }
    }

}
