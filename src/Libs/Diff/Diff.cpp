
#include "Libs/Diff/Diff.h"

DiffLines::DiffLines()
{
}

int DiffLines::numLines() const
{
	return mLines.count();
}

QString DiffLines::lineAt( int i ) const
{
	return mLines[ i ];
}

void DiffLines::addLine( const QString& line )
{
	mLines.append( line );
}

int DiffLines::firstLine() const
{
	return mFirstLine;
}

int DiffLines::lastLine() const
{
	return mFirstLine + mLines.count() - 1;
}

void DiffLines::setFirstLine( int i )
{
	mFirstLine = i;
}

Difference::Difference( int numSides, Type t )
	: mType( t )
{
	while( numSides-- )
	{
		mSides.append( new DiffLines() );
	}
}

Difference::~Difference()
{
}

Difference::Type Difference::type() const
{
	return mType;
}

DiffLines* Difference::sideLines( int side )
{
	return mSides[ side ];
}

void Difference::setType( Type t )
{
	mType = t;
}
