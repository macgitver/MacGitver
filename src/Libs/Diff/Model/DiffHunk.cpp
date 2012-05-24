/*
 * MacGitver
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

#include "Diff/Model/Diff.h"
#include "Diff/Model/DiffHunk.h"

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
