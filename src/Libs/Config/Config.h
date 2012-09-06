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

#ifndef MGV_CONFIG_H
#define MGV_CONFIG_H

#include <QList>
#include <QString>
#include <QVariant>
#include <QFont>

class QSettings;

#include "Config/UserLevelDefinition.h"

class CONFIG_API Config : public QObject
{
	Q_OBJECT

public:
	static Config& self();

public:
	void loadSettings();
	void saveSettings();

	void loadLevels( const QString& fileName );
	QList< UserLevelDefinition::Ptr > levels() const;

	QVariant get( const char* szPath, const char* szDefaultValue ) const;
	QVariant get( const char* szPath, const QVariant& defaultValue = QVariant() ) const;
	QVariant get( const QString& path, const QVariant& defaultValue = QVariant() ) const;
	void set( const QString& path, const QVariant& value );

	static QFont defaultFont();
	static QFont defaultDialogFont();
	static QFont defaultFixedFont();

	void setDefaultFont( const QFont& font );
	void setDefaultDialogFont( const QFont& font );
	void setDefaultFixedFont( const QFont& font );

signals:
	void fontsChanged();

private:
	void addUserLevel( UserLevelDefinition::Ptr level );

private:
	static Config* sSelf;
	Config();
	~Config();
	QList< UserLevelDefinition::Ptr >	mLevels;
	QSettings*							mSettings;
	QFont								mDefaultFont;
	QFont								mDefaultDialogFont;
	QFont								mDefaultFixedFont;
};

#endif
