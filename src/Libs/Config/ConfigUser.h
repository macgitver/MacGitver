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

#ifndef MGV_CONFIG_USER_H
#define MGV_CONFIG_USER_H

#include <QVariant>

#include "Config/ConfigApi.h"

class CONFIG_API ConfigUser
{
protected:
	ConfigUser( const QString& configBasePath );
	ConfigUser( const char* pszConfigBasePath );
	virtual ~ConfigUser();

public:
	QVariant configGet( const QString& subPath, const QVariant& defaultValue = QVariant() ) const;
	QVariant configGet( const char* pszSubPath, const QVariant& defaultValue = QVariant() ) const;

	void configSet( const QString& subPath, const QVariant& value ) const;
	void configSet( const char* pszsubPath, const QVariant& value ) const;

	QString configSubPath( const QString& subPath ) const;
	QString configSubPath( const char* pszSubPath ) const;

	QString basePath() const;

public:
	virtual void configChanged( const QString& subPath, const QVariant& value );

private:
	QString			mConfigBase;
};

#endif
