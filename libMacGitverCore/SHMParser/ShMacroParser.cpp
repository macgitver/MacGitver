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

#include "ShMacroParser.hpp"

#include <QString>

ShMacroParser::ShMacroParser()
{
}

ShMacroParser::~ShMacroParser()
{
}

/**
 * @brief Expand macros within a given template string.
 * @param input the template string
 * @param macros the macros to replace in the string
 * @return The resulting string with the replaced macros.
 */
QString ShMacroParser::expandMacros(const QString &input, const QHash<QString, QString> &macros) const
{
    // TODO: extend to real parser.
    // ( this is just a simple implementation to start with )
    QString result = input;
    foreach (QString key, macros.keys())
    {
        result.replace( QString::fromUtf8("$%1").arg( key ), macros[key] );
    }

    //    qDebug( qPrintable( result ) );
    return result;
}
