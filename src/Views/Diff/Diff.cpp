
#include <QFile>

#include "Views/Diff/Diff.h"

DifferenceLine::DifferenceLine( const QByteArray& ba, DiffLineType type )
	: mText( ba )
	, mType( type )
{
}

DifferenceHunkSide::DifferenceHunkSide()
	: mStartLineNumber( -1 )
{
}

DifferenceHunkSide::~DifferenceHunkSide()
{
	qDeleteAll( mLines );
}

void DifferenceHunkSide::setStartingLine( quint32 startLineNumber )
{
	mStartLineNumber = startLineNumber;
}

void DifferenceHunkSide::addLine( DifferenceLine* line )
{
	mLines.append( line );
}

int DifferenceHunkSide::numLines() const
{
	return mLines.count();
}

DifferenceLine* DifferenceHunkSide::line( int index )
{
	return mLines[ index ];
}

int DifferenceHunkSide::startingLineNumber() const
{
	return mStartLineNumber;
}

DifferenceHunk::DifferenceHunk( int sides )
	: mNumSides( sides )
{
	mSideHunks = new DifferenceHunkSide[ sides ];
}

DifferenceHunk::~DifferenceHunk()
{
	delete[] mSideHunks;
}

DifferenceHunkSide* DifferenceHunk::side( int i )
{
	Q_ASSERT( i >= 0 && i < mNumSides );
	return mSideHunks + i;
}

const DifferenceHunkSide* DifferenceHunk::side( int i ) const
{
	Q_ASSERT( i >= 0 && i < mNumSides );
	return mSideHunks + i;
}

int DifferenceHunk::maximumDisplayLines() const
{
	int lines = mSideHunks[ 0 ].numLines();

	for( int i = 1; i < mNumSides; i++ )
	{
		if( mSideHunks[ i ].numLines() > lines )
		{
			lines = mSideHunks[ i ].numLines();
		}
	}

	return lines;
}

Difference::Difference( const QStringList& pathNames )
	: mPathNames( pathNames )
{
}

Difference::~Difference()
{
	qDeleteAll( mHunks );
}

QStringList Difference::pathNames() const
{
	return mPathNames;
}

QList< DifferenceHunk* > Difference::allHunks() const
{
	return mHunks;
}

void Difference::addHunk( DifferenceHunk* hunk )
{
	mHunks.append( hunk );
}

int Difference::numHunks() const
{
	return mHunks.count();
}

int Difference::maximumDisplayLines() const
{
	int lines = 0;

	for( int i = 0; i < mHunks.count(); i++ )
	{
		lines += mHunks[ i ]->maximumDisplayLines();
	}

	return lines;
}

Differences::Differences()
{
}

Differences::~Differences()
{
	qDeleteAll( mPaths );
}

void Differences::addPath( Difference* diff )
{
	mPaths.append( diff );
}

QList< Difference* > Differences::allPaths() const
{
	return mPaths;
}

int Differences::numPaths() const
{
	return mPaths.count();
}

Difference* Differences::pathAt( int index )
{
	return mPaths[ index ];
}

Differences* Differences::readPatch( const QString& fileName )
{
	QFile f( fileName );
	if( !f.open( QFile::ReadOnly ) )
	{
		return NULL;
	}

	return readPatch( &f );
}

static QByteArray nextLine( QIODevice* dev )
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

Differences* Differences::readPatch( QIODevice* dev )
{
	Differences* patch = new Differences;

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
				continue;
			pathNames.append( QString( diffParts[ i ] ) );
		}
		Q_ASSERT( pathNames.length() == 2 );

		Difference* diff = new Difference( pathNames );
		patch->addPath( diff );

		do
		{
			line = nextLine( dev );
		} while( !line.startsWith( "---" ) &&
				 !line.startsWith( "diff " ) &&
				 !dev->atEnd() );

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

		do
		{
			line = nextLine( dev );
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

			DifferenceHunk* hunk = new DifferenceHunk( 2 );
			hunk->side( 0 )->setStartingLine( leftStart );
			hunk->side( 1 )->setStartingLine( rightStart );
			diff->addHunk( hunk );

			while( leftLength && rightLength )
			{
				DifferenceLine* diffline;
				line = nextLine( dev );

				switch( line[0] )
				{
				case ' ':
					diffline = new DifferenceLine( line.mid( 1 ), DiffLineContext );
					hunk->side( 0 )->addLine( diffline );
					leftLength --;
					diffline = new DifferenceLine( line.mid( 1 ), DiffLineContext );
					hunk->side( 1 )->addLine( diffline );
					rightLength --;
					break;

				case '-':
					diffline = new DifferenceLine( line.mid( 1 ), DiffLineDeletion );
					hunk->side( 0 )->addLine( diffline );
					leftLength --;
					break;

				case '+':
					diffline = new DifferenceLine( line.mid( 1 ), DiffLineAddition );
					hunk->side( 1 )->addLine( diffline );
					rightLength --;
					break;

				default:
					delete patch;
					return NULL;
				}
			}
		} while( ! dev->atEnd() );
	}

	return patch;
}
