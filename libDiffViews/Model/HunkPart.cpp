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

#include "libDiffViews/Model/HunkPart.hpp"

namespace DiffViews
{

    HunkPart::HunkPart( int numSides, Type t )
        : mType( t )
    {
        while( numSides-- )
        {
            mSides.append( new DiffLines() );
        }
    }

    HunkPart::~HunkPart()
    {
    }

    HunkPart::Type HunkPart::type() const
    {
        return mType;
    }

    DiffLines* HunkPart::sideLines( int side )
    {
        return mSides[ side ];
    }

    void HunkPart::setType( Type t )
    {
        mType = t;
    }

    int HunkPart::numLines( int side ) const
    {
        DiffLines* s = mSides[ side ];
        Q_ASSERT( s );
        return s->numLines();
    }

    int HunkPart::firstLine( int side ) const
    {
        DiffLines* s = mSides[ side ];
        Q_ASSERT( s );
        return s->firstLine();
    }

    void HunkPart::exportRaw( QTextStream& stream )
    {
        Q_ASSERT( mSides.count() == 2 );

        if( mType == Context )
        {
            mSides[ 0 ]->exportRaw( stream, ' ' );
        }
        else
        {
            mSides[ 0 ]->exportRaw( stream, '-' );
            mSides[ 1 ]->exportRaw( stream, '+' );
        }
    }

    void HunkPart::totalChanges( int& added, int& removed ) const
    {
        if( mType == Context )
        {
            return;
        }

        removed += numLines( 0 );
        added += numLines( 1 );
    }

}
