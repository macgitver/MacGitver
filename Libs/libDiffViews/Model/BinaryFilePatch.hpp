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

#ifndef DIFFVIEWS_MODEL_BINARY_FILE_PATCH_HPP
#define DIFFVIEWS_MODEL_BINARY_FILE_PATCH_HPP

#include "libDiffViews/Model/Hunk.hpp"
#include "libDiffViews/Model/FilePatch.hpp"

namespace DiffViews
{

    class DIFF_VIEWS_API BinaryFilePatch : public FilePatch
    {
    public:
        typedef QExplicitlySharedDataPointer< BinaryFilePatch > Ptr;
        typedef QVector< Ptr > List;

    public:
        BinaryFilePatch( const QStringList& pathNames );
        ~BinaryFilePatch();

    public:
        virtual Type type() const;
    };

}

#endif
