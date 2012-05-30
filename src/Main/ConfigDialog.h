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

#ifndef MGV_CONFIG_WIDGET_H
#define MGV_CONFIG_WIDGET_H

#include <QDialog>
#include <QHash>
#include <QByteArray>

class QTreeWidgetItem;

#include "Interfaces/IConfigDialog.h"

#include "ui_ConfigDialog.h"

class ConfigDialog : public QDialog, public IConfigDialog, private Ui::ConfigDialog
{
	Q_OBJECT
public:
	ConfigDialog();
	~ConfigDialog();

public:
	void addPage( IConfigPage* page );

private:
	QHash< QByteArray, QTreeWidgetItem* >	mGroupsById;
	QHash< QByteArray, IConfigPage* >		mPagesById;
	QHash< QTreeWidgetItem*, QByteArray >	mPageIdsByTree;
};

#endif
