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

#pragma once
#ifndef MGV_LOG_LOG_TEMPLATE_HPP
#define MGV_LOG_LOG_TEMPLATE_HPP

#include <QSharedData>

class QString;

namespace Log
{

    class Template
    {
    public:
        Template(const Template& other);
        Template();
        ~Template();
        Template& operator=(const Template& other);
        bool isValid() const;

    public:
        static Template create();

    private:
        class Data;
        Template(Data* _d);
        QExplicitlySharedDataPointer<Data> d;
    };

}

#endif
