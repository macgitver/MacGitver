
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
