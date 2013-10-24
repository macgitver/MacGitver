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

#ifndef MGV_CORE_CONFIG_PAGE_PROVIDER_HPP
#define MGV_CORE_CONFIG_PAGE_PROVIDER_HPP

#include "libMacGitverCore/MacGitverApi.hpp"

class ConfigDialog;

class MGV_CORE_API ConfigPageProvider
{
public:
    ConfigPageProvider();
    virtual ~ConfigPageProvider();

public:
    /**
     * @brief       Setup a pages for the configuration dialog.
     *
     * @param[in]   dialog  The dialog to add the pages to.
     *
     */
    virtual void setupConfigPages(ConfigDialog* dialog) = 0;

    /**
     * @brief       Get the proirity in which this provider shall be run
     *
     * @return      CONSTANT priority.
     *
     */
    virtual int configPagePriority() const = 0;
};

#endif
