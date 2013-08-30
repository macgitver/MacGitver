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

#ifndef HISTORY_DETAILS_H
#define HISTORY_DETAILS_H

#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/Repository.hpp"

#include <QTextBrowser>

enum HistoryHeaderDetails
{
    HHD_Subject = 1,
    HHD_Author,
    HHD_AuthorName,
    HHD_AuthorMail,
    HHD_AuthorDate,
    HHD_Committer,
    HHD_CommitterName,
    HHD_CommitterDate,
    HHD_CommitterMail,
    HHD_SHA1,
    HHD_ChildrenList,
    HHD_ParentsList
};

class HistoryDetails : public QTextBrowser
{
    Q_OBJECT
public:
    HistoryDetails( QWidget* parent = 0 );

public:
    void setRepository( Git::Repository repo );
    void setCommit( const Git::ObjectId& sha1 );
    void clear();

private:
    void updateText();
    void readConfig();

private:
    Git::ObjectId                   mCurrentSHA1;
    Git::Repository                 mRepo;
    QVector< HistoryHeaderDetails > mViewDetailRows;
};

#endif
