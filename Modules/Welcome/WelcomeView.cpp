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

#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QWebView>

#include "WelcomeView.h"

WelcomeView::WelcomeView()
    : View( "Welcome" )
{
    setViewName( trUtf8( "Welcome" ) );

    mBrowser = new QWebView;
    mBrowser->page()->setLinkDelegationPolicy( QWebPage::DelegateExternalLinks );
    connect( mBrowser, SIGNAL(linkClicked(QUrl)), this, SLOT(onLinkClicked(const QUrl&)) );

    mBrowser->load( QUrl( QStringLiteral("qrc:/ModWelcome/Welcome.html") ) );

    setWidget( mBrowser );
}

void WelcomeView::onLinkClicked(const QUrl &url)
{
    QDesktopServices::openUrl( url );
}


