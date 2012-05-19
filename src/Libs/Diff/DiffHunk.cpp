
#include <QTextStream>

#include "Libs/Diff/Diff.h"
#include "Libs/Diff/DiffHunk.h"

DifferenceHunk::DifferenceHunk()
{
}

DifferenceHunk::~DifferenceHunk()
{
	qDeleteAll( mDifferences );
}

void DifferenceHunk::addDifference( Difference* difference )
{
	mDifferences.append( difference );
}

QList< Difference* > DifferenceHunk::differences() const
{
	return mDifferences;
}

int DifferenceHunk::numLines( int side ) const
{
	int l = 0;

	for( int i = 0; i < mDifferences.count(); i ++ )
	{
		l += mDifferences[ i ]->numLines( side );
	}

	return l;
}

int DifferenceHunk::firstLine( int side ) const
{
	Q_ASSERT( mDifferences.count() > 0 );
	return mDifferences[ 0 ]->firstLine( side );
}

void DifferenceHunk::exportRaw( QTextStream& stream )
{
	stream << "@@ " << firstLine( 0 ) << "," << numLines( 0 ) << " "
		   << firstLine( 1 ) << "," << numLines( 1 ) << " @@";

	if( !mHunkName.isEmpty() )
	{
		stream << " " << mHunkName;
	}

	stream << "\n";

	for( int i = 0; i < mDifferences.count(); i++ )
	{
		mDifferences[ i ]->exportRaw( stream );
	}
}

void DifferenceHunk::setHunkName( const QString& name )
{
	mHunkName = name;
}

QString DifferenceHunk::hunkName() const
{
	return mHunkName;
}
