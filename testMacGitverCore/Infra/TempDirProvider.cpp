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

#include <stdlib.h>

#include <QDir>
#include <QDateTime>
#include <QStringBuilder>
#include <QCoreApplication>

#include "Infra/TempDirProvider.hpp"

TempDirProvider::TempDirProvider()
{
    Q_ASSERT(sSelf == NULL);
    sSelf = this;

    create();
}

TempDirProvider::~TempDirProvider()
{
    Q_ASSERT(sSelf);
    sSelf = NULL;

    remove();
}

QString TempDirProvider::get()
{
    Q_ASSERT(sSelf);
    return sSelf->tempDir;
}

void TempDirProvider::create()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    QDir base = QDir::temp();
    QString realPath;
    for (int i = 0; i < 10; i++) {
        tempPath =
                QDir(qApp->arguments()[0]).dirName() %
                QChar(L'-') %
                QString::number(qrand(),16);

        if (base.mkpath(tempPath)) {
            realPath = base.filePath(tempPath);
            break;
        }
    }
    if (realPath.isEmpty()) {
        qDebug("Cannot create temp path");
        exit(-1);
    }

    tempDir = realPath;
}

void TempDirProvider::remove()
{
    QDir::temp().rmpath(tempPath);
}

TempDirProvider* TempDirProvider::sSelf = NULL;
