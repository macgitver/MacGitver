/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#ifndef MGV_DIFF_PATCH_FILE_H
#define MGV_DIFF_PATCH_FILE_H

#include <QStringList>

class QTextStream;

#include "Diff/Model/DiffModelApi.h"

class DifferenceHunk;

class DIFF_MODEL_API PatchFile
{
public:
	PatchFile( const QStringList& pathNames );
	~PatchFile();

public:
	QStringList pathNames() const;
	QList< DifferenceHunk* > allHunks() const;
	void addHunk( DifferenceHunk* hunk );
	int numHunks() const;

	void exportRaw( QTextStream& stream );

	void addOptionLine( const QString& line );
	void addOption( const QString& option );

private:
	QStringList					mPathNames;
	QStringList					mOptions;
	QStringList					mOptionLines;
	QList< DifferenceHunk* >	mHunks;
};

#endif
