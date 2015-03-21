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
#include <QFile>

#include "libDiffViews/Model/Patch.hpp"

namespace DiffViews
{

    Patch::Patch()
        : mNumSides( 0 )
    {
    }

    Patch::~Patch()
    {
    }

    void Patch::setNumSides( int sides )
    {
        mNumSides = sides;
    }

    int Patch::numSides() const
    {
        return mNumSides;
    }


    void Patch::addPath( FilePatch::Ptr patch )
    {
        mPaths.append( patch );
    }

    FilePatch::List Patch::allPaths() const
    {
        return mPaths;
    }

    int Patch::numPaths() const
    {
        return mPaths.count();
    }

    FilePatch::Ptr Patch::pathAt( int index )
    {
        return mPaths[ index ];
    }

    Patch::Ptr Patch::readPatch( const QString& fileName )
    {
        QFile f( fileName );
        if( !f.open( QFile::ReadOnly ) )
        {
            return Patch::Ptr();
        }

        return readPatch( &f );
    }

    static inline QByteArray nextLine( QIODevice* dev )
    {
        QByteArray line = dev->readLine();
        if( line.endsWith( '\n' ) )
        {
            return line.left( line.length() - 1 );
        }
        else
        {
            return line;
        }
    }

    Patch::Ptr Patch::readPatch( QIODevice* dev )
    {
        Patch::Ptr patch( new Patch );
        patch->setNumSides( 2 );

        QByteArray line = nextLine( dev );
        while( ! dev->atEnd() )
        {
            if( !line.startsWith( "diff " ) )
            {
                return Patch::Ptr();
            }

            QList< QByteArray > diffParts = line.split( ' ' );
            QStringList pathNames;
            for( int i = 1; i < diffParts.count(); i++ )
            {
                if( diffParts[ i ].startsWith( "--" ) )
                {
                    continue;
                }

                pathNames.append( QString::fromUtf8( diffParts[ i ].constData() ) );
            }
            Q_ASSERT( pathNames.length() == 2 );

            TextFilePatch::Ptr diff( new TextFilePatch( pathNames ) );
            patch->addPath( diff );
            for( int i = 1; i < diffParts.count(); i++ )
            {
                if( !diffParts[ i ].startsWith( "--" ) )
                {
                    continue;
                }

                diff->addOption( QString::fromUtf8( diffParts[ i ].constData() ) );
            }

            line = nextLine( dev );

            while( !line.startsWith( "---" ) &&
                   !line.startsWith( "diff " ) &&
                   !dev->atEnd() )
            {
                diff->addOptionLine( QString::fromUtf8( line.constData() ) );
                line = nextLine( dev );
            }

            if( dev->atEnd() )
            {
                return Patch::Ptr();
            }

            if( line.startsWith( "diff " ) )
            {
                continue;
            }

            line = nextLine( dev );
            if( !line.startsWith( "+++" ) )
            {
                return Patch::Ptr();
            }

            line = nextLine( dev );
            do
            {
                if( line.startsWith( "diff " ) )
                {
                    break;
                }

                if( !line.startsWith( "@@" ) )
                {
                    return Patch::Ptr();
                }

                QList< QByteArray > hunkParts = line.split( ' ' );
                Q_ASSERT( hunkParts[ 1 ].startsWith( '-' ) );
                Q_ASSERT( hunkParts[ 2 ].startsWith( '+' ) );

                QList< QByteArray > lineParts = hunkParts[ 1 ].mid( 1 ).split( ',' );
                int leftStart = lineParts[ 0 ].toInt();
                int leftLength = lineParts[ 1 ].toInt();

                lineParts = hunkParts[ 2 ].mid( 1 ).split( ',' );
                int rightStart = lineParts[ 0 ].toInt();
                int rightLength = lineParts[ 1 ].toInt();

                Hunk::Ptr hunk( new Hunk );
                diff->addHunk( hunk );

                if( hunkParts.count() > 4 )
                {
                    QString name = QString::fromUtf8( hunkParts[ 4 ].constData() );

                    for( int i = 5; i < hunkParts.count(); i++ )
                    {
                        name += L' ' + QString::fromUtf8( hunkParts[ i ].constData() );
                    }

                    hunk->setHunkHeader( name );
                }

                line = nextLine( dev );

                while( leftLength || rightLength )
                {
                    if( line[ 0 ] == ' ' )
                    {
                        HunkPart::Ptr differ( new HunkPart( 2, HunkPart::Context ) );
                        hunk->addPart( differ );
                        differ->sideLines( 0 )->setFirstLine( leftStart );
                        differ->sideLines( 1 )->setFirstLine( rightStart );

                        while( line[ 0 ] == ' ' && ( leftLength || rightLength ) )
                        {
                            QString realLine = QString::fromUtf8( line.mid( 1 ).constData() );
                            differ->sideLines( 0 )->addLine( realLine );
                            leftLength--;
                            leftStart++;

                            differ->sideLines( 1 )->addLine( realLine );
                            rightLength--;
                            rightStart++;

                            line = nextLine( dev );
                        }
                    }
                    else
                    {
                        HunkPart::Ptr differ( new HunkPart( 2, HunkPart::Change ) );
                        hunk->addPart( differ );
                        differ->sideLines( 0 )->setFirstLine( leftStart );
                        differ->sideLines( 1 )->setFirstLine( rightStart );

                        while( line[ 0 ] == '-' && ( leftLength || rightLength ) )
                        {
                            QString realLine = QString::fromUtf8( line.mid( 1 ).constData() );
                            differ->sideLines( 0 )->addLine( realLine );
                            leftLength--;
                            leftStart++;

                            line = nextLine( dev );
                        }

                        while( line[ 0 ] == '+' && ( leftLength || rightLength ) )
                        {
                            QString realLine = QString::fromUtf8( line.mid( 1 ).constData() );
                            differ->sideLines( 1 )->addLine( realLine );
                            rightLength--;
                            rightStart++;

                            line = nextLine( dev );
                        }

                        if( differ->sideLines( 0 )->numLines() == 0 )
                        {
                            differ->setType( HunkPart::Insert );
                        }
                        else if( differ->sideLines( 1 )->numLines() == 0 )
                        {
                            differ->setType( HunkPart::Delete );
                        }
                    }
                }

            } while( ! dev->atEnd() );
        }

        return patch;
    }

    void Patch::exportRaw( QTextStream& stream )
    {
        for( int i = 0; i < mPaths.count(); i++ )
        {
            mPaths[ i ]->exportRaw( stream );
        }
    }

    QString Patch::toString()
    {
        QString result;
        QTextStream ts( &result );
        exportRaw( ts );
        return result;
    }


}
