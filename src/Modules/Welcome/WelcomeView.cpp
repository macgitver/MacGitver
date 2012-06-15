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

#include "WelcomeView.h"

WelcomeView::WelcomeView()
{
	setViewName( trUtf8( "Welcome" ) );

	mBrowser = new QTextBrowser;

	mBrowser->setHtml( "<html><h1>Welcome to MacGitver</h1>"
					   "<p><em>The swiss army knife for GIT</em></p></html>");

	QVBoxLayout* l = new QVBoxLayout;
	l->addWidget( mBrowser );
	setLayout( l );
}


