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

#ifndef CFGCOMP_MODEL_HPP
#define CFGCOMP_MODEL_HPP

#include <QDomElement>

#include "VariantTypes.hpp"

class ConfigSetting
{
public:
	ConfigSetting( QDomElement el );

public:
	QString name() const;
	VariantType type() const;

	QString typeName() const;
	QString subTypeName() const;

	QString validatorRule() const;

	QString defaultValue() const;

private:
	QString			mName;
	QString			mDefaultValue;
	QString			mType;
	QString			mSubType;
	QString			mValidatorRule;
};

class ConfigSection
{
public:
	ConfigSection( QDomElement el );
	~ConfigSection();

public:
	QString className() const;
	QString configPath() const;

	QList< ConfigSetting* > settings() const;

private:
	QString					mConfigPath;
	QString					mClassName;
	QList< ConfigSetting* >	mSettings;
};

#endif
