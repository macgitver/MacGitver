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

#ifndef MGV_MODULE_LOGGING_H
#define MGV_MODULE_LOGGING_H

#include "libMacGitverCore/MacGitver/Module.h"

#include "libMacGitverCore/Log/LogConsumer.hpp"
#include "libMacGitverCore/Log/LogEvent.hpp"

class LoggingView;

class LoggingModule : public Module, private Log::Consumer
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "org.macgitver.Module/0.1" FILE "Module.json" )
    Q_INTERFACES( Module )

public:
    LoggingModule();
    ~LoggingModule();

public:
    void initialize();
    void deinitialize();

public:
    void setView(LoggingView* view);
    Log::Event::List currentEvents() const;

private:
    void channelAdded(Log::Channel channel);
    void logCleaned(quint64 upToId);
    void eventAdded(Log::Event e);

    void queueViewUpdate();

private slots:
    void viewUpdate();

private:
    bool mQueuedUpdate;
    LoggingView* mView;
    Log::Event::List mEvents;

private:
    static LoggingModule* sSelf;
    static Heaven::View* createLoggingView();
};

#endif
