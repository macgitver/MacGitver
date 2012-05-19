
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>

#include "Libs/Diff/Patch.h"
#include "Libs/Diff/PatchFile.h"
#include "Libs/Diff/Diff.h"
#include "Libs/Diff/DiffHunk.h"

Patch::Patch()
	: mNumSides( 0 )
{
}

Patch::~Patch()
{
	qDeleteAll( mPaths );
}

void Patch::setNumSides( int sides )
{
	mNumSides = sides;
}

int Patch::numSides() const
{
	return mNumSides;
}


void Patch::addPath( PatchFile* diff )
{
	mPaths.append( diff );
}

QList< PatchFile* > Patch::allPaths() const
{
	return mPaths;
}

int Patch::numPaths() const
{
	return mPaths.count();
}

PatchFile* Patch::pathAt( int index )
{
	return mPaths[ index ];
}

Patch* Patch::readPatch( const QString& fileName )
{
	QFile f( fileName );
	if( !f.open( QFile::ReadOnly ) )
	{
		return NULL;
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

Patch* Patch::readPatch( QIODevice* dev )
{
	Patch* patch = new Patch;
	patch->setNumSides( 2 );

	QByteArray line = nextLine( dev );
	while( ! dev->atEnd() )
	{
		if( !line.startsWith( "diff " ) )
		{
			delete patch;
			return NULL;
		}

		QList< QByteArray > diffParts = line.split( ' ' );
		QStringList pathNames;
		for( int i = 1; i < diffParts.count(); i++ )
		{
			if( diffParts[ i ].startsWith( "--" ) )
			{
				continue;
			}

			pathNames.append( QString( diffParts[ i ] ) );
		}
		Q_ASSERT( pathNames.length() == 2 );

		PatchFile* diff = new PatchFile( pathNames );
		patch->addPath( diff );
		for( int i = 1; i < diffParts.count(); i++ )
		{
			if( !diffParts[ i ].startsWith( "--" ) )
			{
				continue;
			}

			diff->addOption( diffParts[ i ] );
		}

		line = nextLine( dev );

		while( !line.startsWith( "---" ) &&
			   !line.startsWith( "diff " ) &&
			   !dev->atEnd() )
		{
			diff->addOptionLine( line );
			line = nextLine( dev );
		}

		if( dev->atEnd() )
		{
			delete patch;
			return NULL;
		}

		if( line.startsWith( "diff " ) )
		{
			continue;
		}

		line = nextLine( dev );
		if( !line.startsWith( "+++" ) )
		{
			delete patch;
			return NULL;
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
				delete patch;
				return NULL;
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

			DifferenceHunk* hunk = new DifferenceHunk();
			diff->addHunk( hunk );

			if( hunkParts.count() > 4 )
			{
				QString name = hunkParts[ 4 ];
				for( int i = 5; i < hunkParts.count(); i++ )
					name += " " + hunkParts[ i ];
				hunk->setHunkName( name );
			}

			line = nextLine( dev );

			while( leftLength || rightLength )
			{
				if( line[ 0 ] == ' ' )
				{
					Difference* differ = new Difference( 2, Difference::Context );
					hunk->addDifference( differ );
					differ->sideLines( 0 )->setFirstLine( leftStart );
					differ->sideLines( 1 )->setFirstLine( rightStart );

					while( line[ 0 ] == ' ' && ( leftLength || rightLength ) )
					{
						QByteArray realLine = line.mid( 1 );
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
					Difference* differ = new Difference( 2, Difference::Change );
					hunk->addDifference( differ );
					differ->sideLines( 0 )->setFirstLine( leftStart );
					differ->sideLines( 1 )->setFirstLine( rightStart );

					while( line[ 0 ] == '-' && ( leftLength || rightLength ) )
					{
						QByteArray realLine = line.mid( 1 );
						differ->sideLines( 0 )->addLine( realLine );
						leftLength--;
						leftStart++;

						line = nextLine( dev );
					}

					while( line[ 0 ] == '+' && ( leftLength || rightLength ) )
					{
						QByteArray realLine = line.mid( 1 );
						differ->sideLines( 1 )->addLine( realLine );
						rightLength--;
						rightStart++;

						line = nextLine( dev );
					}

					if( differ->sideLines( 0 )->numLines() == 0 )
					{
						differ->setType( Difference::Insert );
					}
					else if( differ->sideLines( 1 )->numLines() == 0 )
					{
						differ->setType( Difference::Delete );
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
