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

#ifndef MGV_I_CONFIG_PAGE_H
#define MGV_I_CONFIG_PAGE_H

#include <QtPlugin>

#include "Interfaces/InterfaceApi.h"

class QWidget;

class IConfigDialog;

class INTERFACES_API IConfigPage
{
public:
	IConfigPage( IConfigDialog* dlg );
	virtual ~IConfigPage();

public:
	virtual QByteArray pageId() const = 0;
	virtual QString pageName() const = 0;
	virtual QByteArray groupId() const = 0;
	virtual QString groupName() const = 0;

	virtual QWidget* widget() = 0;

protected:
	void setModified( bool value = true );

private:
	IConfigDialog*			mDialog;
};

Q_DECLARE_INTERFACE( IConfigPage,
					 "org.macgitver.macgitver.configpage/1.0")

#endif
