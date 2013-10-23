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

#include <QStringBuilder>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>

#include "LoggingView.hpp"
#include "LoggingModule.hpp"

#include "libMacGitverCore/Log/LogEvent.hpp"

#include "libMacGitverCore/Config/Config.h"

LoggingView::LoggingView(LoggingModule* module)
    : View("Log")
    , mModule(module)
{
    setViewName( trUtf8( "Log" ) );
    mBrowser = new QWebView;
    mBrowser->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    setWidget( mBrowser );

    mModule->setView(this);

    connect(&Config::self(), SIGNAL(fontsChanged()),
            this, SLOT(clearPrefixCache()));
}

LoggingView::~LoggingView()
{
    mModule->setView(NULL);
}

QSize LoggingView::sizeHint() const
{
    return QSize( 300, 110 );
}

void LoggingView::clearCache()
{
    mCache.clear();
}

void LoggingView::regenerate()
{
    quint64 lastId = 0;
    QString html;
    if (htmlPrefix.isEmpty()) {
        calculatePrefix();
    }

    html = htmlPrefix;

    Log::Event::List events = mModule->currentEvents();

    foreach (Log::Event e, events) {

        if (mHiddenChannels.contains(e.channel().name())) {
            continue;
        }

        QString eventHtml;

        if (mCache.contains(e.uniqueId())) {
            eventHtml = mCache[e.uniqueId()];
        }
        else {
            eventHtml = e.html();
            mCache.insert(e.uniqueId(), eventHtml);
        }

        html = html % QLatin1Literal("<p>") % eventHtml % QLatin1Literal("</p>");
        lastId = e.uniqueId();
    }

    html += htmlPostfix;

    mBrowser->setHtml(html);

    if (lastId) {
        QString anchor = QString::number(lastId);
        mBrowser->page()->mainFrame()->scrollToAnchor(anchor);
    }
}

void LoggingView::clearPrefixCache()
{
    htmlPrefix = QString();
    htmlPostfix = QString();
    mModule->queueViewUpdate();
}

void LoggingView::calculatePrefix()
{
    const char* sz =
            "<html>"
              "<head>"
                "<style>"
                  "body { margin: 2px; } "
                  "p { margin: 0px; %1 } "
                  "code { margin: 0px; border: 1px solid #CCC; padding: 1px; %2 }"
                "</style>"
              "</head>"
              "<body>";

    htmlPrefix = QString::fromUtf8(sz)
            .arg(Config::defaultFontCSS())
            .arg(Config::defaultFixedFontCSS());

    htmlPostfix = QLatin1String("</body></html>");
}
