/*
 * libDiffViews
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

#ifndef LIB_DIFF_VIEWS_API_H
#define LIB_DIFF_VIEWS_API_H

#include <qglobal.h>

#ifdef DiffViews_EXPORTS
#   define DIFF_VIEWS_API Q_DECL_EXPORT
#else
#   define DIFF_VIEWS_API Q_DECL_IMPORT
#endif

#endif
