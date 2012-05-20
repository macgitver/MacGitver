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

#ifndef MGV_MODULE_H
#define MGV_MODULE_H

#include <QObject>

#include "Libs/Git/Repository.h"

#include "Libs/Core/ConfigPage.h"

class Module : public QObject
{
	Q_OBJECT
public:
	enum Type
	{
		View			= 1 << 0,


		None			= 0	// Silly :)
	};
	typedef QFlags< Type > Types;

public:
	Module();

public:
	virtual void repositoryChanged( Git::Repository newRepository );

	virtual void setupConfigPages( ConfigDlg* dlg );
	virtual Types providesModuleTypes() const = 0;

	virtual void initialize();
	virtual void deinitialize();
};

#define IMPLEMENT_INTERNAL_MODULE(Name) \
	Module* createIntModule_##Name() \
	{ \
		return new Name##Module(); \
	}

#endif
