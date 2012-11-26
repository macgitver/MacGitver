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

#ifndef MGV_CONFIG_WIDGET_H
#define MGV_CONFIG_WIDGET_H

#include <QDialog>
#include <QHash>
#include <QByteArray>

class QTreeWidgetItem;

#include "Config/Ui/ConfigPage.hpp"

namespace Ui
{
    class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigDialog();
    ~ConfigDialog();

public:
    int exec();

public:
    virtual void setModified( ConfigPage* page, bool value );
    virtual void addPage( ConfigPage* page );

private slots:
    void onApply();
    void onWidgetChange( QTreeWidgetItem* newCurrent );

private:
    Ui::ConfigDialog*                       ui;
    QHash< QByteArray, QTreeWidgetItem* >   mGroupsById;
    QHash< QByteArray, ConfigPage* >       mPagesById;
    QHash< QTreeWidgetItem*, QByteArray >   mPageIdsByTree;
};

#endif
