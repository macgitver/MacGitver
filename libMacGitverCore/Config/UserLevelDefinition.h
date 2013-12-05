/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#include "libMacGitverCore/MacGitverApi.hpp"

class QDomElement;

namespace BlueSky
{
    class Mode;
}

class UserLevelDefinition;

class MGV_CORE_API EnableDisable
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

class MGV_CORE_API EnableDisableList
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

class MGV_CORE_API UserLevelMode : public QSharedData
{
public:
    typedef QExplicitlySharedDataPointer< UserLevelMode > Ptr;
private:
    UserLevelMode();
public:
    UserLevelMode( UserLevelDefinition* level, const QString& modeName );
    ~UserLevelMode();

public:
    QString heavenModeName() const;
    QString name() const;
    EnableDisableList& allowedViews();
    const EnableDisableList& allowedViews() const;

    bool isLockingMode() const;
    bool isUserSelectable() const;

public:
    static UserLevelMode::Ptr read( UserLevelDefinition* level, const QDomElement& el );

private:
    UserLevelDefinition*        mLevel;
    QString                     mModeName;
    BlueSky::Mode*              mHeavenMode;
    EnableDisableList           mAllowedViews;
    bool                        mIsLocking;
    bool                        mIsUserSelectable;
};

class MGV_CORE_API UserLevelDefinition : public QSharedData
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
    QString id() const;
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
    QString                     mName;
    QString                     mId;
    int                         mAppLevel;
    int                         mPrecedence;
    QString                     mDescription;
    QList< UserLevelMode::Ptr > mModes;
    QHash< QString, QString >   mHeavenPresets;
};

#endif
