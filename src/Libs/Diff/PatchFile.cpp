
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

}
