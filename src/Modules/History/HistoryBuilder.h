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

#ifndef HISTORY_BUILDER_H
#define HISTORY_BUILDER_H

#include <QMap>

#include "Libs/Git/ObjectCommit.h"
#include "Libs/Git/Repository.h"
#include "Libs/Git/RevisionWalker.h"

class HistoryEntries;

class HistoryBuilder
{
public:
	HistoryBuilder( Git::Repository repo, HistoryEntries* entries );
	~HistoryBuilder();

public:
	void addHEAD();
	void addAllRefs();

public:
	void start();

private:
	Git::Repository				mRepo;
	Git::RevisionWalker			mWalker;
	HistoryEntries*				mEntries;
};

#endif
