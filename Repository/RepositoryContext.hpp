/*
 * libHeaven - A Qt-based ui framework for strongly modularized applications
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#ifndef REPOSITORY_CONTEXT_HPP
#define REPOSITORY_CONTEXT_HPP

#include "libMacGitverCore/MacGitver/IRepositoryContext.hpp"

#include "libHeaven/CentralUI/Contexts/ViewContext.hpp"

class RepositoryContext : public Heaven::ViewContext, public IRepositoryContext
{
    Q_OBJECT
    Q_INTERFACES( IRepositoryContext )

public:
    RepositoryContext();

public:
    void setRepository( RepositoryInfo* repo );
    RepositoryInfo* repository();

private:
    RepositoryInfo*         mRepo;
};

#endif
