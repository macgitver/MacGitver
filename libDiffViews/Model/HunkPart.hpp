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

#ifndef DIFFVIEWS_MODEL_HUNK_PART_HPP
#define DIFFVIEWS_MODEL_HUNK_PART_HPP

#include <QString>
#include <QSharedData>

class QTextStream;

#include "libDiffViews/DiffViewsApi.hpp"
#include "libDiffViews/Model/DiffLines.hpp"

namespace DiffViews
{

    class DIFF_VIEWS_API HunkPart : public QSharedData
    {
    public:
        typedef QExplicitlySharedDataPointer< HunkPart > Ptr;
        typedef QVector< Ptr > List;

    public:
        enum Type
        {
            Context,
            Delete,
            Insert,
            Change
        };

    public:
        HunkPart( int numSides, Type t = Context );
        ~HunkPart();

    public:
        void setType( Type t );
        Type type() const;

        DiffLines* sideLines( int side );

        int numLines( int side ) const;
        int firstLine( int side ) const;

        void exportRaw( QTextStream& stream );

        void totalChanges( int& added, int& removed ) const;

    private:
        Type            mType;
        DiffLines::List mSides;
    };

}

#endif
