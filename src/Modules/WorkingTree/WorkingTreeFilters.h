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

#ifndef MGV_WT_WORKING_TREE_FILTERS_H
#define MGV_WT_WORKING_TREE_FILTERS_H

#include <QFlags>

enum WorkingTreeFilter
{
	WTF_Untracked		= 1 << 0,
	WTF_Missing			= 1 << 1,
	WTF_Changed			= 1 << 2,
	WTF_Unchanged		= 1 << 3,
	WTF_Ignored			= 1 << 4,

	WTF_None			= 0,
	WTF_All				= WTF_Unchanged | WTF_Untracked | WTF_Missing | WTF_Ignored | WTF_Changed
};

typedef QFlags< WorkingTreeFilter > WorkingTreeFilters;

#endif
