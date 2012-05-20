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

#ifndef MGV_CONFIG_PAGE_H
#define MGV_CONFIG_PAGE_H

#include <QWidget>
#include <QHash>

class ConfigPageGroup
{
public:
	ConfigPageGroup();
	ConfigPageGroup( ConfigPageGroup* parent, const QString& name, const QByteArray& id );

public:
	QByteArray id() const;
	QString name() const;
	ConfigPageGroup* parent();

public:
	void addPageGroup( ConfigPageGroup* child );
	ConfigPageGroup* getPageGroup( const QByteArray& id );

private:
	ConfigPageGroup*	mParent;
	QString				mName;
	QByteArray			mId;
	QHash< QByteArray, ConfigPageGroup* > mChildren;
};

class ConfigDlg;

class ConfigPage : public QWidget
{
	Q_OBJECT
public:
	ConfigPage( ConfigDlg* dlg );

protected:
	ConfigPageGroup* getOrMakeGroup( const QString& name,
									 const QByteArray& id,
									 ConfigPageGroup* parent = 0 );
	void setModified();

public:
	virtual void apply() = 0;
	virtual void init() = 0;

	virtual ConfigPageGroup* group() = 0;
	virtual QString name() const = 0;

	ConfigDlg* configDialog();

private:
	ConfigDlg* mDialog;
};

#endif
