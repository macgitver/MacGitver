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

#include "RepoManLoggerModule.hpp"

#include "Listener.hpp"
#include "TemplateNames.hpp"

RepoManLoggerModule::RepoManLoggerModule()
{
}

RepoManLoggerModule::~RepoManLoggerModule()
{
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
}

void RepoManLoggerModule::setupTemplates()
{
    Log::Template t = Log::Template::create(TMPL_REPO_ACTIVITY);
    t.setTransformation(tr("Repository <code>$RepoName$</code> was <em>$Action$</em>."));
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

