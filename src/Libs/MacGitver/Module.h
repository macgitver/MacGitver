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

#ifndef MGV_MODULE_H
#define MGV_MODULE_H

#include <QObject>

#include "GitWrap/Repository.h"

#include "Interfaces/IModule.h"
#include "Interfaces/IMainWindow.h"

#include "MacGitver/MacGitverApi.h"

class MGV_CORE_API Module : public QObject, public IModule
{
	Q_INTERFACES( IModule )
	Q_OBJECT
public:
	Module();

public:
	IMainWindow* mainWindow();
};

#endif
