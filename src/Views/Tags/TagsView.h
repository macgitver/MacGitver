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

#ifndef MGV_TAGS_VIEW_H
#define MGV_TAGS_VIEW_H

#include "Libs/Heaven/HView.h"

#include "Libs/Git/Repository.h"

class QListWidget;

class TagsView : public HeavenView
{
	Q_OBJECT
public:
	TagsView();

private slots:
	void repositoryChanged( Git::Repository repo );

private:
	Git::Repository		mRepo;
	QListWidget*		mListWidget;
};

#endif
