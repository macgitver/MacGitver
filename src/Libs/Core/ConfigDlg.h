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

#ifndef MGV_CONFIG_WIDGET_H
#define MGV_CONFIG_WIDGET_H

#include <QDialog>
#include <QHash>
#include <QByteArray>

class QSplitter;
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;

class ConfigPageGroup;
class ConfigPage;

namespace Ui
{
	class ConfigDlg;
}

class ConfigDlg : public QDialog
{
	Q_OBJECT
public:
	ConfigDlg();
	~ConfigDlg();

public:

private:
	Ui::ConfigDlg*								ui;
	QHash< ConfigPageGroup*, QTreeWidgetItem* >	mGroupItems;
	QHash< QTreeWidgetItem*, ConfigPage* >		mItemPages;
};

#endif
