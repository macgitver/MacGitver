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

#ifndef MGV_I_MODULE_H
#define MGV_I_MODULE_H

#include <QFlags>

#include "GitWrap/Repository.h"

#include "Interfaces/InterfaceApi.h"

class IConfigDialog;

class INTERFACES_API IModule
{
public:
	enum Type
	{
		ConfigDialog	= 1 << 0,

		View			= 1 << 16,


		None			= 0	// Silly :)
	};
	typedef QFlags< Type > Types;

public:
	IModule();
	virtual ~IModule();

public:
	virtual void repositoryChanged( Git::Repository newRepository ) = 0;

	virtual void setupConfigPages( IConfigDialog* dialog ) = 0;
	virtual Types providesModuleTypes() const = 0;

	virtual void initialize() = 0;
	virtual void deinitialize() = 0;
};

Q_DECLARE_INTERFACE( IModule,
					 "org.babbelbox.sacu.macgitver.IModule/0.1" )

#endif
