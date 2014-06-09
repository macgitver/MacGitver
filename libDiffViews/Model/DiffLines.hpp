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

#ifndef DIFFVIEWS_MODEL_DIFF_LINES_HPP
#define DIFFVIEWS_MODEL_DIFF_LINES_HPP

#include <QStringList>
#include <QVector>

#include "libDiffViews/DiffViewsApi.hpp"

class QTextStream;

namespace DiffViews
{

    class DIFF_VIEWS_API DiffLines
    {
    public:
        typedef QVector< DiffLines* > List;

    public:
        DiffLines();

    public:
        int numLines() const;
        QString lineAt( int i ) const;
        void addLine( const QString& line );

        int firstLine() const;
        int lastLine() const;

        void setFirstLine( int i );

        void exportRaw( QTextStream& stream, char prefix );

    private:
        int             mFirstLine;
        QStringList     mLines;
    };

}

#endif
