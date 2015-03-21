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

#ifndef DIFFVIEWS_MODEL_HUNK_HPP
#define DIFFVIEWS_MODEL_HUNK_HPP

#include "libDiffViews/Model/HunkPart.hpp"

namespace DiffViews
{

    class DIFF_VIEWS_API Hunk : public QSharedData
    {
    public:
        typedef QExplicitlySharedDataPointer< Hunk > Ptr;
        typedef QVector< Ptr > List;

    public:
        Hunk();
        ~Hunk();

    public:
        void addPart( HunkPart::Ptr part );
        HunkPart::List parts() const;

        int numLines( int side ) const;
        int firstLine( int side ) const;

        void exportRaw( QTextStream& stream );

        QString area() const;
        QString completeHeader() const;

        void setHunkHeader( const QString& header );
        QString hunkHeader() const;

        void totalChanges( int& added, int& removed ) const;

    private:
        QString         mHunkHeader;
        HunkPart::List  mParts;
    };

}

#endif
