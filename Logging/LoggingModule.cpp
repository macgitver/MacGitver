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

#include <QtPlugin>

#include "libMacGitverCore/Log/LogEvent.hpp"

#include "LoggingModule.hpp"
#include "LoggingView.hpp"
#include "LoggingMode.hpp"

LoggingModule::LoggingModule()
    : mQueuedUpdate(false)
    , mView(NULL)
    , mMode(NULL)
{
    sSelf = this;
}

LoggingModule::~LoggingModule() {
    sSelf = NULL;
}

LoggingModule* LoggingModule::self() {
    return sSelf;
}

void LoggingModule::initialize() {

    mMode = new LoggingMode(this);

    registerView<LoggingView>(tr("Logbook"));
    registerMode(mMode);

    MacGitver::log().setLogConsumer(this);
}

void LoggingModule::deinitialize() {
    MacGitver::log().setLogConsumer(NULL);

    unregisterView<LoggingView>();
    unregisterMode(mMode);

    delete mMode;
    mMode = NULL;
}

void LoggingModule::setView(LoggingView* view) {
    mView = view;
    if (view) {
        queueViewUpdate();
    }
}

void LoggingModule::channelAdded(Log::Channel channel) {
    queueViewUpdate();
}

void LoggingModule::logCleaned(quint64 upToId)
{
    Log::Event::List old = mEvents;
    mEvents.clear();

    foreach (Log::Event e, old) {
        if (e.uniqueId() > upToId) {
            mEvents.append(e);
        }
    }

    if (mView) {
        mView->clearCache();
    }

    queueViewUpdate();
}

void LoggingModule::eventAdded(Log::Event e)
{
    mEvents.append(e);
    queueViewUpdate();
}

void LoggingModule::queueViewUpdate() {

    if (!mQueuedUpdate) {
        mQueuedUpdate = true;
        QMetaObject::invokeMethod(this, "viewUpdate", Qt::QueuedConnection);
    }
}

Log::Event::List LoggingModule::currentEvents() const {
    return mEvents;
}

void LoggingModule::viewUpdate() {
    if (mView) {
        mView->regenerate();
    }
    mQueuedUpdate = false;
}

LoggingModule* LoggingModule::sSelf = NULL;

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( Logging, LoggingModule )
#endif
