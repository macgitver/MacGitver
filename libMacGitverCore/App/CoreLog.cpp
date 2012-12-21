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

#include "libMacGitverCore/App/CoreLog.hpp"

LogEntry::LogEntry( LogType type, const QString& text )
    : mTimeStamp( QDateTime::currentDateTime() )
    , mType( type )
    , mText( text )
{
}

QDateTime LogEntry::timeStamp() const
{
    return mTimeStamp;
}

QString LogEntry::text() const
{
    return mText;
}

LogType LogEntry::type() const
{
    return mType;
}

CoreLog::CoreLog()
{
}

CoreLog::~CoreLog()
{
}

void CoreLog::addMessage( LogType type, const QString& text )
{
    LogEntry* le = new LogEntry( type, text );
    addMessage( le );
}

void CoreLog::addMessage( LogEntry* logEntry )
{
    if( mEntries.count() == 1000 )
    {
        LogEntry* retire = mEntries.takeFirst();
        emit aboutToRemove( retire );
        delete retire;
    }

    mEntries.append( logEntry );
    emit entryAdded( logEntry );
}

void CoreLog::clear()
{
    QList< LogEntry* > oldEntries = mEntries;
    mEntries.clear();
    emit cleared();
    qDeleteAll( oldEntries );
}


