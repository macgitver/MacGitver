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

#ifndef DIFFVIEWS_MODEL_PATCH_HPP
#define DIFFVIEWS_MODEL_PATCH_HPP

#include "libDiffViews/Model/TextFilePatch.hpp"

namespace DiffViews
{

    class DIFF_VIEWS_API Patch : public QSharedData
    {
    public:
        typedef QExplicitlySharedDataPointer< Patch > Ptr;

    public:
        Patch();
        ~Patch();

    public:
        void addPath( FilePatch::Ptr patch );
        FilePatch::List allPaths() const;
        int numPaths() const;
        FilePatch::Ptr pathAt( int index );

        void setNumSides( int sides );
        int numSides() const;

        static Patch::Ptr readPatch( const QString& fileName );
        static Patch::Ptr readPatch( QIODevice* device );

        void exportRaw( QTextStream& stream );

        QString toString();

    private:
        int             mNumSides;
        FilePatch::List mPaths;
    };

}

#endif
