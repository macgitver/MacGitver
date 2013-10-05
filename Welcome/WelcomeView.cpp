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

#include <QTextBrowser>
#include <QFile>

#include "WelcomeView.h"

WelcomeView::WelcomeView()
    : View( "Welcome" )
{
    setViewName( trUtf8( "Welcome" ) );

    mBrowser = new QTextBrowser;
    mBrowser->setFrameShape( QFrame::NoFrame );
    mBrowser->setOpenExternalLinks( true );

    // NOTE: Set qss style because QTextDocument doesn't support background-repeat.
    QString css = QString::fromUtf8(
                "background-image: url(':/ModWelcome/images/bg_logo.png');"
                "background-attachment: scroll;"
                "background-repeat: no-repeat;"
                "background-position: center center;"
                "background-color: qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #FFFFFF, stop:.25 #CBD0D8, stop:.5 #BBC0CA, stop:.75 #CBD0D8, stop:1 #FFFFFF );"
                );
    mBrowser->setStyleSheet( css );

    QTextDocument *doc = new QTextDocument;
//    doc->addResource( QTextDocument::ImageResource, QUrl( QLatin1String("images/bg_logo.png") ), QPixmap( QLatin1String(":/ModWelcome/images/bg_logo.png") ) );

//    QFile css( QLatin1String( ":/ModWelcome/format.css" ) );
//    css.open( QFile::ReadOnly );
//    doc->addResource( QTextDocument::StyleSheetResource, QUrl( QLatin1String("format.css") ), css.readAll() );

    QFile f( QLatin1String( ":/ModWelcome/Welcome.html" ) );
    f.open( QFile::ReadOnly );
    doc->setHtml( QString::fromUtf8( f.readAll().constData() ) );

    mBrowser->setDocument( doc );

    setWidget( mBrowser );
}


