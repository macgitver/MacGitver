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

#include "libDiffViews/Model/TextFilePatch.hpp"
#include "libDiffViews/Model/BinaryFilePatch.hpp"

namespace DiffViews
{

    FilePatch::FilePatch( const QStringList& pathNames )
        : mPathNames( pathNames )
    {
    }

    FilePatch::~FilePatch()
    {
    }

    TextFilePatch::Ptr FilePatch::asTextFilePatch()
    {
        return TextFilePatch::Ptr( NULL );
    }

    BinaryFilePatch::Ptr FilePatch::asBinaryFilePatch()
    {
        return BinaryFilePatch::Ptr( NULL );
    }

    void FilePatch::exportRaw( QTextStream& stream )
    {
        Q_ASSERT( mPathNames.count() == 2 );

        stream << "diff " << mOptions.join( QLatin1String( " " ) );
        if( mOptions.count() > 0 )
            stream << ' ';
        stream << mPathNames.join( QLatin1String( " " ) ) << '\n';

        stream << mOptionLines.join( QLatin1String( "\n" ) );
        if( mOptionLines.count() > 0 )
            stream << '\n';

        stream << "--- " << mPathNames[ 0 ] << '\n';
        stream << "+++ " << mPathNames[ 1 ] << '\n';
    }

    QStringList FilePatch::pathNames() const
    {
        return mPathNames;
    }

    void FilePatch::addOptionLine( const QString& line )
    {
        mOptionLines.append( line );
    }

    void FilePatch::addOption( const QString& option )
    {
        mOptions.append( option );
    }

}


