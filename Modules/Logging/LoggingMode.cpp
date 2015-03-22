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

#include <QFile>

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "LoggingMode.hpp"

LoggingMode::LoggingMode(QObject *parent)
    : BlueSky::Mode(staticMetaObject.className(), parent)
{
    setName(tr("Logbook"));
    setEnabled(false);
    setDisplayOrder(100);

    connect(&MacGitver::self().repoMan(),
            &RM::RepoMan::hasActiveRepositoryChanged,
            this, &LoggingMode::setEnabled);
}

QString LoggingMode::createDefaultState() const {
    QFile f(QLatin1String(":/Modes/LoggingMode.xml"));
    if (!f.open(QFile::ReadOnly)) {
        return QString();
    }

    return QString::fromUtf8(f.readAll().constData());
}
