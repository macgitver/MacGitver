/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Nils Fenner <nilsfenner@web.de>
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

#ifndef MGV_SH_MACRO_PARSER
#define MGV_SH_MACRO_PARSER

#include "libMacGitverCore/MacGitverApi.hpp"

template <class Key, class T> class QHash;
class QString;

class MGV_CORE_API ShMacroParser
{
public:
    ShMacroParser();
    ~ShMacroParser();

    QString expandMacros(const QString& input, const QHash<QString, QString>& macros) const;
};

#endif
