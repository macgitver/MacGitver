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
#include <QVector>
#include <QSharedData>

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
	EnableDisableList( const QDomElement& parent );

public:
	QStringList appliedTo( const QStringList& list ) const;

public:
	int count() const;
	const EnableDisable& at( int index ) const;

private:
	bool read( const QDomElement& parent );

private:
	QList< EnableDisable > mList;
};

class CONFIG_API UserLevelDefaultLayoutEntry : public QSharedData
{
public:
	typedef QExplicitlySharedDataPointer< UserLevelDefaultLayoutEntry > Ptr;

public:
	enum Type
	{
		View,
		Split,
		Tab
	};

	enum TabPos
	{
		Left,
		Right,
		Top,
		Bottom
	};

public:
	UserLevelDefaultLayoutEntry();
	~UserLevelDefaultLayoutEntry();

public:
	Type type() const;
	int numChildren() const;
	UserLevelDefaultLayoutEntry::Ptr childAt( int index ) const;

public:
	static UserLevelDefaultLayoutEntry::Ptr read( const QDomElement& el );

private:
	static bool parseOrient( const QString& s );
	static TabPos parseCaption( const QString& s );

private:
	Type										mType;
	QString										mName;
	int											mStretch;
	bool										mVertical;
	TabPos										mTabPos;
	QVector< UserLevelDefaultLayoutEntry::Ptr >	mChildren;
};

class CONFIG_API UserLevelDefaultLayout : public QSharedData
{
public:
	typedef QExplicitlySharedDataPointer< UserLevelDefaultLayout > Ptr;
public:
	UserLevelDefaultLayout();
	~UserLevelDefaultLayout();

public:
	static UserLevelDefaultLayout::Ptr read( const QDomElement& el );

private:
	UserLevelDefaultLayoutEntry::Ptr mRoot;
};

class CONFIG_API UserLevelMode : public QSharedData
{
public:
	typedef QExplicitlySharedDataPointer< UserLevelMode > Ptr;
public:
	UserLevelMode();
	UserLevelMode( const QString& modeName );
	~UserLevelMode();

public:
	QString name() const;
	EnableDisableList& allowedViews();
	const EnableDisableList& allowedViews() const;

	bool isLockingMode() const;
	bool isUserSelectable() const;

public:
	static UserLevelMode::Ptr read( const QDomElement& el );

private:
	QString						mModeName;
	UserLevelDefaultLayout::Ptr	mDefaultLayout;
	EnableDisableList			mAllowedViews;
	bool						mIsLocking;
	bool						mIsUserSelectable;
};

class CONFIG_API UserLevelDefinition : public QSharedData
{
public:
	typedef QExplicitlySharedDataPointer< UserLevelDefinition > Ptr;

public:
	~UserLevelDefinition();

private:
	UserLevelDefinition();

public:
	void setDescription( const QString& desc );
	void addPreset( const QString& type, const QString& preset );

public:
	QString name() const;
	int precedence() const;
	QString description() const;
	void readGuiDef(const QString& fileName );
	QString preset( const QString& type ) const;

	int numModes() const;
	QList< UserLevelMode::Ptr > userModes() const;
	QList< UserLevelMode::Ptr > allModes() const;
	UserLevelMode::Ptr mode( int index ) const;
	UserLevelMode::Ptr mode( const QString& name ) const;

public:
	static UserLevelDefinition::Ptr read( const QDomElement& el );

private:
	QString						mName;
	int							mAppLevel;
	int							mPrecedence;
	QString						mDescription;
	QList< UserLevelMode::Ptr >	mModes;
	QHash< QString, QString >	mHeavenPresets;
};

#endif
