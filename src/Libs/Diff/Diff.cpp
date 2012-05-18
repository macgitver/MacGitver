
#include <QTextStream>

#include "Libs/Diff/Diff.h"

static inline QString untabbify( const QString& input )
{
	QString output;
	output.reserve( input.length() );
	for( int i = 0; i < input.length(); i++ )
	{
		if( input[ i ] == '\t' )
		{
			do
			{
				output += ' ';
			} while( output.length() % 4 );
		}
		else
		{
			output += input[ i ];
		}
	}

	return output;
}

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

void DiffLines::exportRaw( QTextStream& stream, char prefix )
{
	for( int i = 0; i < mLines.count(); i++ )
	{
		stream << prefix << untabbify( mLines[ i ] ) << "\n";
	}
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

int Difference::numLines( int side ) const
{
	DiffLines* s = mSides[ side ];
	Q_ASSERT( s );
	return s->numLines();
}

int Difference::firstLine( int side ) const
{
	DiffLines* s = mSides[ side ];
	Q_ASSERT( s );
	return s->firstLine();
}

void Difference::exportRaw( QTextStream& stream )
{
	Q_ASSERT( mSides.count() == 2 );

	if( mType == Context )
	{
		mSides[ 0 ]->exportRaw( stream, ' ' );
	}
	else
	{
		mSides[ 0 ]->exportRaw( stream, '-' );
		mSides[ 1 ]->exportRaw( stream, '+' );
	}
}

