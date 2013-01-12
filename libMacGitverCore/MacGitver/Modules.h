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

#ifndef MGV_MODULES_H
#define MGV_MODULES_H

#include <QSet>

#include "MacGitver/Module.h"

class MGV_CORE_API Modules : public QObject
{
    Q_OBJECT
public:
    Modules();
    ~Modules();

public:
    void addModule( Module* mod );
    void delModule( Module* mod );

public:
    void initialize();
    void setupConfigPages( ConfigDialog* dlg );

public:
    void repositoryChanged( Git::Repository newRepository );

private:
    void setupInternals();

private:
    QSet< Module* >     mModules;
};

#endif
