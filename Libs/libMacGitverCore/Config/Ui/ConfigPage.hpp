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

#ifndef MGV_I_CONFIG_PAGE_H
#define MGV_I_CONFIG_PAGE_H

#include <QWidget>

#include "libMacGitverCore/MacGitverApi.hpp"

class ConfigDialog;

class MGV_CORE_API ConfigPage : public QWidget
{
    Q_OBJECT

public:
    ConfigPage(ConfigDialog* dlg);
    virtual ~ConfigPage();

public:
    virtual QByteArray pageId() const = 0;
    virtual QString pageName() const = 0;
    virtual QByteArray groupId() const = 0;
    virtual QString groupName() const = 0;

    virtual void apply() = 0;
    virtual void init() = 0;

protected slots:
    void setModified(bool value = true);

private:
    ConfigDialog*   mDialog;
};

#endif
