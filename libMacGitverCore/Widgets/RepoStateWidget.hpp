/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef MGV_REPO_STATE_WIDGET_HPP
#define MGV_REPO_STATE_WIDGET_HPP

class QLabel;

#include <QWidget>

namespace RM
{
    class Repo;
}

class RepoStateWidget : public QWidget
{
    Q_OBJECT
public:
    RepoStateWidget();

private slots:
    void repositoryActivated(RM::Repo* info);
    void repositoryDeactivated(RM::Repo* info);

private:
    void setupUi();
    void setRepoState();

private:
    RM::Repo*       repo;
    QLabel*         txthlRepo;
    QLabel*         txthlState;
    QLabel*         txthlBranch;

    QLabel*         txtRepo;
    QLabel*         txtState;
    QLabel*         txtBranch;
};

#endif


