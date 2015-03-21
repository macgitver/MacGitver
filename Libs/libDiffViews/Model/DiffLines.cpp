/*
 * libDiffViews
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

#include <QTextStream>

#include "libDiffViews/Model/DiffLines.hpp"

namespace DiffViews
{

    /* THIS MUST GO AWAY! */
    static inline QString untabbify( const QString& input )
    {
        QString output;
        output.reserve( input.length() );
        for( int i = 0; i < input.length(); i++ )
        {
            if( input[ i ] == L'\t' )
            {
                do
                {
                    output += L' ';
                } while( output.length() % 4 );
            }
            else
            {
                output += input[ i ];
            }
        }

        return output;
    }


    DiffLines::DiffLines()
    {
    }

    int DiffLines::numLines() const
    {
        return mLines.count();
    }

    QString DiffLines::lineAt( int i ) const
    {
        return mLines[ i ];
    }

    void DiffLines::addLine( const QString& line )
    {
        mLines.append( line );
    }

    int DiffLines::firstLine() const
    {
        return mFirstLine;
    }

    int DiffLines::lastLine() const
    {
        return mFirstLine + mLines.count() - 1;
    }

    void DiffLines::setFirstLine( int i )
    {
        mFirstLine = i;
    }

    void DiffLines::exportRaw( QTextStream& stream, char prefix )
    {
        for( int i = 0; i < mLines.count(); i++ )
        {
            stream << prefix << untabbify( mLines[ i ] ) << "\n";
        }
    }

}

