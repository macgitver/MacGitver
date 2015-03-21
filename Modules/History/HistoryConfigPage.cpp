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

#include <QSignalMapper>

#include "libMacGitverCore/Config/Config.h"

#include "HistoryConfigPage.h"
#include "HistoryDetails.h"

HistoryConfigPage::HistoryConfigPage( ConfigDialog* dlg )
    : ConfigPage( dlg )
{
    setupUi( this );
    init();
}

HistoryConfigPage::~HistoryConfigPage()
{
}

void HistoryConfigPage::apply()
{
    int i = 0;
    if( chkDiffAndHistoryBottom->isChecked() )
        i = 0;
    else if( chkDiffAboveDetails->isChecked() )
        i = 1;
    else if( chkDiffRight->isChecked() )
        i = 2;

    Config::self().set( "History/SplitLayout", i );
}

void HistoryConfigPage::init()
{
    sswDetailsOverview->addEntry( trUtf8( "Subject" ), false, HHD_Subject );
    sswDetailsOverview->addEntry( trUtf8( "Author" ), false, HHD_Author );
    sswDetailsOverview->addEntry( trUtf8( "Author date" ), false, HHD_AuthorDate );
    sswDetailsOverview->addEntry( trUtf8( "Author name" ), false, HHD_AuthorName );
    sswDetailsOverview->addEntry( trUtf8( "Author mail" ), false, HHD_AuthorMail );
    sswDetailsOverview->addEntry( trUtf8( "Committer" ), false, HHD_Committer );
    sswDetailsOverview->addEntry( trUtf8( "Committer date" ), false, HHD_CommitterDate );
    sswDetailsOverview->addEntry( trUtf8( "Committer name" ), false, HHD_CommitterName );
    sswDetailsOverview->addEntry( trUtf8( "Committer mail" ), false, HHD_CommitterMail );

    QSignalMapper* map = new QSignalMapper( this );
    map->setMapping( chkDiffAndHistoryBottom, 0 );
    map->setMapping( chkDiffAboveDetails, 1 );
    map->setMapping( chkDiffRight, 2 );

    connect( chkDiffAboveDetails, SIGNAL(clicked()), map, SLOT(map()) );
    connect( chkDiffAndHistoryBottom, SIGNAL(clicked()), map, SLOT(map()) );
    connect( chkDiffRight, SIGNAL(clicked()), map, SLOT(map()) );

    int i = Config::self().get( "History/SplitLayout", 0 ).toInt();
    chkDiffAndHistoryBottom->setChecked( i == 0 );
    chkDiffAboveDetails->setChecked( i == 1 );
    chkDiffRight->setChecked( i == 2 );

    connect( map, SIGNAL(mapped(int)), this, SLOT(onDiffPosChanged(int)) );
}

QByteArray HistoryConfigPage::pageId() const
{
    return "History";
}

QByteArray HistoryConfigPage::groupId() const
{
    return "Views";
}

QString HistoryConfigPage::pageName() const
{
    return trUtf8( "History" );
}

QString HistoryConfigPage::groupName() const
{
    return trUtf8( "Views" );
}

void HistoryConfigPage::onDiffPosChanged( int newPos )
{
    setModified();
}
