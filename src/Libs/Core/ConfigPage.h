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

class ConfigPageGroup
{
public:
	ConfigPageGroup( ConfigPageGroup* parent, const QString& name, const QByteArray& id );

public:
	QByteArray id() const;
	QString name() const;
	ConfigPageGroup* parent();

private:
	ConfigPageGroup*	mParent;
	QString				mName;
	QByteArray			mId;
};

class ConfigWidget;

class ConfigPage : public QWidget
{
	Q_OBJECT
public:
	ConfigPage( ConfigWidget* widget );

protected:
	ConfigPageGroup* getOrMakeGroup( const QString& name, const QByteArray& id );
	void setModified();

public:
	virtual void apply() = 0;
	virtual void init() = 0;

	virtual ConfigPageGroup* group() const = 0;
	virtual QString name() const = 0;

	ConfigWidget* configWidget();

private:
	ConfigWidget* mWidget;
};

#endif
