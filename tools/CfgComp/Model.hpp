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

class ConfigSubSection;

class ConfigSetting
{
public:
	ConfigSetting( QDomElement el, ConfigSubSection* parentSection = NULL );

public:
	QString fullName() const;
	QString fullPath() const;
	bool isSubSectioned() const;

	bool emitSignal() const;

	QString name() const;
	VariantType type() const;

	QString typeName() const;
	QString subTypeName() const;

	QString validatorRule() const;

	QString defaultValue() const;
	QString defaultInitializer() const;

private:
	ConfigSubSection*	mSubSection;
	bool				mEmitSignal;
	QString				mName;
	QString				mDefaultValue;
	QString				mType;
	QString				mSubType;
	QString				mValidatorRule;
};

class ConfigSubSection
{
public:
	ConfigSubSection( QDomElement el, ConfigSubSection* parent = NULL );
	~ConfigSubSection();

public:
	QList< ConfigSubSection* > sections() const;
	QList< ConfigSetting* > settings() const;

	QString name() const;
	QString fullPath() const;
	QString fullName() const;

	void addAllSettings( QList< ConfigSetting* >& settings) const;

private:
	QString						mName;
	ConfigSubSection*			mParent;
	QList< ConfigSubSection* >	mSections;
	QList< ConfigSetting* >		mSettings;
};

class ConfigSection
{
public:
	ConfigSection( QDomElement el );
	~ConfigSection();

public:
	QString className() const;
	QString configPath() const;

	QList< ConfigSetting* > allSettings() const;
	QList< ConfigSetting* > settings() const;
	QList< ConfigSubSection* > sections() const;

private:
	QString						mConfigPath;
	QString						mClassName;
	QList< ConfigSubSection* >	mSections;
	QList< ConfigSetting* >		mSettings;
};

#endif
