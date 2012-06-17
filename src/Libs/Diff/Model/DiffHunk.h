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

#ifndef MGV_DIFF_HUNK_H
#define MGV_DIFF_HUNK_H

#include <QList>
#include <QSharedData>

class QTextStream;

#include "Diff/Model/Diff.h"

class DIFF_MODEL_API DifferenceHunk : public QSharedData
{
public:
	typedef QExplicitlySharedDataPointer< DifferenceHunk > Ptr;
	typedef QList< Ptr > List;

public:
	DifferenceHunk();
	~DifferenceHunk();

public:
	void addDifference( Difference::Ptr difference );
	Difference::List differences() const;

	int numLines( int side ) const;
	int firstLine( int side ) const;

	void exportRaw( QTextStream& stream );

	void setHunkName( const QString& name );
	QString hunkName() const;

private:
	QString				mHunkName;
	Difference::List	mDifferences;
};

#endif
