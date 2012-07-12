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

#ifndef MGV_MGV_USERLEVELDEF_H
#define MGV_MGV_USERLEVELDEF_H

#include <QString>
#include <QStringList>
#include <QHash>

#include "Config/ConfigApi.h"

class QDomElement;

class CONFIG_API EnableDisable
{
public:
	EnableDisable();
	EnableDisable( const QString& regex, bool enable );

public:
	QString regex() const;
	bool enable() const;

private:
	QString	mRegex;
	bool	mEnable;
};

class CONFIG_API EnableDisableList
{
public:
	EnableDisableList();

public:
	bool read( const QDomElement& parent );

	QStringList appliedTo( const QStringList& list ) const;

public:
	int count() const;
	const EnableDisable& at( int index ) const;

private:
	QList< EnableDisable > mList;
};

class UserLevelMode
{
public:
	UserLevelMode( const QString& modeName );

public:
	QString name() const;
	EnableDisableList& allowedViews();
	const EnableDisableList& allowedViews() const;

private:
	QString				mModeName;
	EnableDisableList	mAllowedViews;
};

class CONFIG_API UserLevelDefinition
{
public:
	UserLevelDefinition( const QString& name, int appLevel, int precedence );

public:
	void setDescription( const QString& desc );
	void addPreset( const QString& type, const QString& preset );

public:
	QString name() const;
	int precedence() const;
	QString description() const;
	void readGuiDef(const QString& fileName );
	QString preset( const QString& type ) const;

private:
	QString						mName;
	int							mAppLevel;
	int							mPrecedence;
	QString						mDescription;
	QList< UserLevelMode >		mModes;
	QHash< QString, QString >	mHeavenPresets;
};

#endif
