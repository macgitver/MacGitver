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

#include "libLogger/Api.hpp"

#include <QSharedData>


class QString;

namespace Log
{

    class Event;

    class LOGGER_API Template
    {
    public:
        Template(const Template& other);
        Template();
        ~Template();
        Template& operator=(const Template& other);
        bool isValid() const;

    public:
        static Template create(const QString& name);

    public:
        QString name() const;
        void setTransformation(const QString& transformText);

    public:
        QString apply(Event event) const;

    private:
        class Data;
        Template(Data* _d);
        QExplicitlySharedDataPointer<Data> d;
    };

}
