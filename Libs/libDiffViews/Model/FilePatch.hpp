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

#ifndef DIFFVIEWS_MODEL_FILE_PATCH_HPP
#define DIFFVIEWS_MODEL_FILE_PATCH_HPP

#include <QVector>
#include <QStringList>
#include <QSharedData>

class QTextStream;

#include "libDiffViews/DiffViewsApi.hpp"

namespace DiffViews
{

    class TextFilePatch;
    class BinaryFilePatch;

    class DIFF_VIEWS_API FilePatch : public QSharedData
    {
    public:
        typedef QExplicitlySharedDataPointer< FilePatch > Ptr;
        typedef QVector< Ptr > List;

        enum Type
        {
            TextPatch,
            BinaryPatch
        };

    protected:
        FilePatch( const QStringList& pathNames );

    public:
        virtual ~FilePatch();

    public:
        virtual Type type() const = 0;
        virtual void exportRaw( QTextStream& stream );
        virtual QExplicitlySharedDataPointer< TextFilePatch > asTextFilePatch();
        virtual QExplicitlySharedDataPointer< BinaryFilePatch > asBinaryFilePatch();

    public:
        QStringList pathNames() const;

        void addOptionLine( const QString& line );
        void addOption( const QString& option );

    private:
        QStringList mPathNames;
        QStringList mOptions;
        QStringList mOptionLines;
    };

}

#endif
