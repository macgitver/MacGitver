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

#include <QWebView>

#include "LoggingView.hpp"

LoggingView::LoggingView()
    : View( "Log" )
{
    setViewName( trUtf8( "Log" ) );

    mBrowser = new QWebView;

    setWidget( mBrowser );
}

QSize LoggingView::sizeHint() const
{
    return QSize( 300, 110 );
}
