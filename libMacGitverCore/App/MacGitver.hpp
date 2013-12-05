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

#ifndef MGV_MACGITVER_H
#define MGV_MACGITVER_H

#include <QObject>

#include "libMacGitverCore/MacGitverApi.hpp"
#include "libMacGitverCore/Log/LogManager.hpp"

#include "libGitWrap/Repository.hpp"
#include "libGitWrap/Result.hpp"

#include "libBlueSky/Views.hpp"
#include "libBlueSky/Containers.hpp"
#include "libBlueSky/ViewDescriptor.hpp"

namespace RM
{
    class RepoMan;
}

typedef BlueSky::ViewDescriptor::CreatorFunc MgvViewCreator;

class MacGitverPrivate;

class MGV_CORE_API MacGitver : public QObject
{
    Q_OBJECT
public:
    MacGitver(bool runGui = true);
    ~MacGitver();

public:
    static MacGitver&   self();
    static RM::RepoMan& repoMan();
    static Log::Manager log();

public:
    static int exec();

public:
    bool isRunningGui() const;

public:
    void integrateView(BlueSky::View* view, BlueSky::Positions place );

    void registerView (const BlueSky::ViewIdentifier& identifier, const QString& displayName,
                       MgvViewCreator creator);
    void unregisterView(const BlueSky::ViewIdentifier& identifier);

    static void log( Log::Type type, const QString& logMessage );
    static void log( Log::Type type, const char* logMessage );
    static void log( Log::Type type, const Git::Result& r, const char* logMessage = NULL );

signals:
    void repositoryChanged( const Git::Repository& repo ); /* deprecated */

private:
    MacGitverPrivate* d;
};

#endif

