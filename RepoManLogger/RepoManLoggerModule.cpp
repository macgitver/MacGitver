/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#include <QtPlugin>

#include "RepoManLoggerModule.hpp"

#include "Listener.hpp"
#include "TemplateNames.hpp"

RepoManLoggerModule::RepoManLoggerModule()
    : listener(NULL)
{
}

RepoManLoggerModule::~RepoManLoggerModule()
{
    if (listener) {
        delete listener;
    }
}

void RepoManLoggerModule::initialize()
{
    setupTemplates();
    setupChannel();
    setupListener();
}

void RepoManLoggerModule::deinitialize()
{
    // Templates should be kept registered
    // Channel should also be kept registered
    // But listener has to be destroyed...
    delete listener;
    listener = NULL;
}

void RepoManLoggerModule::setupTemplates()
{
    Log::Template t = Log::Template::create(TMPL_REPO_ACTIVITY);
    t.setTransformation(tr("Repository <code>$RepoName$</code> was <em>$Action$</em>."));
    MacGitver::log().addTemplate(t);

    t = Log::Template::create(TMPL_FOUND_NEW_OBJ);
    t.setTransformation(tr("Found the $Type$ <em>$ObjName</em> in <code>$RepoName$</code>."));
    MacGitver::log().addTemplate(t);

    t = Log::Template::create(TMPL_FOUND_NEW_REF);
    t.setTransformation(tr("Found the $Type$ <em>$ObjName$</em> at <code>$SHA$</code> "
                           "in repository <code>$RepoName$</code>."));
    MacGitver::log().addTemplate(t);

    t = Log::Template::create(TMPL_BRANCH_MOVED);
    t.setTransformation(tr("Branch <em>$ObjName$</em> "
                           "in repository <code>$RepoName$</code> "
                           "moved to <code>$SHA$</code>."));
    MacGitver::log().addTemplate(t);

    t = Log::Template::create(TMPL_FOUND_NEW_SM);
    t.setTransformation(tr("Found submodule <em>$ObjName$</em> "
                           "in repository <code>$RepoName$</code>."));
    MacGitver::log().addTemplate(t);
}

void RepoManLoggerModule::setupChannel()
{
    repoManChannel = Log::Channel::create(QLatin1String("RepoMan"));
    repoManChannel.setDisplayName(trUtf8("Repository Log"));
    repoManChannel.setDefaultTemplate(defTemplate);
    MacGitver::log().addChannel(repoManChannel);
}

void RepoManLoggerModule::setupListener()
{
    listener = new Listener(repoManChannel);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(RepoManLogger, RepoManLoggerModule)
#endif
