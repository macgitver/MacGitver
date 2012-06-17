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

#ifndef DIFF_DIFF_H
#define DIFF_DIFF_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QSharedData>

class QTextStream;

#include "Diff/Model/DiffModelApi.h"

class DIFF_MODEL_API DiffLines
{
public:
	DiffLines();

public:
	int numLines() const;
	QString lineAt( int i ) const;
	void addLine( const QString& line );

	int firstLine() const;
	int lastLine() const;

	void setFirstLine( int i );

	void exportRaw( QTextStream& stream, char prefix );

private:
	int					mFirstLine;
	QStringList			mLines;
};

class DIFF_MODEL_API Difference : public QSharedData
{
public:
	typedef QExplicitlySharedDataPointer< Difference > Ptr;
	typedef QList< Ptr > List;

public:
	enum Type
	{
		Context,
		Delete,
		Insert,
		Change
	};

public:
	Difference( int numSides, Type t = Context );
	~Difference();

public:
	void setType( Type t );
	Type type() const;

	DiffLines* sideLines( int side );

	int numLines( int side ) const;
	int firstLine( int side ) const;

	void exportRaw( QTextStream& stream );

private:
	Type				mType;
	QList< DiffLines* >	mSides;
};

#endif
