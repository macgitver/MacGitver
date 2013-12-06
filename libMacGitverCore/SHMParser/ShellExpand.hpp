/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#ifndef SHELL_EXPAND_HPP
#define SHELL_EXPAND_HPP

#include "libMacGitverCore/MacGitverApi.hpp"

#include <QString>
#include <QHash>

class MGV_CORE_API ShellExpand
{
private:
    class State;

public:
    typedef QHash<QString, QString> Macros;

    ShellExpand();

public:
    bool overwriteMacroDeclarations() const;
    void setOverwriteMacroDeclarations(bool enabled);

    bool addMacro(const QString &name, const QString &value);
    bool addMacros(const Macros& macros);

    QString expandText(const QString& input);
    QString expandFile(const QString& fileName);

private:
    QString replacementLogic(QString parameter, QString command = QString(), QString arg = QString());
    inline bool processExternal(QString &value, const QString &command, const QString &arg);

private:
    Macros  mMacros;
    bool    mOverwriteMacroDecls;
};


#endif
