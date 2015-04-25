/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
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

#include "LoggingView.hpp"
#include "LoggingModule.hpp"

#include <QStringBuilder>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>

#include "libLogger/Event.hpp"

#include "libMacGitverCore/Config/Config.h"

LoggingView::LoggingView()
    : View("Log")
{
    setViewName( trUtf8( "Log" ) );
    mBrowser = new QWebView;
    mBrowser->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    setWidget( mBrowser );

    LoggingModule::self()->setView(this);

    connect(&Config::self(), SIGNAL(fontsChanged()),
            this, SLOT(clearPrefixCache()));
}

LoggingView::~LoggingView()
{
    LoggingModule::self()->setView(NULL);
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

    Log::Event::List events = LoggingModule::self()->currentEvents();

    foreach (Log::Event e, events) {

        if (mHiddenChannels.contains(e.channel().name())) {
            continue;
        }

        QString eventHtml;

        if (mCache.contains(e.uniqueId())) {
            eventHtml = mCache[e.uniqueId()];
        }
        else {
            QDateTime stamp = e.timeStamp();
            eventHtml =
                    tr("<div id=\"%3\" class=\"evt\"><span class=\"ts\">%1</span>%2</div>\n")
                       .arg(stamp.time().toString())
                       .arg(e.html())
                       .arg(e.uniqueId());
            mCache.insert(e.uniqueId(), eventHtml);
        }


        html = html % eventHtml;
        lastId = e.uniqueId();
    }

    html += htmlPostfix;

    //qDebug("Setting HTML to\n%s", qPrintable(html));

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
    LoggingModule::self()->queueViewUpdate();
}

void LoggingView::calculatePrefix()
{
    const char* sz =
            "<html>\n"
            "  <head>\n"
            "    <style type=\"text/css\">\n"

            "      body {\n"
            "        margin: 2px;\n"
            "      }\n"

            "      div {\n"
            "        margin: 0px 0px 0px 72px;\n"
            "        padding: 1px 2px 3px 0px;\n"
            "        %1\n"
            "      }\n"

            "      span.ts {\n"
            "        text-color: blue;\n"
            "        padding: 0px 0px 0px 2px;\n"
            "        margin: 0px 7px 0px -72px;\n"
            "        %2\n"
            "      }\n"

            "      code {\n"
            "        margin: 0px;\n"
            "        background-color: #EEE;\n"
            "        border-radius: 5px;\n"
            "        border: 1px solid #CCC;\n"
            "        padding: 0px 1px 0px 1px;\n"
            "        %2\n"
            "      }\n"

            "    </style>\n"
            "  </head>\n"
            "  <body>\n";

    htmlPrefix = QString::fromUtf8(sz)
            .arg(Config::defaultFontCSS())
            .arg(Config::defaultFixedFontCSS());

    htmlPostfix = QLatin1String("</body></html>");
}
