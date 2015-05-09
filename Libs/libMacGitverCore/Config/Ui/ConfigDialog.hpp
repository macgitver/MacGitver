/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
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

#ifndef MGV_CONFIG_WIDGET_H
#define MGV_CONFIG_WIDGET_H

#include <QHash>
#include <QMap>
#include <QSet>
#include <QByteArray>

class QTreeWidgetItem;

#include "libMacGitverCore/Config/Ui/ConfigPage.hpp"

#include "libBlueSky/Dialog.hpp"

namespace Ui
{
    class ConfigDialog;
}

class ConfigPageProvider;

class ConfigDialog : public BlueSky::Dialog
{
    Q_OBJECT
public:
    ConfigDialog();
    ~ConfigDialog();

public:
    int exec();

public:
    static void registerProvider(ConfigPageProvider* provider);
    static void unregisterProvider(ConfigPageProvider* provider);

public:
    virtual void setModified( ConfigPage* page, bool value );
    virtual void addPage( ConfigPage* page );

private slots:
    void onApply();
    void onWidgetChange( QTreeWidgetItem* newCurrent );

private:
    void setupConfigPages();

private:
    Ui::ConfigDialog*                       ui;
    QHash<QByteArray, QTreeWidgetItem*>     mGroupsById;
    QHash<QByteArray, ConfigPage*>          mPagesById;
    QHash<QTreeWidgetItem*, QByteArray>     mPageIdsByTree;
    static QMap< int, QSet<ConfigPageProvider*> >
                                            sProviders;
};

#endif
