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

#include "GitWrap/Repository.h"

#include "Libs/Core/ConfigPage.h"

class /* MGV_CORE_API */ ModuleInterface
{
public:
	enum Type
	{
		View			= 1 << 0,


		None			= 0	// Silly :)
	};
	typedef QFlags< Type > Types;

public:
	virtual ~ModuleInterface();

public:
	virtual void repositoryChanged( Git::Repository newRepository ) = 0;

	virtual void setupConfigPages( ConfigDlg* dlg ) = 0;
	virtual Types providesModuleTypes() const = 0;

	virtual void initialize() = 0;
	virtual void deinitialize() = 0;
};

Q_DECLARE_INTERFACE( ModuleInterface,
					 "org.babbelbox.sacu.macgitver.ModuleInterface/0.1" )

class /* MGV_CORE_API */ Module : public QObject, public ModuleInterface
{
	Q_INTERFACES( ModuleInterface )
	Q_OBJECT
public:
	Module();

public:
	void repositoryChanged( Git::Repository newRepository );

	void setupConfigPages( ConfigDlg* dlg );
	Types providesModuleTypes() const = 0;

	void initialize();
	void deinitialize();
};

#define IMPLEMENT_INTERNAL_MODULE(Name) \
	Module* createIntModule_##Name() \
	{ \
		return new Name##Module(); \
	}

#endif
