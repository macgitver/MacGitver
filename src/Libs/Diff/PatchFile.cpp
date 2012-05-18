
#include <QTextStream>

#include "Libs/Diff/Diff.h"
#include "Libs/Diff/DiffHunk.h"
#include "Libs/Diff/PatchFile.h"

PatchFile::PatchFile( const QStringList& pathNames )
	: mPathNames( pathNames )
{
}

PatchFile::~PatchFile()
{
	qDeleteAll( mHunks );
}

QStringList PatchFile::pathNames() const
{
	return mPathNames;
}

QList< DifferenceHunk* > PatchFile::allHunks() const
{
	return mHunks;
}

void PatchFile::addHunk( DifferenceHunk* hunk )
{
	mHunks.append( hunk );
}

int PatchFile::numHunks() const
{
	return mHunks.count();
}

void PatchFile::exportRaw( QTextStream& stream )
{
	Q_ASSERT( mPathNames.count() == 2 );

	stream << "diff " << mPathNames.join( " " ) << "\n";
	stream << "--- " << mPathNames[ 0 ] << "\n";
	stream << "+++ " << mPathNames[ 1 ] << "\n";

	for( int i = 0; i < mHunks.count(); i++ )
	{
		mHunks[ i ]->exportRaw( stream );
	}
}
