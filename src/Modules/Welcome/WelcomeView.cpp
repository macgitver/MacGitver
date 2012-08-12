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
#include <QVBoxLayout>
#include <QFile>

#include "WelcomeView.h"

WelcomeView::WelcomeView()
	: View( QLatin1String( "Welcome" ) )
{
	setViewName( trUtf8( "Welcome" ) );

	mBrowser = new QTextBrowser;
	mBrowser->setFrameShape( QFrame::NoFrame );

	QFile f( QLatin1String( ":/ModWelcome/Welcome.html" ) );
	f.open( QFile::ReadOnly );
	mBrowser->setHtml( QString::fromUtf8( f.readAll().constData() ) );

	QVBoxLayout* l = new QVBoxLayout;
	l->addWidget( mBrowser );
	l->setSpacing( 0 );
	l->setMargin( 0 );
	setLayout( l );
}


