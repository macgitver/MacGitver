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

#ifndef MGV_MODULE_GITCONFIGWIDGET_H
#define MGV_MODULE_GITCONFIGWIDGET_H

#include <QWidget>

class QFileSystemWatcher;

#include "GitWrap/Config.h"

#include "ui_GitConfigWidget.h"

class GitConfigWidget : public QWidget, private Ui::GitConfigWidget
{
	Q_OBJECT

	enum KeyType
	{
		KeySection,
		KeyValue,
		KeyMultiVarHead,
		KeyMultiVarValue
	};

	struct EntryData
	{
		KeyType				type;
		QTreeWidgetItem*	treeItem;
		QString				fullKey;
		QString				value;
	};

public:
	GitConfigWidget();

public:
	void setConfig( const QString& fileName );
	void setConfigs( const QStringList& fileNames );

private:
	void setConfig( const Git::Config& config );
	void syncConfig();
	void allItems( QSet< QTreeWidgetItem* >& result, QTreeWidgetItem* parent );

private slots:
	void configChanged();

private:
	bool				mEditable;
	QStringList			mConfigFiles;
	Git::Config			mConfig;
	QFileSystemWatcher*	mWatcher;
	QHash< QTreeWidgetItem*, EntryData* >	mEntries;
	QHash< QString, EntryData* >			mSectionEntries;
	QHash< QString, EntryData* >			mValueEntries;
};

#endif
