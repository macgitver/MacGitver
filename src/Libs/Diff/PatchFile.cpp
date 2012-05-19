
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

void PatchFile::addOptionLine( const QString& line )
{
	mOptionLines.append( line );
}

void PatchFile::addOption( const QString& option )
{
	mOptions.append( option );
}

void PatchFile::exportRaw( QTextStream& stream )
{
	Q_ASSERT( mPathNames.count() == 2 );

	stream << "diff " << mOptions.join( " " );
	if( mOptions.count() > 0 )
		stream << ' ';
	stream << mPathNames.join( " " ) << '\n';

	stream << mOptionLines.join( "\n" );
	if( mOptionLines.count() > 0 )
		stream << '\n';

	stream << "--- " << mPathNames[ 0 ] << '\n';
	stream << "+++ " << mPathNames[ 1 ] << '\n';

	for( int i = 0; i < mHunks.count(); i++ )
	{
		mHunks[ i ]->exportRaw( stream );
	}
}
