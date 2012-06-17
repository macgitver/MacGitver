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
#include <QHash>

#include "Config/ConfigApi.h"

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
	QString preset( const QString& type ) const;

private:
	QString						mName;
	int							mAppLevel;
	int							mPrecedence;
	QString						mDescription;
	QHash< QString, QString >	mHeavenPresets;
};

#endif
