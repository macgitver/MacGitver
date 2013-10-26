/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
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

#ifndef MGV_MOD_REPO_MAN_LOGGER_TMPL_NAMES_HPP
#define MGV_MOD_REPO_MAN_LOGGER_TMPL_NAMES_HPP

#define TEMPLATE_NAME(name) \
    QLatin1String(#name)

#define TMPL_REPO_ACTIVITY              TEMPLATE_NAME(RepoActivity)
#define TMPL_FOUND_NEW_OBJ              TEMPLATE_NAME(FoundNewObj)
#define TMPL_FOUND_NEW_REF              TEMPLATE_NAME(FoundNewRef)
#define TMPL_BRANCH_MOVED               TEMPLATE_NAME(BranchMoved)

#endif
