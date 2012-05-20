/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#include "Libs/Core/Module.h"

class ConfigDlg;

class Modules : public QObject
{
	Q_OBJECT
public:
	Modules( QObject* parent );
	~Modules();

public:
	void addModule( Module* mod );
	void delModule( Module* mod );

public:
	void initialize();
	void setupConfigPages( ConfigDlg* dlg );

public:
	void repositoryChanged( Git::Repository newRepository );

private:
	void setupInternals();

private:
	QSet< Module* >			mModules;
};

#endif
