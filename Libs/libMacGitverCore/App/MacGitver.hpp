/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
 * (C) Cunz RaD Ltd.
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

#pragma once

#include "libMacGitverCore/MacGitverApi.hpp"

#include "libGitWrap/Result.hpp"

#include "libBlueSky/Views.hpp"
#include "libBlueSky/Containers.hpp"
#include "libBlueSky/ViewDescriptor.hpp"

#include "libLogger/Manager.hpp"

#include <QObject>

namespace RM
{
    class RepoMan;
}

typedef BlueSky::ViewDescriptor::CreatorFunc MgvViewCreator;

class MacGitverPrivate;

class MGV_CORE_API MacGitver
        : public QObject
{
    Q_OBJECT
public:
    MacGitver(bool runGui = true);
    ~MacGitver();

public:
    static MacGitver&   self();
    GW_DEPRECATED
    static RM::RepoMan& repoMan();

public:
    static int exec();

public:
    bool isRunningGui() const;

public:
    void integrateView(BlueSky::View* view, BlueSky::Positions place );

    BlueSky::ViewDescriptor* registerView(const BlueSky::ViewIdentifier& identifier,
                                          const QString& displayName,
                                          MgvViewCreator creator);
    void unregisterView(const BlueSky::ViewIdentifier& identifier);

    static void log(Log::Type type, const QString& logMessage);
    static void log(Log::Type type, const char* logMessage);
    static void log(Log::Type type, const Git::Result& r, const char* logMessage = nullptr);

private:
    MacGitverPrivate* d;
};
