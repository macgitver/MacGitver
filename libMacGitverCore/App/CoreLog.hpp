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

#ifndef MGV_CORE_LOG_HPP
#define MGV_CORE_LOG_HPP

#include <QDateTime>
#include <QString>
#include <QList>

#include "libMacGitverCore/MacGitverApi.hpp"

enum LogType
{
    ltInformation,
    ltNormal,
    ltWarning,
    ltError
};

class MGV_CORE_API LogEntry
{
public:
    LogEntry( LogType type, const QString& text );

public:
    QDateTime timeStamp() const;
    LogType type() const;
    QString text() const;

private:
    QDateTime   mTimeStamp;
    LogType     mType;
    QString     mText;
};

class MGV_CORE_API CoreLog : public QObject
{
    Q_OBJECT
public:
    CoreLog();
    virtual ~CoreLog();

public:
    void clear();
    void addMessage( LogType type, const QString& text );
    void addMessage( LogEntry* logEntry );

signals:
    void cleared();
    void aboutToRemove( LogEntry* entry );
    void entryAdded( LogEntry* entry );

private:
    QList< LogEntry* > mEntries;
};

#endif
