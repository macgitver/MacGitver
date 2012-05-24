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

class QTextStream;

#include "Diff/Model/DiffModelApi.h"

class Difference;

class DIFF_MODEL_API DifferenceHunk
{
public:
	DifferenceHunk();
	~DifferenceHunk();

public:
	void addDifference( Difference* difference );
	QList< Difference* > differences() const;

	int numLines( int side ) const;
	int firstLine( int side ) const;

	void exportRaw( QTextStream& stream );

	void setHunkName( const QString& name );
	QString hunkName() const;

private:
	QString					mHunkName;
	QList< Difference* >	mDifferences;
};

#endif
