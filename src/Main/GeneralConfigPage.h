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

#ifndef MGV_GENERAL_CONFIG_PAGE_H
#define MGV_GENERAL_CONFIG_PAGE_H

#include <QWidget>

#include "Libs/Interfaces/IConfigPage.h"
#include "Libs/Interfaces/IConfigDialog.h"

namespace Ui
{
	class GeneralConfigPage;
}

class GeneralConfigPage : public QWidget, public IConfigPage
{
	Q_OBJECT
public:
	GeneralConfigPage( IConfigDialog* dlg );
	~GeneralConfigPage();

public:
	void apply();
	void init();

	QByteArray pageId() const;
	QByteArray groupId() const;

	QString pageName() const;
	QString groupName() const;

	QWidget* widget();

private:
	Ui::GeneralConfigPage*		ui;
};

#endif
