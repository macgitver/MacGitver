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

#ifndef MGV_LOGGING_VIEW_H
#define MGV_LOGGING_VIEW_H

#include <QString>
#include <QSet>

#include "libBlueSky/Views.hpp"

class QWebView;
class LoggingModule;

class LoggingView : public BlueSky::View
{
    Q_OBJECT
public:
    LoggingView();
    ~LoggingView();

public:
    void regenerate();
    void clearCache();

public:
    QSize sizeHint() const;

private slots:
    void clearPrefixCache();

private:
    void calculatePrefix();

private:
    QString htmlPrefix;
    QString htmlPostfix;
    QSet<QString> mHiddenChannels;
    QHash< quint64, QString > mCache;
    LoggingModule* mModule;
    QWebView* mBrowser;
};

#endif

