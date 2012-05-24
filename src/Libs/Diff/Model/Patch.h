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

#ifndef MGV_DIFF_PATCH_H
#define MGV_DIFF_PATCH_H

#include <QList>
class QString;
class QIODevice;
class QTextStream;

#include "Diff/Model/DiffModelApi.h"

class PatchFile;

class DIFF_MODEL_API Patch
{
public:
	Patch();
	~Patch();

public:
	void addPath( PatchFile* diff );
	QList< PatchFile* > allPaths() const;
	int numPaths() const;
	PatchFile* pathAt( int index );

	void setNumSides( int sides );
	int numSides() const;

	static Patch* readPatch( const QString& fileName );
	static Patch* readPatch( QIODevice* device );

	void exportRaw( QTextStream& stream );

	QString toString();

private:
	int					mNumSides;
	QList< PatchFile* >	mPaths;
};

#endif
